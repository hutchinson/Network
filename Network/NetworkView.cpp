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
  
  QBrush backgroundBrush( QColor(128, 128, 128, 128) );
  setBackgroundBrush(backgroundBrush);

  // Set up the grid.
  _grid = new GridGraphicsItem( QRect(0, 0, INITIAL_WORLD_WIDTH, INITIAL_WORLD_HEIGHT));
  _scene->addItem(_grid);

  show();
}

void NetworkView::newHostAdded(netviz::HostSP host)
{
  QRect position;
  _determineCandidatePositionFor(host, position, _scene->width(), _scene->height());
  HostGraphicsItem *item = new HostGraphicsItem(position, host);
  
  // Check if there is already a host within this host position?
  QRect exclusionSpace(position);
  exclusionSpace.adjust(-25, -25, 25, 25);
  QGraphicsItem *existingItem = _scene->itemAt(position.center(), QTransform());
  if(existingItem)
  {
    std::cout << host->hostName() << " clash!" << std::endl;
  }
  
  _scene->addItem(item);
  _hostMap.insert(std::pair<QGraphicsItem*, netviz::HostSP>(item, host));
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
  
  qreal row = (thirdOctet / 4) % 4;
  qreal col = (fourthOctet / 4) % 4;

  where.moveTo(where.x() + (CELL_WIDTH * row), where.y() + (CELL_HEIGHT * col));
}

