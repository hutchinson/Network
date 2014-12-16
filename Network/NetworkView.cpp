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

  // 'Jet' gray
  QBrush backgroundBrush( QColor(52, 52, 52, 190) );
  setBackgroundBrush(backgroundBrush);

  // Set up the grid.
  _grid = new GridGraphicsItem( QRect(0, 0, INITIAL_WORLD_WIDTH, INITIAL_WORLD_HEIGHT));
  _scene->addItem(_grid);

  show();
}

bool NetworkView::isSpaceOccupiedByHost(const QRectF &position) const
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
  // There are two approaches I've considered so far to how we should place hosts
  // in the scene.
  //
  // 1). We try to place the host, if we can't, we resize the map until we find
  // a unique location for it. i.e. the placement algorithm says 'no' room in the
  // place I want to put this host, make the map bigger and i'll try again, but
  // eventually give up.
  //
  // 2). We decide how full the map should get, and we force every placement
  // algorithm to determine a unique place for the host based on the current
  // map's bounds - in this approach, we enfore order on our unruly networked
  // world :)
  
  QRectF position;
  double currentMapWidth = _scene->width();
  double currentMapHeight = _scene->height();

  // Should we re-size the map i.e. are things getting a little crowded?
  qreal totalHostSpaces = 1.0f * ((_scene->width() / CELL_WIDTH) *  (_scene->width() / CELL_WIDTH));
  qreal currentHostsTotal = 1.0f * _hostMap.size();
  qreal mapToSpaceRatio = (currentHostsTotal / totalHostSpaces);
  if(mapToSpaceRatio > 0.05)
  {
    // Resize the map...
    // Pick a new host world size
    _newMapSizeStrategy(currentMapWidth, currentMapHeight);
    _scene->setSceneRect(0, 0, currentMapWidth, currentMapHeight);
    _grid->setNewGridSize(QRectF(0.0f, 0.0f, currentMapWidth, currentMapHeight));
    
    // Move all the existing hosts to new positions in the new world.
    for(std::map<std::string, HostGraphicsItem*>::iterator it = _hostMap.begin();
        it != _hostMap.end();
        ++it)
    {
      HostGraphicsItem *hostGraphics = (*it).second;
      netviz::HostSP existingHost = hostGraphics->host();
      
      QRectF newHostPosition;
      _placementStrategy->positionForHost(*this, existingHost, newHostPosition);
      hostGraphics->moveHostTo(newHostPosition.center());
    }
  }

  _placementStrategy->positionForHost(*this, host, position);
  if(isSpaceOccupiedByHost(position))
  {
    // Tut tut... we told you we'd crash your app but no, we're too kind,
    // we'll save the un-placed host.
    std::cout << "WARNING: Couldn't place " << host->hostName() << " using "
              << _placementStrategy->strategyName().toStdString()
              << " fix your app." << std::endl;
    _unplacedHosts.push_back(host);
  }

  HostGraphicsItem *item = new HostGraphicsItem(position, host);
  _scene->addItem(item);
  _hostMap.insert(std::pair<std::string, HostGraphicsItem*>(host->hostIP(), item));

  // TODO: Scope this in a setting.
//  fitInView(_scene->itemsBoundingRect(), Qt::KeepAspectRatioByExpanding	);
  _scene->update();
}


void NetworkView::newPacket(netviz::HostSP from, netviz::PacketSP packet, netviz::HostSP to)
{
  HostGraphicsItem *fromHost = _hostMap[from->hostIP()];
  // TODO: it would be nice to have a little wave like animation radiate from the sending host
  // to give some visual indication as to what is happening.
  fromHost->animateActivityAtHost(0.0);
  HostGraphicsItem *toHost = _hostMap[to->hostIP()];
  // TODO: Base the 'toHost' delay on actual latency rather than hard coded.s
  toHost->animateActivityAtHost(500.0f);
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
