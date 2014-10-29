//
//  main.cpp
//  Network
//
//  Created by Dan Hutchinson on 29/10/2014.
//
//

#include "GlobalThings.h"
#include "MainWindow.qt.h"

#include <QApplication>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  app.setOrganizationDomain(qApp->tr(ORG_NAME));
  app.setApplicationName(qApp->tr("Network"));
  MainWindow mainWindow;

  mainWindow.show();
  app.exec();
}