//
//  NetworkStatsDock.qt.h
//  Network
//
//  Created by Dan Hutchinson on 24/11/2014.
//
//

#ifndef Network_NetworkStatsDock_qt_h
#define Network_NetworkStatsDock_qt_h

#include <QDockWidget>

class NetworkStatsDock : public QDockWidget
{
  Q_OBJECT
  
public:
  NetworkStatsDock(QWidget *parent = 0, Qt::WindowFlags flags = 0);

};

#endif
