//
//  Grid.qt.h
//  Network
//
//  Created by Dan Hutchinson on 19/11/2014.
//
//

#ifndef Network_Grid_qt_h
#define Network_Grid_qt_h

#include <QObject>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QPainter>
#include <QRect>

class GridGraphicsItem : public QGraphicsObject
{
  Q_OBJECT
  
public:
  GridGraphicsItem(const QRectF &rect, QGraphicsItem *parent = 0);
  
  QRectF boundingRect() const
  {
    return _drawAt;
  }
  
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
  QRectF _drawAt;
};


#endif
