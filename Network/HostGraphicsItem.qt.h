//
//  HostGraphicsItem.qt.h
//  Network
//
//  Created by Dan Hutchinson on 17/11/2014.
//
//

#ifndef Network_HostGraphicsItem_qt_h
#define Network_HostGraphicsItem_qt_h

#include <QObject>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QPainter>
#include <QRect>

#include "Model/Host.h"
#include "GlobalThings.h"

class HostGraphicsItem : public QGraphicsObject
{
  Q_OBJECT
  
  Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
  Q_PROPERTY(qreal scale READ scale WRITE setScale)
  
public:
  HostGraphicsItem(const QRectF &rect, netviz::HostSP host, QGraphicsItem *parent = 0);

  netviz::HostSP host() const { return _host; };
  
  QRectF boundingRect() const
  {
    QPointF topLeft(-CELL_WIDTH * 0.5, CELL_WIDTH * 0.5);
    QPointF bottomRight(CELL_WIDTH * 0.5, -CELL_WIDTH * 0.5);
    return QRectF(topLeft, bottomRight);
  }

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  const QBrush &brush() const { return _brush; }
  const QPen &pen() const { return _pen; }

  bool hasHover() const { return _hasHover; }

  void animateActivityAtHost(qreal delay);

  void moveHostTo(const QPointF &pos, bool animated = true);
  
protected:
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private:
  netviz::HostSP _host;
  QRectF _drawAt;
  QBrush _brush;
  QPen _pen;

  QSequentialAnimationGroup *_hostActivityAnimationGroup;
  
  bool _hasHover;
  QPointF _mouseHoverPosition;

  void _updateMouseHover(const QPointF &with);
};


#endif
