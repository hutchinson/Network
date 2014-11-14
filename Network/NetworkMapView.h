//
//  NetworkMapView.h
//  Network
//
//  Created by Dan Hutchinson on 11/11/2014.
//
//

#ifndef __Network__NetworkMapView__
#define __Network__NetworkMapView__

#include "Model/Host.h"
#include "PacketController/PacketFormat.h"
#include "DrawState.qt.h"

#include <memory>
#include <map>

#define INITIAL_WORLD_WIDTH 1000
#define INITIAL_WORLD_HEIGHT 1000

#define CELL_WIDTH 25
#define CELL_HEIGHT 25

#define QUAD_BOUNDS_1 64
#define QUAD_BOUNDS_2 128
#define QUAD_BOUNDS_3 192
#define QUAD_BOUNDS_4 256

class QPainter;

class NetworkMapView
{
public:
  virtual ~NetworkMapView() {}
  virtual void newHost(netviz::HostSP host) = 0;
  virtual int viewWidth() const = 0;
  virtual int viewHeight() const = 0;
  virtual void draw(QPainter &painter, int viewportWidth, int viewportHieght) = 0;
};

////////////////////////////////////////////////////////////////////////////////

#include <iostream>

namespace
{
  struct Box
  {
    Box() : x0(0), y0(0), width(0), height(0) {}
    Box(const Box &rhs) : x0(rhs.x0), y0(rhs.y0), width(rhs.width), height(rhs.height) {}
    
    int x0, y0, width, height;
    
    bool operator<(const Box &rhs) const
    {
      if(x0 == rhs.x0)
        return y0 < rhs.y0;
      else
        return x0 < rhs.x0;
    }
  };

  //////////////////////////////////////////////////////////////////////////////

  class HostCell
  {
  public:
    HostCell(netviz::HostSP host)
    : _host(host)
    , _drawState(new DrawState(0.0f, 0.0f))
    {}

    const netviz::HostSP host() const { return _host; }
    const DrawStateSP drawState() const { return _drawState; }
    
  private:
    HostCell(const HostCell &rhs);
    HostCell &operator=(const HostCell &rhs);

    netviz::HostSP _host;
    DrawStateSP _drawState;
  };

  typedef std::shared_ptr<HostCell> HostCellSP;
  typedef std::map<Box, HostCellSP> HostMap;
  typedef std::shared_ptr<HostMap> HostMapSP;

  //////////////////////////////////////////////////////////////////////////////
  
  class World
  {
  public:
    World(int initialWidth = INITIAL_WORLD_WIDTH, int initialHeight = INITIAL_WORLD_HEIGHT)
    : _width(initialWidth)
    , _height(initialHeight)
    , _hosts(new HostMap())
    {}

    // Adds the new host to the world, potentially scaling the world up if
    // we've run out of room.
    void addHost(netviz::HostSP host);

    int width() const { return _width; }
    int height() const { return _height; }
    
    void draw(QPainter &painter);

  private:
    int _width, _height;
    HostMapSP _hosts;

    void _determineCandidatePositionFor(const netviz::HostSP host, Box &box, int initialWidth, int initialHeight) const;
    void _offsetBox(Box &box, uint32_t octet, int currentXOffset, int currentYOffset) const;
    void _resizeMap();
    
    void _scaleMapDimensionStrategy(int &width, int &height);
  };
  typedef std::shared_ptr<World> WorldSP;
}

////////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<NetworkMapView> NetworkMapViewSP;
class QuadrantBaseNetworkMapView : public NetworkMapView
{
public:
  QuadrantBaseNetworkMapView();
  virtual ~QuadrantBaseNetworkMapView();
  virtual void newHost(netviz::HostSP host);
  virtual void draw(QPainter &painter, int viewportWidth, int viewportHieght);
  virtual int viewWidth() const { return _world->width(); }
  virtual int viewHeight() const { return _world->height(); }
private:
  WorldSP _world;
};

#endif
