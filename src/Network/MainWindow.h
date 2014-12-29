//
//  MainWindow.qt.h
//  Network
//
//  Created by Dan Hutchinson on 29/10/2014.
//
//

#ifndef Network_MainWindow_qt_h
#define Network_MainWindow_qt_h

#include "Controller.h"

#include "DrawOptionsDock.h"
#include "NetworkStatsDock.h"

#include <zmq.hpp>

#include <QMainWindow>

#define ANIMATION_FPS 30
#define FPS_MILLISECONS 1000 / ANIMATION_FPS

class QAction;
class QComboBox;
class QPushButton;
class QTimer;

class NetworkView;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(zmq::context_t &context);
  
public slots:
  void listeningStatusChanged();
  void hostAdded(netviz::HostSP newHost);
  void newPacket(netviz::HostSP from, netviz::PacketSP packet, netviz::HostSP to);
  void globalPacketStatsChanged(uint64_t totalPackets, const uint64_t* packetTypeBreakdown);
  void selectedItemChanged();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void startListening();
  void stopListening();
  
private:
  void _createActions();
  void _createToolBar();
  void _createDrawOptionsBar();
  void _createNetworkStatsBar();
  void _createNetworkView();
  void _readSettings();

  void _populateAvailableInterfaces();
  
  QAction *_startListeningAction;
  QAction *_stopListeningAction;

  QToolBar *_interfaceSelectToolBar;
  QComboBox *_interfaceComboBox;
  QPushButton *_startListeningButton;
  QPushButton *_stopListeningButton;
  
  DrawOptionsDock *_drawOptionsDock;
  NetworkStatsDock *_networkStatsDock;

  ControllerSP _controller;

  NetworkView *_networkView;
  
  zmq::context_t &_zmqContext;
  
  // Temporary
  int _hostsSeen;
  netviz::HostSP _lastHost;
};


#endif
