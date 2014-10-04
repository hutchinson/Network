//
//  main.cpp
//  QtDrawTest
//
//  Created by Dan Hutchinson on 03/10/2014.
//
//

#include <QApplication>

#include "Window.qt.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  
  Window mainWindow;
  mainWindow.show();

  return app.exec();
}
