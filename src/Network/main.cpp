//
//  main.cpp
//  Network
//
//  Created by Dan Hutchinson on 29/10/2014.
//
//

#include "MetaTypeExtensions.h"

#include "GlobalThings.h"
#include "MainWindow.qt.h"

#include <zmq.hpp>
#include <QApplication>

int main(int argc, char **argv)
{
  declareQtMetaTypeExtensions();

  zmq::context_t zmqContext;

  QApplication app(argc, argv);
  app.setOrganizationDomain(qApp->tr(ORG_NAME));
  app.setApplicationName(qApp->tr("Network"));

  MainWindow mainWindow(zmqContext);
  mainWindow.show();

  app.exec();
}