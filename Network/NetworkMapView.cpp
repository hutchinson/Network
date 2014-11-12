//
//  NetworkMapView.cpp
//  Network
//
//  Created by Dan Hutchinson on 11/11/2014.
//
//

#include "NetworkMapView.h"

#include <QPainter>

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

void World::_offsetBox(Box &box, uint32_t octet, int currentXOffset, int currentYOffset) const
{
  if(octet < QUAD_BOUNDS_1)
  {
    // TOP LEFT
    box.x0 += 0;
    box.y0 += 0;
  }
  else if(octet >= QUAD_BOUNDS_1 && octet < QUAD_BOUNDS_2)
  {
    // TOP RIGHT
    box.x0 += currentXOffset; box.y0 += 0;
  }
  else if(octet >= QUAD_BOUNDS_2 && octet < QUAD_BOUNDS_3)
  {
    // BOTTOM RIGHT
    box.x0 += currentXOffset; box.y0 += currentYOffset;
  }
  else
  {
    // BOTTOM LEFT
    box.x0 += 0; box.y0 += currentYOffset;
  }
}

// This function encapsulates the QuadrantBasedNetworkMapView host placement
// logic.
void World::_determineCandidatePositionFor(const netviz::HostSP host, Box &box, int initialWidth, int initialHeight) const
{
  box.x0 = 0;
  box.y0 = 0;
  box.width = CELL_WIDTH;
  box.height = CELL_HEIGHT;

  int width = initialWidth / 2;
  int height = initialHeight / 2;

  // Determine the top level quadrant to add it to.
  uint32_t currentOctet = getIPv4Octet(netviz::One, host->ip());
  _offsetBox(box, currentOctet, width, height);
  
  // Get a little smaller and go again.
  width = width / 2;
  height = height / 2;
  currentOctet = getIPv4Octet(netviz::Two, host->ip());
  _offsetBox(box, currentOctet, width, height);
  
  int thirdOctet = netviz::getIPv4Octet(netviz::Three, host->ip());
  int fourthOctet = netviz::getIPv4Octet(netviz::Four, host->ip());
  
  int row = (thirdOctet / 4) % 4;
  int col = (fourthOctet / 4) % 4;
  
  box.x0 += CELL_WIDTH * row;
  box.y0 += CELL_HEIGHT * col;
}

void World::_scaleMapDimensionStrategy(int &width, int &height)
{
  // The idea here is we scale the two dimensions based on some scaling strategy
  // be it, doubling, squaring, fibonacci sequence.
  width *= 2;
  height *= 2;
}

void World::_resizeMap()
{
  // increase the size of the map
  // try place all the existing hosts into a new map
  // if they all fit swap them out
  // if any clash size up again.
  int candidateMapWidth = _width;
  int candidateMapHeight = _height;

  const HostMapSP existingMap = _hosts;
  bool collisionOccured = true;

  while(collisionOccured)
  {
    _scaleMapDimensionStrategy(candidateMapWidth, candidateMapHeight);
    HostMapSP candidateDistribution(new HostMap());

    for(HostMap::const_iterator it = existingMap->begin();
        it != existingMap->end();
        ++it)
    {
      const netviz::HostSP host = it->second->host();
      Box newHostPosition;
      _determineCandidatePositionFor(host, newHostPosition, candidateMapWidth, candidateMapHeight);

      // Try insert into our candidate distribution.
      HostMap::iterator hostExists = candidateDistribution->find(newHostPosition);
      if(hostExists != candidateDistribution->end())
        // Throw away this attempt and start again.
        break;

      // If we got this far, means it's not in the map so add it.
      candidateDistribution->insert(std::pair<Box, HostCellSP>(it->first, it->second));
    }

    // If we got this far it means we managed to resize the entire map with no
    // clashes.
    collisionOccured = false;
    _hosts = candidateDistribution;
    _width = candidateMapWidth;
    _height = candidateMapHeight;
  }
}

void World::addHost(netviz::HostSP host)
{
  // Figure out where we're going to put it.
  Box candidatePosition;
  std::map<Box, HostCellSP>::const_iterator it;
  bool resizeRequired = false;
  do
  {
    // Determine the cadidate position based on the current world size.
    _determineCandidatePositionFor(host, candidatePosition, _width, _height);
    // Is that position already taken?
    it = _hosts->find(candidatePosition);
    resizeRequired = it != _hosts->end();
    if(resizeRequired)
      _resizeMap();
  }while(resizeRequired);

  // Enter the host into the world at the candidate position.
  HostCellSP newHostCell(new HostCell(host));
  _hosts->insert(std::pair<Box, HostCellSP>(candidatePosition, newHostCell) );
}

void World::draw(QPainter &painter)
{
  for(HostMap::const_iterator hostCell = _hosts->begin();
      hostCell != _hosts->end();
      ++hostCell)
  {
    painter.save();
    const Box &where = hostCell->first;
    const HostCellSP what = hostCell->second;
    const DrawStateSP drawState = what->drawState();

    // Now draw.
    painter.translate(where.x0, where.y0);
    painter.setPen(drawState->pen());
    painter.setBrush(drawState->brush());
    painter.setOpacity(drawState->opacity());

    qreal padding = 2.0f;
    qreal radius = ((CELL_HEIGHT - (padding * 2)) / 2) * drawState->scale();
    QPointF centre((CELL_WIDTH / 2) - (padding * 2), (CELL_WIDTH / 2) - (padding * 2));
    painter.drawEllipse(centre, radius, radius);
    painter.restore();
  }
}

////////////////////////////////////////////////////////////////////////////////

QuadrantBaseNetworkMapView::QuadrantBaseNetworkMapView()
: NetworkMapView()
, _world(new World()) {}

QuadrantBaseNetworkMapView::~QuadrantBaseNetworkMapView() {}

void QuadrantBaseNetworkMapView::newHost(netviz::HostSP host)
{
  _world->addHost(host);
}

void QuadrantBaseNetworkMapView::draw(QPainter &painter, int viewportWidth, int viewportHieght)
{
  _world->draw(painter);
}