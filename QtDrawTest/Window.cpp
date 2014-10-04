//
//  MainWindow.cpp
//  Network
//
//  Created by Dan Hutchinson on 04/10/2014.
//
//
#include <QTimer>
#include <QVBoxLayout>

#include "Window.qt.h"
#include "GLWidget.qt.h"

Window::Window(QWidget *parent)
: QWidget(parent)
{
  GLWidget *openGL = new GLWidget(this);
  
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(openGL);

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), openGL, SLOT(animate()) );
  timer->start(33);

  setLayout(layout);
  setObjectName("MainWindow");
  setWindowTitle(tr("2D Drawing Test"));
}