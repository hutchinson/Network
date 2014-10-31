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
  class NewHostListenerImpl : public netviz::NewHostListener
  {
  public:
    NewHostListenerImpl(Controller *controller)
    : _controller(controller) { }

    virtual ~NewHostListenerImpl() { _controller = NULL; }

    virtual void newHostAdded(netviz::HostSP newHost)
    {
      _controller->signalNewHostAdded(newHost);
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
  // TODO: Move this out.
  qRegisterMetaType<netviz::HostSP>("netviz::HostSP");

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
  
  _newHostListner.reset(new NewHostListenerImpl(this));
  _model.reset(new netviz::NetworkModel());
  _packetListner.reset(new netviz::PacketListener(_context));
  _packetDecoder.reset(new netviz::PacketDecoder(_context));
  _modelInputQueueProcessor.reset(new netviz::ModelInputQueueProcessor(_context, *_model));

  _model->addNewHostListener(_newHostListner);
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
  _newHostListner.reset();  

  _listening = false;
  emit(listeningStatusChanged());  
}
