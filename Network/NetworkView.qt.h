//
//  NetworkView.qt.h
//  Network
//
//  Created by Dan Hutchinson on 30/10/2014.
//
//

#ifndef Network_NetworkView_qt_h
#define Network_NetworkView_qt_h

#include <QGLWidget>

class NetworkView : public QGLWidget
{
  Q_OBJECT

public:
  NetworkView(QWidget *parent = 0);

public slots:
  void animate();

protected:
  void paintEvent(QPaintEvent* event);
  
};

#endif
