//
//  NetworkView.cpp
//  Network
//
//  Created by Dan Hutchinson on 30/10/2014.
//
//

#include "NetworkView.qt.h"
#include "GlobalThings.h"

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
, _placementStrategy(QuadrantPlacementStrategy::create())
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

bool NetworkView::isSpaceOccupiedByHost(const QRectF &position)
{
  // Check if there is already a host within this host position?
  QRectF exclusionSpace(position);
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
  QRectF position;
  double currentMapWidth = _scene->width();
  double currentMapHeight = _scene->height();

  _placementStrategy->positionForHost(*this, host, position);

  // Only attempt to resize 2 times otherwise drop.
  int numAttempts = 0;
  while(isSpaceOccupiedByHost(position))
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

      QRectF newHostPosition;
      _placementStrategy->positionForHost(*this, existingHost, newHostPosition);
      hostGraphics->setPos(newHostPosition.center());
    }
    
    // Always zoom out to the extents of the world (we'll have this animate
    // outwards soon.
    fitInView(_scene->itemsBoundingRect(), Qt::KeepAspectRatioByExpanding	);
    _scene->update();

    std::cout << host->hostIP() << " clashed so resized to "
              << currentMapWidth << ", " << currentMapHeight << std::endl;

    // Then try add this host.
    _placementStrategy->positionForHost(*this, host, position);
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
