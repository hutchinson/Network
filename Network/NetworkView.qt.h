//
//  NetworkView.qt.h
//  Network
//
//  Created by Dan Hutchinson on 30/10/2014.
//
//

#ifndef Network_NetworkView_qt_h
#define Network_NetworkView_qt_h

#include <QGLWidget>

#include "Model/Host.h"

#include <iostream>
#include <memory>

#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_RIGHT 2
#define BOTTOM_LEFT 3

#define INITIAL_WORLD_WIDTH 500
#define INITIAL_WORLD_HEIGHT 500
#define INITIAL_QUADRANT_WIDTH INITIAL_WORLD_WIDTH / 2
#define INITIAL_QUADRANT_HEIGHT INITIAL_WORLD_HEIGHT / 2

#define QUAD_BOUNDS_1 64
#define QUAD_BOUNDS_2 128
#define QUAD_BOUNDS_3 192
#define QUAD_BOUNDS_4 256

namespace
{
  //////////////////////////////////////////////////////////////////////////////
  // The view is composed of (currently) 4 quadrants, each of which is composed
  // of 4 sectors and within each sector are a number of cells.

  class Cell
  {
    
  };
  
  typedef std::shared_ptr<Cell> CellPtr;
  
  //////////////////////////////////////////////////////////////////////////////
  
  class Sector
  {
  public:
    Sector(int x0, int y0, int width, int height)
    : _x0(x0), _y0(y0), _width(width), _height(height) {}

    void newHost(netviz::HostSP host)
    {
      
    }

  private:
    int _x0, _y0, _width, _height;
  };
  typedef std::shared_ptr<Sector> SectorSP;
  
  //////////////////////////////////////////////////////////////////////////////
  
  class Quadrant
  {
  public:
    Quadrant(std::string name, int x0, int y0, int width, int height)
    : _name(name), _x0(x0), _y0(y0), _width(width), _height(height)
    {
      // Create the Sectors
      int sectorWidth = _width / 2;
      int sectorHeight = _height / 2;
      _sectors[TOP_LEFT] = SectorSP( new Sector(_x0, _y0, sectorWidth, sectorHeight) );
      _sectors[TOP_RIGHT] = SectorSP( new Sector((_x0 + sectorWidth), _y0, sectorWidth, sectorHeight) );
      _sectors[BOTTOM_RIGHT] = SectorSP( new Sector((_x0 + sectorWidth), (_y0 + sectorHeight), sectorWidth, sectorHeight) );
      _sectors[BOTTOM_LEFT] = SectorSP( new Sector(x0, (_y0 + sectorHeight), sectorWidth, sectorHeight) );
    }

    void newHost(netviz::HostSP host)
    {
      // Figure out which sector to add it to based on second octet
      std::cout << "Adding " << host->hostIP() << " to quadrant " << _name << std::endl;

      drawTest.push_back(host);

      const std::string &ipStr = host->hostIP();
      std::size_t oct1Pos = ipStr.find('.');
      std::size_t oct2Pos = ipStr.find('.', oct1Pos + 1);
      std::size_t oct3Pos = ipStr.find('.', oct2Pos + 1);
      
      std::string secondOctetStr = ipStr.substr(oct1Pos + 1, oct3Pos - oct2Pos);
      int secondOctet = atoi(secondOctetStr.c_str());

      if(secondOctet >= 0 && secondOctet < QUAD_BOUNDS_1)
        _sectors[TOP_LEFT]->newHost(host);
      else if(secondOctet >= QUAD_BOUNDS_1 && secondOctet < QUAD_BOUNDS_2)
        _sectors[TOP_RIGHT]->newHost(host);
      else if(secondOctet >= QUAD_BOUNDS_2 && secondOctet < QUAD_BOUNDS_3)
        _sectors[BOTTOM_RIGHT]->newHost(host);
      else
        _sectors[BOTTOM_LEFT]->newHost(host);
    }
    
    void drawQuadrant(QPainter &painter);

  private:
    std::string _name;
    int _x0, _y0, _width, _height;

    std::vector<netviz::HostSP> drawTest;

    SectorSP _sectors[4];
  };
  
  typedef std::shared_ptr<Quadrant> QuadrantSP;

}

////////////////////////////////////////////////////////////////////////////////

class NetworkView : public QGLWidget
{
  Q_OBJECT

public:
  NetworkView(QWidget *parent = 0);
  
  void newHostAdded(netviz::HostSP host);

public slots:
  void animate();

protected:
  void resizeGL(int width, int height);
  void paintEvent(QPaintEvent* event);
  
private:
  QuadrantSP _quadrants[4];

};

#endif
