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
#include <QSettings>

#include <iostream>

MainWindow::MainWindow()
: _interfaceSelectToolBar(NULL)
, _interfaceComboBox(NULL)
, _startListeningButton(NULL)
, _stopListeningButton(NULL)
{
  // Create and setCentralWidget()
  
  _createToolBar();
  
  _readSettings();

  setUnifiedTitleAndToolBarOnMac(true);
//  showMaximized();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  std::cout << "Shutting down" << std::endl;
}

void MainWindow::_createToolBar()
{
  _interfaceSelectToolBar = addToolBar(tr("Select Interface"));

  _interfaceComboBox = new QComboBox( _interfaceSelectToolBar );
  _populateAvailableInterfaces();

  _startListeningButton = new QPushButton( tr("Start Listening"), _interfaceSelectToolBar );
  _stopListeningButton = new QPushButton( tr("Stop Listening"), _interfaceSelectToolBar );

  _interfaceSelectToolBar->addWidget(_interfaceComboBox);
  _interfaceSelectToolBar->addWidget(_startListeningButton);
  _interfaceSelectToolBar->addWidget(_stopListeningButton);
}

void MainWindow::_readSettings()
{
  QSettings settings(ORG_NAME, "Network");
  QPoint pos = settings.value("mainWindow/position", QPoint(200, 200)).toPoint();
  QSize size = settings.value("mainWindow/size", QSize(800, 800)).toSize();

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
