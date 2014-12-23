//
//  HostGraphicsItem.cpp
//  Network
//
//  Created by Dan Hutchinson on 17/11/2014.
//
//

#include "HostGraphicsItem.qt.h"

#include "PacketController/PacketFormat.h"

#include <QGraphicsSceneHoverEvent>
#include <QString>

#define SELECTION_HALO_WIDTH 2.0f

HostGraphicsItem::HostGraphicsItem(const QRectF &rect,  netviz::HostSP host, QGraphicsItem *parent)
: QGraphicsObject(parent)
, _host(host)
, _drawAt(rect)
, _brush(Qt::SolidPattern)
, _pen(Qt::SolidLine)
, _hostActivityAnimationGroup(NULL)
, _hasHover(false)
, _mouseHoverPosition()
{
  moveHostTo(_drawAt.center());

  setFlag(QGraphicsItem::ItemIsSelectable, true);

  setOpacity(0.0f);
  setScale(1.0f);

  setAcceptHoverEvents(true);

  // TODO: Refactor this so it's similar to the HostPlacementStrategy i.e.
  // a HostColorStrategy.
  QColor hostColor(netviz::getIPv4Octet(netviz::One, host->ip()),
                   netviz::getIPv4Octet(netviz::Two, host->ip()),
                   netviz::getIPv4Octet(netviz::Four, host->ip()),
                   190);

  _brush.setColor(hostColor);
  _pen.setColor(hostColor);

  _hostActivityAnimationGroup = new QSequentialAnimationGroup();
  
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
  painter->setOpacity(opacity());

  QPointF centre(0.0f, 0.0f);

  if(isSelected())
  {
    painter->setPen(Qt::white);
    painter->setBrush(Qt::white);
    painter->drawEllipse(centre, (CELL_WIDTH * scale() * 0.5) + SELECTION_HALO_WIDTH,
                                 (CELL_HEIGHT * scale() * 0.5) + SELECTION_HALO_WIDTH);
  }

  painter->setPen(_pen);
  painter->setBrush(_brush);
  painter->drawEllipse(centre, (CELL_WIDTH * scale() * 0.5), (CELL_HEIGHT * scale() * 0.5));

  if(_hasHover)
  {
    QPen pen;
    pen.setColor(Qt::white);
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

void HostGraphicsItem::animateActivityAtHost(qreal delay)
{
  _hostActivityAnimationGroup->stop();
  if(_hostActivityAnimationGroup->animationCount())
    _hostActivityAnimationGroup->clear();

  _hostActivityAnimationGroup->addPause(delay);
  
  QPropertyAnimation *scaleUp = new QPropertyAnimation(this, "scale");
  scaleUp->setDuration(500.0f);
  scaleUp->setStartValue(1.0f);
  scaleUp->setEndValue(1.25f);
  scaleUp->setEasingCurve(QEasingCurve::OutBounce);

  QPropertyAnimation *scaleDown = new QPropertyAnimation(this, "scale");
  scaleDown->setDuration(500.0f);
  scaleDown->setStartValue(1.25f);
  scaleDown->setEndValue(1.0f);
  scaleDown->setEasingCurve(QEasingCurve::OutBounce);
  
  _hostActivityAnimationGroup->addAnimation(scaleUp);
  _hostActivityAnimationGroup->addPause(150.0f);
  _hostActivityAnimationGroup->addAnimation(scaleDown);
  
  _hostActivityAnimationGroup->start();
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
