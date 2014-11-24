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
#include <QPainter>
#include <QRect>

#include "Model/Host.h"

#define CELL_WIDTH 25
#define CELL_HEIGHT 25

class HostGraphicsItem : public QGraphicsObject
{
  Q_OBJECT
  
  Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
  Q_PROPERTY(qreal scale READ scale WRITE setScale)
  
public:
  HostGraphicsItem(const QRectF &rect, netviz::HostSP host, QGraphicsItem *parent = 0);

  QRectF boundingRect() const
  {
    QPointF centre((CELL_WIDTH / 2), (CELL_WIDTH / 2));
    return QRectF(centre.x(), centre.y(), CELL_WIDTH, CELL_HEIGHT);
  }

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  const QBrush &brush() const { return _brush; }
  const QPen &pen() const { return _pen; }

  bool hasHover() const { return _hasHover; }

protected:
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private:
  netviz::HostSP _host;
  QRectF _drawAt;
  QBrush _brush;
  QPen _pen;

  bool _hasHover;
  QPointF _mouseHoverPosition;

  void _updateMouseHover(const QPointF &with);
};


#endif
