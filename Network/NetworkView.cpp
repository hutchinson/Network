//
//  NetworkView.cpp
//  Network
//
//  Created by Dan Hutchinson on 30/10/2014.
//
//

#include "NetworkView.qt.h"

#include <QGLWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>

#include <float.h>

#define INITIAL_WORLD_WIDTH 500
#define INITIAL_WORLD_HEIGHT 500

#define QUAD_BOUNDS_1 64
#define QUAD_BOUNDS_2 128
#define QUAD_BOUNDS_3 192
#define QUAD_BOUNDS_4 256

#define MAX_RESIZE_ATTEMPTS 1

NetworkView::NetworkView(QWidget *parent)
: QGraphicsView(parent)
, _scene(NULL)
, _grid(NULL)
, _hostMap()
, _unplacedHosts()
{
  _scene = new QGraphicsScene(0, 0, (1.0f * INITIAL_WORLD_WIDTH), (1.0f * INITIAL_WORLD_HEIGHT), this);
  
  QGLWidget *glWidget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
  setViewport(glWidget);
  setRenderHint( QPainter::Antialiasing );
  setRenderHint( QPainter::HighQualityAntialiasing );

  setScene(_scene);
  setInteractive(true);
  setMouseTracking(true);
  viewport()->setMouseTracking(true);
  
  setDragMode(QGraphicsView::ScrollHandDrag);

  QBrush backgroundBrush( QColor(128, 128, 128, 128) );
  setBackgroundBrush(backgroundBrush);

  // Set up the grid.
  _grid = new GridGraphicsItem( QRect(0, 0, INITIAL_WORLD_WIDTH, INITIAL_WORLD_HEIGHT));
  _scene->addItem(_grid);

  show();
}

bool NetworkView::_isSpaceOccupiedByHost(const QRect &position)
{
  // Check if there is already a host within this host position?
  QRect exclusionSpace(position);
  exclusionSpace.adjust(-25, -25, 25, 25);
  QList<QGraphicsItem *> itemsAtLocation = _scene->items(exclusionSpace);
  return itemsAtLocation.count() > 1;
}

// The new map size strategy will allow us to resize the map using different
// 'strategies' in future, like using a fib sequence or something.
void NetworkView::_newMapSizeStrategy(double &newWidth, double &newHeight)
{
  if( !(newWidth < 2000.0f) )
    return;

  if((_scene->width() * 2) < DBL_MAX)
    newWidth = _scene->width() * 2;
  if((_scene->height() * 2) < DBL_MAX)
    newHeight = _scene->height() * 2;
}

void NetworkView::settingsDidChange()
{
  _scene->update();
}

void NetworkView::newHostAdded(netviz::HostSP host)
{
  QRect position;
  double currentMapWidth = _scene->width();
  double currentMapHeight = _scene->height();
  _determineCandidatePositionFor(host, position, currentMapWidth, currentMapHeight);
  
  // Only attempt to resize 2 times otherwise drop.
  int numAttempts = 0;
  while(_isSpaceOccupiedByHost(position))
  {
    std::cout << "Attempting to re-position " << host->hostName()
              << " at (" << position.x() << ", " << position.y() << ")" << std::endl;
    
    // Pick a new host world size
    _newMapSizeStrategy(currentMapWidth, currentMapHeight);
    _scene->setSceneRect(0, 0, currentMapWidth, currentMapHeight);
    _grid->setNewGridSize(QRectF(0.0f, 0.0f, currentMapWidth, currentMapHeight));

    // Move all the existing hosts to new positions in the new world.
    for(std::map<HostGraphicsItem*, netviz::HostSP>::iterator it = _hostMap.begin();
        it != _hostMap.end();
        ++it)
    {
      HostGraphicsItem *hostGraphics = (*it).first;
      netviz::HostSP existingHost = (*it).second;

      QRect newHostPosition;
      _determineCandidatePositionFor(existingHost, newHostPosition, currentMapWidth, currentMapHeight);
      hostGraphics->setPos(newHostPosition.center());
    }
    
    // Always zoom out to the extents of the world (we'll have this animate
    // outwards soon.
    fitInView(_scene->itemsBoundingRect(), Qt::KeepAspectRatioByExpanding	);
    _scene->update();

    std::cout << host->hostIP() << " clashed so resized to "
              << currentMapWidth << ", " << currentMapHeight << std::endl;

    // Then try add this host.
    _determineCandidatePositionFor(host, position, currentMapWidth, currentMapHeight);
    if(++numAttempts > MAX_RESIZE_ATTEMPTS)
    {
      std::cout << "Couldn't place " << host->hostName()
                << " after " << MAX_RESIZE_ATTEMPTS
                << " resize attempts at location ("
                << position.x() << ", " << position.y() << ")" << std::endl;
      _unplacedHosts.push_back(host);
      return;
    }
  }
  
  HostGraphicsItem *item = new HostGraphicsItem(position, host);
  _scene->addItem(item);
  _hostMap.insert(std::pair<HostGraphicsItem*, netviz::HostSP>(item, host));
}

