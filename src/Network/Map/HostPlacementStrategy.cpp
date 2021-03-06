//
//  HostPlacementStrategy.cpp
//  Network
//
//  Created by Dan Hutchinson on 03/12/2014.
//
//

#include "HostPlacementStrategy.h"

#include "GlobalThings.h"
#include "PacketController/PacketFormat.h"

#include "NetworkView.h"

#include <QApplication>

#define QUAD_BOUNDS_1 64
#define QUAD_BOUNDS_2 128
#define QUAD_BOUNDS_3 192
#define QUAD_BOUNDS_4 256

HostPlacementStrategy::~HostPlacementStrategy() {}

QuadrantPlacementStrategy::QuadrantPlacementStrategy()
: HostPlacementStrategy()
{
  
}

QuadrantPlacementStrategy::~QuadrantPlacementStrategy()
{
  
}

HostPlacementStrategySP QuadrantPlacementStrategy::create()
{
  return HostPlacementStrategySP(new QuadrantPlacementStrategy());
}

const QString QuadrantPlacementStrategy::strategyName()
{
  return qApp->tr("QuadrantPlacementStrategy");
}

void QuadrantPlacementStrategy::positionForHost(const NetworkView &theMap, const netviz::HostSP host, QRectF &where)
{
  // Determine an initial position.
  where.setX(0);
  where.setY(0);
  where.setWidth(CELL_WIDTH);
  where.setHeight(CELL_HEIGHT);
  
  QGraphicsScene *scene = theMap.scene();
  assert(scene);
  qreal width = scene->width() / 2;
  qreal height = scene->height() / 2;
  
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
  
  int finalQuad = (scene->width() / 4) / CELL_WIDTH;
  qreal row = ( (thirdOctet + fourthOctet) * firstOctet % finalQuad);
  qreal col = ( (fourthOctet + thirdOctet) * secondOctet % finalQuad);
  
  where.moveTo(where.x() + (CELL_WIDTH * row), where.y() + (CELL_HEIGHT * col));

  // Cap the max spaces we can move by
  int maxSpaces = (scene->width() / CELL_WIDTH) * (scene->width() / CELL_WIDTH);
  
  while(theMap.isSpaceOccupiedByHost(where))
  {
    // Keep shunting till we find an empty space.
    
    QPointF moveBy(0.0f, 0.0f);
    bool goAgain = false;
    do
    {
      // Pick a random number of spaces to move in the x and y direction
      int numSpacesX = rand() % maxSpaces;
      int numSpacesY = rand() % maxSpaces;

      moveBy = QPointF(numSpacesX * CELL_WIDTH, numSpacesY * CELL_HEIGHT);

      QRectF candidatePosition = where;
      candidatePosition.moveTo(where.topLeft() + moveBy);

      if((candidatePosition.bottomRight().x() > scene->width()) ||
         (candidatePosition.bottomRight().y() > scene->height()) ||
         theMap.isSpaceOccupiedByHost(candidatePosition))
        goAgain = true;
      else
        goAgain = false;

    }while( goAgain );

    where.moveTo(where.topLeft() + moveBy);
  }
}

void QuadrantPlacementStrategy::_offsetBox(QRectF &box, uint32_t octet, qreal currentXOffset, qreal currentYOffset) const
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

