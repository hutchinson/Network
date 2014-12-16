//
//  Controller.h
//  Network
//
//  Created by Dan Hutchinson on 30/10/2014.
//
//

#ifndef __Network__Controller__
#define __Network__Controller__

#include <memory>
#include <string>

#include <QObject>
#include <QString>

#include <zmq.hpp>

#include "PacketController/PacketDecoder.h"
#include "PacketController/PacketListener.h"
#include "Model/ModelInputQueueProcessor.h"
#include "Model/NetworkModel.h"

class MainWindow;

class Controller : public QObject
{
  Q_OBJECT
  
public:
  Controller(zmq::context_t &context, MainWindow *mainWindow);
  ~Controller();

  bool isListening() const { return _listening; }

  void startListening(const std::string &interface);
  void stopListening();

  void signalPacketStatisticsChanged(uint64_t totalPackets, const std::vector<uint64_t> &packetTypeBreakdown)
  {
    emit(globalPacketStatsChanged(totalPackets, &packetTypeBreakdown[0]));
  }
  
  void signalNewHostAdded(netviz::HostSP host)
  {
    emit(newHostAdded(host));
  }

  void signalNewPacket(netviz::HostSP from, netviz::PacketSP packet, netviz::HostSP to)
  {
    emit(newPacket(from, packet, to));
  }

signals:
  void listeningStatusChanged();
  void newHostAdded(netviz::HostSP host);
  void newPacket(netviz::HostSP from, netviz::PacketSP packet, netviz::HostSP to);
  void globalPacketStatsChanged(uint64_t totalPackets, const uint64_t *packetTypeBreakdown);

private:
  Controller(const Controller &rhs);
  Controller &operator=(const Controller &rhs);

  zmq::context_t &_context;
  MainWindow *_mainWindow;
  
  bool _listening;

  netviz::NetworkModelDelegateSP _delegate;
  netviz::NetworkModelSP _model;
  
  netviz::PacketDecoderSP _packetDecoder;
  netviz::PacketListenerSP _packetListner;
  netviz::ModelInputQueueProcessorSP _modelInputQueueProcessor;
};

typedef std::shared_ptr<Controller> ControllerSP;

#endif /* defined(__Network__Controller__) */
