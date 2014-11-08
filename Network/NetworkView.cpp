//
//  NetworkView.cpp
//  Network
//
//  Created by Dan Hutchinson on 30/10/2014.
//
//

#include "NetworkView.qt.h"

#include <QPaintEvent>
#include <QPainter>

NetworkView::NetworkView(QWidget *parent)
: QGLWidget(parent)
, _currentWidth(width())
, _currentHeight(height())
{
  QGLWidget::setFormat(QGLFormat(QGL::SampleBuffers));

  // Create the Quadrants
  _quadrants[TOP_LEFT] =
    QuadrantSP( new Quadrant("TOP_LEFT", 0, 0, INITIAL_QUADRANT_WIDTH,  INITIAL_QUADRANT_HEIGHT) );
  _quadrants[TOP_RIGHT] =
    QuadrantSP( new Quadrant("TOP_RIGHT", INITIAL_QUADRANT_WIDTH, 0, INITIAL_QUADRANT_WIDTH,  INITIAL_QUADRANT_WIDTH) );
  _quadrants[BOTTOM_RIGHT] =
    QuadrantSP( new Quadrant("BOTTOM_RIGHT", INITIAL_QUADRANT_WIDTH, INITIAL_QUADRANT_HEIGHT, INITIAL_QUADRANT_WIDTH,  INITIAL_QUADRANT_HEIGHT) );
  _quadrants[BOTTOM_LEFT] =
    QuadrantSP( new Quadrant("BOTTOM_LEFT", 0, INITIAL_QUADRANT_HEIGHT, INITIAL_QUADRANT_WIDTH,  INITIAL_QUADRANT_HEIGHT) );
}

void NetworkView::newHostAdded(netviz::HostSP host)
{
  // Figure out which quadrant to add it to
  uint32_t firstOctet = getIPv4Octet(netviz::One, host->ip());

  if(firstOctet < QUAD_BOUNDS_1)
    _quadrants[TOP_LEFT]->newHost(host);
  else if(firstOctet >= QUAD_BOUNDS_1 && firstOctet < QUAD_BOUNDS_2)
    _quadrants[TOP_RIGHT]->newHost(host);
  else if(firstOctet >= QUAD_BOUNDS_2 && firstOctet < QUAD_BOUNDS_3)
    _quadrants[BOTTOM_RIGHT]->newHost(host);
  else
    _quadrants[BOTTOM_LEFT]->newHost(host);
}

void NetworkView::animate()
{
  // Draw it.
  repaint();
}

void NetworkView::resizeGL(int width, int height)
{
  _currentWidth = width;
  _currentHeight = height;
}

void NetworkView::paintEvent(QPaintEvent *event)
{
  QPainter painter;

  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);

  // Draw background
  QBrush background( QColor(0, 0, 128) );
  painter.fillRect(event->rect(), background);
  
  // Translate to center (for now, we'll add panning later)
  int middleX = _currentWidth / 2;
  int middleY = _currentHeight / 2;
  
  int centreX = _quadrants[TOP_LEFT]->width();
  int centreY = _quadrants[TOP_LEFT]->height();
  
  int offsetX = middleX - centreX;
  int offsetY = middleY - centreY;
  
  painter.translate(offsetX, offsetY);
  
  // Ask each quad to draw itself
  for(int i = 0; i < 4; ++i)
  {
    _quadrants[i]->drawQuadrant(painter);
  }

//  painter.end();
}

////////////////////////////////////////////////////////////////////////////////

void Sector::drawSector(QPainter &painter)
{
  painter.save();

  painter.translate(_x0, _y0);
  
  for(int row = 0; row < 4; ++row)
  {
    CellColumn &columns = _cellRows[row];
    
    CellColumn::iterator column = columns.begin();
    while(column != columns.end())
    {
      CellPtr cell = *column;

      painter.setPen(cell->pen());
      painter.setBrush(cell->brush());
      painter.setOpacity(cell->opacity());

      qreal padding = 2.0f;
      qreal radius = ((CELL_HEIGHT - 4) * cell->scale()) / 2;
      QPointF centre( cell->column() * CELL_WIDTH + radius + padding, row * CELL_HEIGHT + radius + padding);
      painter.drawEllipse(centre, radius, radius);
      ++column;
    }
  }
  painter.setOpacity(1.0f);
  painter.restore();
}

void Quadrant::drawQuadrant(QPainter &painter)
{
  painter.save();
  
  // Translate relative to this quadrant
  painter.translate(_x0, _y0);

  // Draw bounds of quad
  QBrush boundsBrush(QColor(255, 255, 255));
  painter.setOpacity(0.75f);
  painter.setBrush(boundsBrush);
  painter.drawRect(0, 0, _width, _height);
  painter.setOpacity(1.0f);
  
  for(int i = 0; i < 4; ++i) _sectors[i]->drawSector(painter);

  painter.restore();
}
