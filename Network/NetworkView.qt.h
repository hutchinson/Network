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
#include "PacketController/PacketFormat.h"

#include <iostream>
#include <memory>
#include <list>
#include <vector>

#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_RIGHT 2
#define BOTTOM_LEFT 3

#define INITIAL_WORLD_WIDTH 500
#define INITIAL_WORLD_HEIGHT 500
#define INITIAL_QUADRANT_WIDTH INITIAL_WORLD_WIDTH / 2
#define INITIAL_QUADRANT_HEIGHT INITIAL_WORLD_HEIGHT / 2

////////////////////////////////////////////////////////////////////////////////
// Cell width is constant (won't resize even if world needs to grow.
////////////////////////////////////////////////////////////////////////////////
#define CELL_WIDTH INITIAL_QUADRANT_WIDTH / 10
#define CELL_HEIGHT INITIAL_QUADRANT_WIDTH / 10

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
  public:
    Cell(netviz::HostSP host, int column)
    : _host(host)
    , _col(column) {}

    const netviz::HostSP host() const { return _host; }
    int column() const { return _col; }

  private:
    netviz::HostSP _host;
    int _col;
  };
  
  typedef std::shared_ptr<Cell> CellPtr;
  typedef std::list<CellPtr> CellColumn;
  typedef std::vector<CellColumn> CellRow;
  
  //////////////////////////////////////////////////////////////////////////////
  
  class Sector
  {
  public:
    Sector(int x0, int y0, int width, int height)
    : _x0(x0)
    , _y0(y0)
    , _width(width)
    , _height(height)
    , _cellRows()
    {
      // A given sector will hold 1/4 of the quadrants available cells.
      for(int i = 0; i < 4; ++i)
        _cellRows.push_back(CellColumn());
    }

    void newHost(netviz::HostSP host)
    {
      // Need to determine a row and column that this new host is going to live
      // in. 3rd octet will give us this information.
      int thirdOctet = netviz::getIPv4Octet(netviz::Three, host->ip());
      int fourthOctet = netviz::getIPv4Octet(netviz::Four, host->ip());

      int row = (thirdOctet / 4) % 4;
      int col = (fourthOctet / 4) % 4;

      CellColumn &columns = _cellRows[row];
      for(CellColumn::iterator it = columns.begin();
          it != columns.end();
          ++it)
      {
        if((*it)->column() == col)
        {
          std::cout << "CLASH Error adding " << host->hostIP() << " " << (*it)->host()->hostIP() << " already stored here." << std::endl;
          break;
        }
      }
      
      CellPtr newCell( new Cell(host, col) );

      // Find the correct place to insert the new cell.
      // TODO: All this better...
      columns.insert(columns.end(), newCell);
    }

    void drawSector(QPainter &painter);

  private:
    int _x0, _y0, _width, _height;
    CellRow _cellRows;
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
      _sectors[TOP_LEFT] = SectorSP( new Sector(0, 0, sectorWidth, sectorHeight) );
      _sectors[TOP_RIGHT] = SectorSP( new Sector(sectorWidth, 0, sectorWidth, sectorHeight) );
      _sectors[BOTTOM_RIGHT] = SectorSP( new Sector(sectorWidth, sectorHeight, sectorWidth, sectorHeight) );
      _sectors[BOTTOM_LEFT] = SectorSP( new Sector(0, sectorHeight, sectorWidth, sectorHeight) );
    }

    void newHost(netviz::HostSP host)
    {
      uint32_t secondOctet = getIPv4Octet(netviz::Two, host->ip());
      // Figure out which sector to add it to based on second octet
      std::cout << "Adding " << host->hostIP() << " to quadrant " << _name << std::endl;

      if(secondOctet < QUAD_BOUNDS_1)
        _sectors[TOP_LEFT]->newHost(host);
      else if(secondOctet >= QUAD_BOUNDS_1 && secondOctet < QUAD_BOUNDS_2)
        _sectors[TOP_RIGHT]->newHost(host);
      else if(secondOctet >= QUAD_BOUNDS_2 && secondOctet < QUAD_BOUNDS_3)
        _sectors[BOTTOM_RIGHT]->newHost(host);
      else
        _sectors[BOTTOM_LEFT]->newHost(host);
    }
    
    void drawQuadrant(QPainter &painter);
    
    int width() const { return _width; }
    int height() const { return _height; }

  private:
    std::string _name;
    int _x0, _y0, _width, _height;

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
  
  int _currentWidth, _currentHeight;

};

#endif
