//
//  MainWindow.qt.h
//  Network
//
//  Created by Dan Hutchinson on 29/10/2014.
//
//

#ifndef Network_MainWindow_qt_h
#define Network_MainWindow_qt_h

#include "Controller.qt.h"

#include <zmq.hpp>

#include <QMainWindow>

class QAction;
class QComboBox;
class QPushButton;

class NetworkView;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(zmq::context_t &context);

public slots:
  void listeningStatusChanged();
  void hostAdded(netviz::HostSP newHost);
  
protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void startListening();
  void stopListening();
  
private:
  void _createActions();
  void _createToolBar();
  void _createNetworkView();
  void _readSettings();

  void _populateAvailableInterfaces();
  
  QAction *_startListeningAction;
  QAction *_stopListeningAction;

  QToolBar *_interfaceSelectToolBar;
  QComboBox *_interfaceComboBox;
  QPushButton *_startListeningButton;
  QPushButton *_stopListeningButton;

  ControllerSP _controller;

  NetworkView *_networkView;
  
  zmq::context_t &_zmqContext;
  
  // Temporary
  int _hostsSeen;
  netviz::HostSP _lastHost;
};


#endif