void NetworkView::_offsetBox(QRect &box, uint32_t octet, qreal currentXOffset, qreal currentYOffset) const
{
  if(octet < QUAD_BOUNDS_1)
  {
    // Doesn't need to move any where, its the top left quadrant
    // TOP LEFT
    box.moveTo(box.x(), box.y());
  }
  else if(octet >= QUAD_BOUNDS_1 && octet < QUAD_BOUNDS_2)
    // TOP RIGHT
    box.moveTo(box.x() + currentXOffset, box.y());
  else if(octet >= QUAD_BOUNDS_2 && octet < QUAD_BOUNDS_3)
    // BOTTOM RIGHT
    box.moveTo(box.x() + currentXOffset, box.y() + currentYOffset);
  else
    // BOTTOM LEFT
    box.moveTo(box.x(), box.y() + currentYOffset);
}

void NetworkView::_determineCandidatePositionFor(const netviz::HostSP host, QRect &where, qreal initialWidth, qreal initialHeight) const
{
  where.setX(0);
  where.setY(0);
  where.setWidth(CELL_WIDTH);
  where.setHeight(CELL_HEIGHT);

  qreal width = initialWidth / 2;
  qreal height = initialHeight / 2;
  
  // Determine the top level quadrant to add it to.
  uint32_t firstOctet = getIPv4Octet(netviz::One, host->ip());
  _offsetBox(where, firstOctet, width, height);
  
  // Get a little smaller and go again.
  width = width / 2;
  height = height / 2;
  uint32_t secondOctet = getIPv4Octet(netviz::Two, host->ip());
  _offsetBox(where, secondOctet, width, height);
  
  int thirdOctet = netviz::getIPv4Octet(netviz::Three, host->ip());
  int fourthOctet = netviz::getIPv4Octet(netviz::Four, host->ip());

  int finalQuad = (initialWidth / 4) / CELL_WIDTH;
  qreal row = ( (thirdOctet + fourthOctet) * firstOctet % finalQuad);
  qreal col = ( (fourthOctet + thirdOctet) * secondOctet % finalQuad);

  where.moveTo(where.x() + (CELL_WIDTH * row), where.y() + (CELL_HEIGHT * col));
}


// Code taken from:
// http://www.qtcentre.org/wiki/index.php?title=QGraphicsView:_Smooth_Panning_and_Zooming
void NetworkView::wheelEvent(QWheelEvent* event)
{
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  // Scale the view / do the zoom
  double scaleFactor = 1.075;
  if(event->delta() > 0)
  {
    // Zoom in
    scale(scaleFactor, scaleFactor);
  }
  else
  {
    // Zooming out
    scale(1.0 / scaleFactor, 1.0 / scaleFactor);
  }
}
