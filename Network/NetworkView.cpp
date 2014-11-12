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
, _mapView(new QuadrantBaseNetworkMapView())
, _currentWidth(width())
, _currentHeight(height())
{
  QGLWidget::setFormat(QGLFormat(QGL::SampleBuffers));
}

void NetworkView::newHostAdded(netviz::HostSP host)
{
  _mapView->newHost(host);
  repaint();
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
  QBrush background( QColor(255, 255, 255) );
  painter.fillRect(event->rect(), background);

  // Translate to center (for now, we'll add panning later)
  int windowMiddleX = _currentWidth / 2;
  int windowMiddleY = _currentHeight / 2;

  int mapMiddleX = _mapView->viewWidth() / 2;
  int mapMiddleY = _mapView->viewHeight() / 2;

  int offsetX = windowMiddleX - mapMiddleX;
  int offsetY = windowMiddleY - mapMiddleY;

  painter.translate(offsetX, offsetY);
  
  _mapView->draw(painter, _currentWidth, _currentHeight);
  
  painter.end();
}
