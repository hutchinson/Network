//
//  Controller.cpp
//  Network
//
//  Created by Dan Hutchinson on 30/10/2014.
//
//

#include "Controller.qt.h"
#include "MainWindow.qt.h"

namespace {
  class NetworkModelDelegateImpl : public netviz::NetworkModelDelegate
  {
  public:
    NetworkModelDelegateImpl(Controller *controller)
    : _controller(controller) { }

    virtual ~NetworkModelDelegateImpl() { _controller = NULL; }

    virtual void newHostAdded(netviz::HostSP newHost)
    {
      _controller->signalNewHostAdded(newHost);
    }
    
    virtual void packetStatisticsChanged(const std::vector<double> &newStats)
    {
      
    }

  private:
    Controller *_controller;
  };
}

Controller::Controller(zmq::context_t &context, MainWindow *mainWindow)
: QObject()
, _context(context)
, _mainWindow(mainWindow)
, _listening(false)
, _packetDecoder()
, _packetListner()
, _modelInputQueueProcessor()
{
  connect(this, SIGNAL(listeningStatusChanged()), _mainWindow, SLOT(listeningStatusChanged()));
  connect(this, SIGNAL(newHostAdded(netviz::HostSP)), _mainWindow, SLOT(hostAdded(netviz::HostSP)));
}

Controller::~Controller()
{
  if(isListening())
    stopListening();
}

void Controller::startListening(const std::string &interface)
{
  if(isListening())
    stopListening();
  
  _delegate.reset(new NetworkModelDelegateImpl(this));
  _model.reset(new netviz::NetworkModel());
  _packetListner.reset(new netviz::PacketListener(_context));
  _packetDecoder.reset(new netviz::PacketDecoder(_context));
  _modelInputQueueProcessor.reset(new netviz::ModelInputQueueProcessor(_context, *_model));

  _model->setDelegate(_delegate);
  _packetListner->startListening(interface);
  _packetDecoder->startDecoding();
  _modelInputQueueProcessor->startProcessing();
  
  _listening = true;
  emit(listeningStatusChanged());
}

void Controller::stopListening()
{
  _modelInputQueueProcessor.reset();
  _packetDecoder.reset();
  _packetListner.reset();
  _model.reset();
  _delegate.reset();

  _listening = false;
  emit(listeningStatusChanged());  
}
