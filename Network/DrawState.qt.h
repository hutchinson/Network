//
//  DrawState.h
//  Network
//
//  Created by Dan Hutchinson on 12/11/2014.
//
//

#ifndef __Network__DrawState__
#define __Network__DrawState__

#include <QObject>
#include <QPropertyAnimation>
#include <QBrush>
#include <QPen>

#include <memory>

class DrawState : public QObject
{
  Q_OBJECT
  
  Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
  Q_PROPERTY(qreal scale READ scale WRITE setScale)
  
public:
  DrawState(qreal opacity, qreal scale)
  : _brush(Qt::SolidPattern)
  , _pen(Qt::SolidLine)
  {
    _brush.setColor(Qt::red);
    _pen.setColor(Qt::red);
    
    setOpacity(0.0f);
    setScale(0.0f);
    
    _opacityAnimation = new QPropertyAnimation(this, "opacity");
    _opacityAnimation->setDuration(500);
    _opacityAnimation->setStartValue(0.0f);
    _opacityAnimation->setEndValue(1.0f);
    _opacityAnimation->setEasingCurve(QEasingCurve::OutBounce);
    _opacityAnimation->start();
    
    _scaleAnimation = new QPropertyAnimation(this, "scale");
    _scaleAnimation->setDuration(750);
    _scaleAnimation->setStartValue(0.0f);
    _scaleAnimation->setEndValue(1.0f);
    _scaleAnimation->setEasingCurve(QEasingCurve::OutBounce);
    _scaleAnimation->start();
  }
  
  void setOpacity(qreal opacity)
  {
    _opacity = opacity;
  }
  
  void setScale(qreal scale)
  {
    _scale = scale;
  }
  
  qreal opacity() const { return _opacity; }
  qreal scale() const { return _scale; }
  
  const QBrush &brush() const { return _brush; }
  const QPen &pen() const { return _pen; }
  
private:
  qreal _opacity, _scale;
  QPropertyAnimation *_opacityAnimation, *_scaleAnimation;
  QBrush _brush;
  QPen _pen;
};
typedef std::shared_ptr<DrawState> DrawStateSP;

#endif
