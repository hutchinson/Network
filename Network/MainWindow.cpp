//
//  MainWindow.cpp
//  Network
//
//  Created by Dan Hutchinson on 29/10/2014.
//
//

#include "MainWindow.qt.h"
#include "NetworkView.qt.h"

#include "PcapHelpers.h"
#include "GlobalThings.h"

#include <QToolBar>
#include <QComboBox>
#include <QPushButton>
#include <QAction>
#include <QSettings>
#include <QStatusBar>
#include <QTimer>

#include <iostream>

MainWindow::MainWindow(zmq::context_t &context)
: _startListeningAction(NULL)
, _stopListeningAction(NULL)
, _interfaceSelectToolBar(NULL)
, _interfaceComboBox(NULL)
, _startListeningButton(NULL)
, _stopListeningButton(NULL)
, _controller()
, _networkView(NULL)
, _zmqContext(context)
, _hostsSeen(0)
{
  // Create and setCentralWidget()
  _createActions();
  _createToolBar();
  _createNetworkView();

  _readSettings();

  setCentralWidget(_networkView);

  _controller = ControllerSP(new Controller(_zmqContext, this));
  // This will boot the status bar - it's also the signal the controller will call
  // whenever the listening status changes.
  listeningStatusChanged();

  setUnifiedTitleAndToolBarOnMac(true);
//  showMaximized();

  // TODO: Animation timer required anymore?
//  QTimer *_animationTimer = new QTimer(this);
//  connect(_animationTimer, SIGNAL(timeout()), _networkView, SLOT(animate()));
//  _animationTimer->start( FPS_MILLISECONS );
}

void MainWindow::listeningStatusChanged()
{
  bool isListening = _controller->isListening();
  if(isListening)
  {
    QString message(tr("Listening to network"));
    message += tr(" | ");
    message += tr("%1 unique hosts").arg(_hostsSeen);
    if(_lastHost)
    {
      message += tr(" | ");
      message += tr("Newest host %1").arg(_lastHost->hostIP().c_str());
    }

    statusBar()->showMessage(message);
  }
  else
  {
    statusBar()->showMessage(tr("Not Listening."));
  }
}

void MainWindow::hostAdded(netviz::HostSP newHost)
{
  // Add to a quadrant in the view!
  _networkView->newHostAdded(newHost);

  ++_hostsSeen;
  _lastHost = newHost;
  listeningStatusChanged();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  _controller->stopListening();
//  _animationTimer->stop();
}

void MainWindow::startListening()
{
  std::string interface = _interfaceComboBox->currentText().toStdString();
  _controller->startListening(interface);
}

void MainWindow::stopListening()
{
  _controller->stopListening();
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
  connect(_startListeningButton, SIGNAL(released()), _startListeningAction, SLOT(trigger()));
  
  _stopListeningButton = new QPushButton( tr("Stop Listening"), _interfaceSelectToolBar );
  connect(_stopListeningButton, SIGNAL(released()), _stopListeningAction, SLOT(trigger()));
  
  _interfaceSelectToolBar->addWidget(_interfaceComboBox);
  _interfaceSelectToolBar->addWidget(_startListeningButton);
  _interfaceSelectToolBar->addWidget(_stopListeningButton);
}

void MainWindow::_createNetworkView()
{
  _networkView = new NetworkView(this);
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
