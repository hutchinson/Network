//
//  GLWidget.h
//  Network
//
//  Created by Dan Hutchinson on 04/10/2014.
//
//

#ifndef __Network__GLWidget__
#define __Network__GLWidget__

#include <QGLWidget>

class GLWidget : public QGLWidget
{
  Q_OBJECT
public:
  GLWidget(QWidget *parent = 0);
  
public slots:
  void animate();

protected:
  void paintEvent(QPaintEvent* event);
  void mouseMoveEvent(QMouseEvent *event);
  
private:
  QPoint _lastSeenPoint;
};

#endif
