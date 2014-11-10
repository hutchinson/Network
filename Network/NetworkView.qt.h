//
//  NetworkView.qt.h
//  Network
//
//  Created by Dan Hutchinson on 30/10/2014.
//
//

#ifndef Network_NetworkView_qt_h
#define Network_NetworkView_qt_h

#include <QObject>
#include <QGLWidget>
#include <QBrush>
#include <QPen>
#include <QPropertyAnimation>

#include "Model/Host.h"
#include "PacketController/PacketFormat.h"

#include <iostream>
#include <memory>
#include <list>
#include <vector>

#include <assert.h>

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

class DrawState : public QObject
{
  Q_OBJECT
  
  Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
  Q_PROPERTY(qreal scale READ scale WRITE setScale)
  
public:
  DrawState(qreal opacity, qreal scale)
  : _brush(Qt::SolidPattern)
  , _pen(Qt::SolidLine)
  {
    _brush.setColor(Qt::red);
    _pen.setColor(Qt::red);

    setOpacity(0.0f);
    setScale(0.0f);
    
    _opacityAnimation = new QPropertyAnimation(this, "opacity");
    _opacityAnimation->setDuration(500);
    _opacityAnimation->setStartValue(0.0f);
    _opacityAnimation->setEndValue(1.0f);
    _opacityAnimation->setEasingCurve(QEasingCurve::OutBounce);
    _opacityAnimation->start();
    
    _scaleAnimation = new QPropertyAnimation(this, "scale");
    _scaleAnimation->setDuration(750);
    _scaleAnimation->setStartValue(0.0f);
    _scaleAnimation->setEndValue(1.0f);
    _scaleAnimation->setEasingCurve(QEasingCurve::OutBounce);
    _scaleAnimation->start();
  }
  
  void setOpacity(qreal opacity)
  {
    _opacity = opacity;
  }

  void setScale(qreal scale)
  {
    _scale = scale;
  }
  
  qreal opacity() const { return _opacity; }
  qreal scale() const { return _scale; }
  
  const QBrush &brush() const { return _brush; }
  const QPen &pen() const { return _pen; }

private:
  qreal _opacity, _scale;
  QPropertyAnimation *_opacityAnimation, *_scaleAnimation;
  QBrush _brush;
  QPen _pen;
};
typedef std::shared_ptr<DrawState> DrawStateSP;

namespace
{
  class Region;
  typedef std::shared_ptr<Region> RegionSP;
  
  class Region
  {
  public:
    Region(const std::string &name, int x0, int y0, int width, int height)
    : _name(name)
    , _x0(x0)
    , _y0(y0)
    , _width(width)
    , _height(height)
    , _subRegions() {}

    virtual void addSubRegion(const RegionSP subRegion) { _subRegions.push_back(subRegion); }

    virtual void draw(QPainter &painter)
    {
      painter.translate(_x0, _y0);
      for(std::vector<RegionSP>::iterator region = _subRegions.begin();
          region != _subRegions.end();
          ++region)
      {
        (*region)->draw(painter);
      }
    }

    bool operator<(const Region &rhs) const
    {
      if(_x0 == rhs._x0)
        return _y0 < rhs._y0;
      else
        return _x0 < rhs._x0;
    }

  protected:
    std::string _name;
    int _x0, _y0, _width, _height;
    std::vector<RegionSP> _subRegions;

  private:
    Region &operator=(const Region &rhs);
    Region(const Region &rhs);
  };

  //////////////////////////////////////////////////////////////////////////////
  
  // This class contains only one host within it.
  class HostOnlyRegion : public Region
  {
  public:
    HostOnlyRegion(const std::string &name, int x0, int y0, int width, int height, netviz::HostSP host)
    : Region(name, x0, y0, width, height)
    , _host(host)
    , _drawState(new DrawState(0.0f, 0.0f)) {}
    
    virtual void addSubRegion(const RegionSP subRegion) { assert("Cannot add Region to HostOnlyRegion"); }
  private:
    netviz::HostSP _host;
    DrawStateSP _drawState;
  };
  
  //////////////////////////////////////////////////////////////////////////////
  // The view is composed of (currently) 4 quadrants, each of which is composed
  // of 4 sectors and within each sector are a number of cells.

  class Cell
  {
  public:
    Cell(netviz::HostSP host, int column)
    : _host(host)
    , _drawState(new DrawState(0.0f, 0.0f))
    , _col(column)
    {}

    const netviz::HostSP host() const { return _host; }
    int column() const { return _col; }

    qreal opacity() const { return _drawState->opacity(); }
    qreal scale() const { return _drawState->scale(); }
    
    const QBrush &brush() const { return _drawState->brush(); }
    const QPen &pen() const { return _drawState->pen(); }
  private:
    Cell(const Cell &rhs);
    Cell &operator=(const Cell &rhs);
    
    netviz::HostSP _host;
    DrawStateSP _drawState;
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
          return;
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
