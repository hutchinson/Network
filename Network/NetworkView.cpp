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
{
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
  const std::string &ipString = host->hostIP();
  std::string firstOctetStr = ipString.substr(0, ipString.find('.'));

  int firstOctet = atoi(firstOctetStr.c_str());

  if(firstOctet >= 0 && firstOctet < QUAD_BOUNDS_1)
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
//  std::cout << "( " << width << ", " << height << ")" << std::endl;
}

void NetworkView::paintEvent(QPaintEvent *event)
{
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  
  // Draw background
  QBrush background( QColor(0, 0, 128) );
  painter.fillRect(event->rect(), background);
  
  // Ask each quad to draw itself
  for(int i = 0; i < 4; ++i) _quadrants[i]->drawQuadrant(painter);

  painter.end();
}

////////////////////////////////////////////////////////////////////////////////

void Quadrant::drawQuadrant(QPainter &painter)
{
  // Draw bounds of quad
  QBrush boundsBrush(QColor(255, 255, 255));
  painter.setBrush(boundsBrush);
  painter.drawRect(_x0, _y0, _width, _height);
  
  int offset = 0;
  for(std::vector<netviz::HostSP>::const_iterator it = drawTest.begin();
      it != drawTest.end();
      ++it)
  {
    painter.fillRect(_x0 + offset, _y0 + offset, 10, 10, Qt::red);
    offset += 10;
  }
}
