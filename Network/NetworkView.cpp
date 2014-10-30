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
  
}

void NetworkView::animate()
{
  // Draw it.
  repaint();
}

void NetworkView::paintEvent(QPaintEvent *event)
{
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  
  // Draw background
  QBrush background( QColor(0, 0, 128) );
  painter.fillRect(event->rect(), background);
  painter.end();
}