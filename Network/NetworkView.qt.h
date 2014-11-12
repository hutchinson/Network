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

#include "Model/Host.h"
#include "PacketController/PacketFormat.h"
#include "DrawState.qt.h"
#include "NetworkMapView.h"

#include <iostream>
#include <memory>
#include <list>
#include <vector>

#include <assert.h>

////////////////////////////////////////////////////////////////////////////////

class NetworkView : public QGLWidget
{
  Q_OBJECT

public:
  NetworkView(QWidget *parent = 0);
  
  void newHostAdded(netviz::HostSP host);

public slots:
  void animate();

protected:
  void resizeGL(int width, int height);
  void paintEvent(QPaintEvent* event);
  
private:
  NetworkMapViewSP _mapView;
  int _currentWidth, _currentHeight;
};

#endif
