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

#define CELL_WIDTH 25
#define CELL_HEIGHT 25

class HostGraphicsItem : public QGraphicsObject
{
  Q_OBJECT
  
  Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
  Q_PROPERTY(qreal scale READ scale WRITE setScale)
  
public:
  HostGraphicsItem(const QRectF &rect, QGraphicsItem *parent = 0)
  : QGraphicsObject(parent)
  , _drawAt(rect)
  , _brush(Qt::SolidPattern)
  , _pen(Qt::SolidLine)
  {
    _brush.setColor(Qt::red);
    _pen.setColor(Qt::red);
    
    setOpacity(0.0f);
    setScale(1.0f);
    
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
  
  QRectF boundingRect() const
  {
    return QRectF(0, 0, CELL_WIDTH, CELL_HEIGHT);
  }
  
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    painter->save();
    painter->setPen(_pen);
    painter->setBrush(_brush);
    painter->setOpacity(opacity());
    
    painter->translate(_drawAt.center());
    
    qreal padding = 2.0f;
    QPointF centre((CELL_WIDTH / 2) - (padding * 2), (CELL_WIDTH / 2) - (padding * 2));
    
    QRect ellipseBounds(0, 0, CELL_WIDTH * scale(), CELL_HEIGHT * scale());
    
    painter->drawEllipse(ellipseBounds);
    painter->restore();
  }
  
  const QBrush &brush() const { return _brush; }
  const QPen &pen() const { return _pen; }
  
private:
  QRectF _drawAt;
  QBrush _brush;
  QPen _pen;
};


#endif
