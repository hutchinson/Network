//
//  HostGraphicsItem.cpp
//  Network
//
//  Created by Dan Hutchinson on 17/11/2014.
//
//

#include "HostGraphicsItem.qt.h"

#include <iostream>

HostGraphicsItem::HostGraphicsItem(const QRectF &rect,  netviz::HostSP host, QGraphicsItem *parent)
: QGraphicsObject(parent)
, _host(host)
, _drawAt(rect)
, _brush(Qt::SolidPattern)
, _pen(Qt::SolidLine)
{
  setPos(_drawAt.center());
  setOpacity(0.0f);
  setScale(1.0f);

  setAcceptHoverEvents(true);

  _brush.setColor(Qt::red);
  _pen.setColor(Qt::red);
  
  QPropertyAnimation *_opacityAnimation = new QPropertyAnimation(this, "opacity");
  _opacityAnimation->setDuration(500);
  _opacityAnimation->setStartValue(0.0f);
  _opacityAnimation->setEndValue(1.0f);
  _opacityAnimation->setEasingCurve(QEasingCurve::OutBounce);
  _opacityAnimation->start(QPropertyAnimation::DeleteWhenStopped);
  
  //    QPropertyAnimation *_scaleAnimation = new QPropertyAnimation(this, "scale");
  //    _scaleAnimation->setDuration(500);
  //    _scaleAnimation->setStartValue(0.0f);
  //    _scaleAnimation->setEndValue(1.0f);
  //    _scaleAnimation->setEasingCurve(QEasingCurve::OutBounce);
  //    _scaleAnimation->start(QPropertyAnimation::DeleteWhenStopped);
}

void HostGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->save();
  painter->setPen(_pen);
  painter->setBrush(_brush);
  painter->setOpacity(opacity());
  
  qreal padding = 2.0f;
  QPointF centre((CELL_WIDTH / 2) - (padding * 2), (CELL_WIDTH / 2) - (padding * 2));
  
  QRect ellipseBounds(0, 0, CELL_WIDTH * scale(), CELL_HEIGHT * scale());
  
  painter->drawEllipse(ellipseBounds);
  painter->restore();
}

bool HostGraphicsItem::sceneEvent(QEvent *event)
{
//  std::cout << "Got event" << std::endl;
  return QGraphicsObject::sceneEvent(event);
}

void HostGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  std::cout << "Entered " << _host->hostIP() << std::endl;
  QGraphicsObject::hoverEnterEvent(event);
}

void HostGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  std::cout << "Left " << _host->hostIP() << std::endl;
  QGraphicsObject::hoverLeaveEvent(event);
}

