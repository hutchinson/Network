//
//  NetworkStatsDock.cpp
//  Network
//
//  Created by Dan Hutchinson on 24/11/2014.
//
//

#include "NetworkStatsDock.qt.h"

NetworkStatsDock::NetworkStatsDock(QWidget *parent, Qt::WindowFlags flags)
: QDockWidget(parent, flags)
{
  setFeatures(QDockWidget::NoDockWidgetFeatures);
}
