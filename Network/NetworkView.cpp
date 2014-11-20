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

#define INITIAL_WORLD_WIDTH 500
#define INITIAL_WORLD_HEIGHT 500

#define QUAD_BOUNDS_1 64
#define QUAD_BOUNDS_2 128
#define QUAD_BOUNDS_3 192
#define QUAD_BOUNDS_4 256


NetworkView::NetworkView(QWidget *parent)
: QGraphicsView(parent)
, _scene(NULL)
, _grid(NULL)
, _hostMap()
{
  _scene = new QGraphicsScene(0, 0, INITIAL_WORLD_WIDTH, INITIAL_WORLD_HEIGHT, this);
  
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
//  std::cout << "Num Items: " << itemsAtLocation.count() << std::endl;
  return itemsAtLocation.count() > 1;
}

// The new map size strategy will allow us to resize the map using different
// 'strategies' in future, like using a fib sequence or something.
void NetworkView::_newMapSizeStrategy(int &newWidth, int &newHeight)
{
  if((_scene->width() * 2) > 0)
    newWidth = _scene->width() * 2;
  if((_scene->height() * 2)> 0)
    newHeight = _scene->height() * 2;
}

void NetworkView::newHostAdded(netviz::HostSP host)
{
  QRect position;
  int currentMapWidth = _scene->width();
  int currentMapHeight = _scene->height();
  _determineCandidatePositionFor(host, position, currentMapWidth, currentMapHeight);
  
  // Only attempt to resize 3 times otherwise drop.
  int numAttempts = 0;
  while(_isSpaceOccupiedByHost(position))
  {
    std::cout << "Attempting to re-position " << host->hostName() << " at (" << position.x() << ", " << position.y() << ")" << std::endl;
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

//    ensureVisible(_scene->itemsBoundingRect());
    _scene->update();

    std::cout << host->hostName() << " clashed so resized to " << currentMapWidth << ", " << currentMapHeight << std::endl;

    // Then try add this host.
    _determineCandidatePositionFor(host, position, currentMapWidth, currentMapHeight);
    if(++numAttempts > 3)
    {
      std::cout << "Couldn't fit " << host->hostName() << " after 3 resize attempts at location (" << position.x() << ", " << position.y() << ")" << std::endl;
      assert("HASH COLLISION: Could not fit to map");
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
    // TOP LEFT
    box.moveTo(box.x(), box.y());
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
  uint32_t currentOctet = getIPv4Octet(netviz::One, host->ip());
  _offsetBox(where, currentOctet, width, height);
  
  // Get a little smaller and go again.
  width = width / 2;
  height = height / 2;
  currentOctet = getIPv4Octet(netviz::Two, host->ip());
  _offsetBox(where, currentOctet, width, height);
  
  int thirdOctet = netviz::getIPv4Octet(netviz::Three, host->ip());
  int fourthOctet = netviz::getIPv4Octet(netviz::Four, host->ip());

  int finalQuad = (initialWidth / 4) / CELL_WIDTH;
  qreal row = (thirdOctet % finalQuad); //% finalQuad;
  qreal col = (fourthOctet % finalQuad); //% finalQuad;

  std::cout << "Placing " << host->hostIP() << " at ( " << row << ", " << col << ")" << std::endl;
  where.moveTo(where.x() + (CELL_WIDTH * row), where.y() + (CELL_HEIGHT * col));
}

