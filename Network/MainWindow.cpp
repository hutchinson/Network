//
//  MainWindow.cpp
//  Network
//
//  Created by Dan Hutchinson on 29/10/2014.
//
//

#include "MainWindow.qt.h"
#include "PcapHelpers.h"
#include "GlobalThings.h"

#include <QToolBar>
#include <QComboBox>
#include <QPushButton>
#include <QAction>
#include <QSettings>

#include <iostream>

MainWindow::MainWindow()
: _startListeningAction(NULL)
, _stopListeningAction(NULL)
, _interfaceSelectToolBar(NULL)
, _interfaceComboBox(NULL)
, _startListeningButton(NULL)
, _stopListeningButton(NULL)
{
  // Create and setCentralWidget()
  _createActions();
  _createToolBar();
  
  _readSettings();

  setUnifiedTitleAndToolBarOnMac(true);
//  showMaximized();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  std::cout << "Shutting down" << std::endl;
}

void MainWindow::startListening()
{
  std::cout << "Starting Listening" << std::endl;
}

void MainWindow::stopListening()
{
  std::cout << "Stopping Listening" << std::endl;
}

void MainWindow::_createActions()
{
  _startListeningAction = new QAction(tr("Start Listening"), this);
  connect(_startListeningAction, SIGNAL(triggered()), this, SLOT(startListening()));
  
  _stopListeningAction = new QAction(tr("Stop Listening"), this);
  connect(_stopListeningAction, SIGNAL(triggered()), this, SLOT(stopListening()));
}

void MainWindow::_createToolBar()
{
  _interfaceSelectToolBar = addToolBar(tr("Select Interface"));

  _interfaceComboBox = new QComboBox( _interfaceSelectToolBar );
  _populateAvailableInterfaces();

  _startListeningButton = new QPushButton( tr("Start Listening"), _interfaceSelectToolBar );
  _startListeningButton->addAction(_startListeningAction);
  connect(_startListeningButton, SIGNAL(released()), _startListeningAction, SLOT(trigger()));
  
  _stopListeningButton = new QPushButton( tr("Stop Listening"), _interfaceSelectToolBar );
  _stopListeningButton->addAction(_stopListeningAction);
  connect(_stopListeningButton, SIGNAL(released()), _stopListeningAction, SLOT(trigger()));
  
  _interfaceSelectToolBar->addWidget(_interfaceComboBox);
  _interfaceSelectToolBar->addWidget(_startListeningButton);
  _interfaceSelectToolBar->addWidget(_stopListeningButton);
}

void MainWindow::_readSettings()
{
  QSettings settings(ORG_NAME, "Network");
  QPoint pos = settings.value("mainWindow/position", QPoint(200, 0)).toPoint();
  QSize size = settings.value("mainWindow/size", QSize(800, 750)).toSize();

  resize(size);
  move(pos);
}

void MainWindow::_populateAvailableInterfaces()
{
  Helpers::InterfaceList interfaces;
  Helpers::AvailableInterfaces(interfaces);

  for(Helpers::InterfaceList::iterator it = interfaces.begin();
      it != interfaces.end();
      ++it)
  {
    _interfaceComboBox->addItem( (*it)->name.c_str() );
  }
}
