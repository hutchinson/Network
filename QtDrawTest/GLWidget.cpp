//
//  GLWidget.cpp
//  Network
//
//  Created by Dan Hutchinson on 04/10/2014.
//
//
#include <QPaintEvent>
#include <QBrush>
#include <QPainter>

#include <iostream>

#include "GLWidget.qt.h"

GLWidget::GLWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
, _lastSeenPoint()
{
  setMouseTracking(true);
  setFixedSize(800, 600);
}

void GLWidget::animate()
{
  // Figure out what to draw
  
  // Draw it.
  repaint();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  _lastSeenPoint = event->pos();
  repaint();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  
  // Draw background
  QBrush background( QColor(0, 0, 255) );
  painter.fillRect(event->rect(), background);

//  painter.save();
//  QBrush middleBox( QColor(255, 0, 0) );
//  QSize drawSize = event->rect().size();
//  int boxSize = 50;
//  
//  painter.translate((drawSize.width() / 2) - (boxSize / 2), (drawSize.height() / 2) - (boxSize / 2));
//  painter.setOpacity(0.7);
//  painter.fillRect(QRect(0, 0, boxSize, boxSize), middleBox);
//  
//  painter.restore();

  painter.save();
  
  painter.translate( 20, 20 );
  QFont textFont;
  textFont.setPixelSize(12);
  QPen textPen(Qt::white);
  QString mousePosString = QString("Position: (%1, %2)").arg(_lastSeenPoint.x()).arg(_lastSeenPoint.y());
  
  painter.setPen(textPen);
  painter.setFont(textFont);
  painter.drawText(QRect(0, 0, 200, 50), Qt::AlignLeft, mousePosString);
  painter.restore();

  painter.end();
}