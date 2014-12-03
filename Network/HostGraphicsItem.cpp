//
//  HostGraphicsItem.cpp
//  Network
//
//  Created by Dan Hutchinson on 17/11/2014.
//
//

#include "HostGraphicsItem.qt.h"

#include <QGraphicsSceneHoverEvent>
#include <QString>

#include <iostream>

HostGraphicsItem::HostGraphicsItem(const QRectF &rect,  netviz::HostSP host, QGraphicsItem *parent)
: QGraphicsObject(parent)
, _host(host)
, _drawAt(rect)
, _brush(Qt::SolidPattern)
, _pen(Qt::SolidLine)
, _hasHover(false)
, _mouseHoverPosition()
{
  moveHostTo(_drawAt.center());
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
}

void HostGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->save();

  painter->setPen(_pen);
  painter->setBrush(_brush);
  painter->setOpacity(opacity());

  QPointF centre((CELL_WIDTH / 2), (CELL_WIDTH / 2));

  QRect ellipseBounds(centre.x(), centre.y(), CELL_WIDTH * scale(), CELL_HEIGHT * scale());
  painter->drawEllipse(ellipseBounds);

  if(_hasHover)
  {
    QColor gray(QColor(127, 127, 127, 255));
    QPen pen;
    pen.setColor(gray);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);

    QLineF line1(_mouseHoverPosition.x(), _mouseHoverPosition.y(),
                _mouseHoverPosition.x() + 20.0f, _mouseHoverPosition.y() - 20.0f);
    painter->drawLine(line1);
    QLineF line2(_mouseHoverPosition.x() + 20.0f, _mouseHoverPosition.y() - 20.0f,
                _mouseHoverPosition.x() + 100.0f, _mouseHoverPosition.y() - 20.0f);
    painter->drawLine(line2);

    QPointF textPos(_mouseHoverPosition.x() + 20.0f, _mouseHoverPosition.y() - 27.0f);
    QString hostText("%1 ( %2 )");
    hostText = hostText.arg(_host->hostIP().c_str()).arg(_host->hostName().c_str());
    painter->drawText(textPos, hostText );
  }

  painter->restore();
}

void HostGraphicsItem::moveHostTo(const QPointF &pos, bool animated)
{
  if(animated)
  {
    QPropertyAnimation *positionAnimation = new QPropertyAnimation(this, "pos");
    positionAnimation->setDuration(1000);
    positionAnimation->setStartValue(this->pos());
    positionAnimation->setEndValue(pos);
    positionAnimation->setEasingCurve(QEasingCurve::OutBounce);
    positionAnimation->start(QPropertyAnimation::DeleteWhenStopped);
  }
  else
  {
    setPos(pos);
  }
}

void HostGraphicsItem::_updateMouseHover(const QPointF &with)
{
  prepareGeometryChange();
  _mouseHoverPosition = with;
}

void HostGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
  _hasHover = true;
  _updateMouseHover(event->pos());
}

void HostGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  QGraphicsObject::hoverEnterEvent(event);
  _hasHover = true;
  _updateMouseHover(event->pos());
}

void HostGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  QGraphicsObject::hoverLeaveEvent(event);
  _hasHover = false;
}

