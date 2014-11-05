//
//  ModelInputQueueProcessor.cpp
//  Network
//
//  Created by Dan Hutchinson on 15/10/2014.
//
//

#include "ModelInputQueueProcessor.h"
#include "ZmqQueueNames.h"
#include "IDGenerator.h"

#include "PacketController/PacketFormat.h"

#include <iostream>

namespace netviz
{
  ModelInputQueueProcessor::ModelInputQueueProcessor(zmq::context_t &context, NetworkModel &model)
  : _objectMutex()
  , _context(context)
  , _controlQueueId(-1)
  , _procesorReady()
  , _processing(false)
  , _processorThread()
  , _model(model)
  {
    
  }
  
  ModelInputQueueProcessor::~ModelInputQueueProcessor()
  {
    this->stopProcessing();
  }

  std::string ModelInputQueueProcessor::generateControlQueueName(int controlQueueId)
  {
    std::stringstream ss;
    ss << "inproc://model_input_control_queue_" << controlQueueId;
    std::string queueName = ss.str();
    return queueName;
  }

  void ModelInputQueueProcessor::startProcessing()
  {
    std::lock_guard<std::mutex> lock(this->_objectMutex);
    if(this->_processing)
      return;

    int controlQueueID = netviz::GetUniqueID();
    this->_procesorReady = std::promise<bool>();
    std::future<bool> processorReadyFuture = this->_procesorReady.get_future();
    
    this->_processorThread = \
      std::thread(&netviz::ModelInputQueueProcessor::processorThread, this, controlQueueID);
    
    // Wait on the listener thread future until it's booted up!
    processorReadyFuture.wait();
    if(processorReadyFuture.get() == true)
    {
      this->_processing = true;
      this->_controlQueueId = controlQueueID;
    }
    else
    {
      this->_processing = false;
      this->_controlQueueId = -1;
    }
  }
  
  void ModelInputQueueProcessor::stopProcessing()
  {
    std::lock_guard<std::mutex> lock(this->_objectMutex);
    if(!this->_processing)
      return;
    
    // Send Stop message to control queue.
    zmq::socket_t controlQueuePublisher(this->_context, ZMQ_PUB);
    std::string queueName = ModelInputQueueProcessor::generateControlQueueName(this->_controlQueueId);
    controlQueuePublisher.bind(queueName.c_str());
    
    const char *stopCommand = "STOP";
    zmq::message_t stopMessage(5);
    memcpy(stopMessage.data(), stopCommand, (strlen(stopCommand) + 1));
    
    controlQueuePublisher.send(stopMessage);
    
    this->_processorThread.join();
    this->_controlQueueId = -1;
  }
  
  void ModelInputQueueProcessor::processorThread(int controlQueueId)
  {
    zmq::socket_t modelInputQueueSubscription(this->_context, ZMQ_SUB);
    modelInputQueueSubscription.connect(netviz::MODEL_PACKET_INPUT_QUEUE());
    int readTimeoutMilliseconds = 200;
    modelInputQueueSubscription.setsockopt(ZMQ_RCVTIMEO, &readTimeoutMilliseconds, sizeof(int));
    
    zmq::socket_t controlQueueSubscription(this->_context, ZMQ_SUB);
    std::string queueName = ModelInputQueueProcessor::generateControlQueueName(controlQueueId);
    controlQueueSubscription.connect(queueName.c_str());
    
    // Subscribe to everything.
    modelInputQueueSubscription.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
    controlQueueSubscription.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
    
    // Tell our calling thread all went well.
    this->_procesorReady.set_value(true);

    bool keepGoing = true;
    while(keepGoing)
    {
      // Check the new packet queue.
      zmq::message_t packetInfoMessage;
      bool gotMessage = modelInputQueueSubscription.recv(&packetInfoMessage);
      if(gotMessage)
      {
        BasicPacketInfo bpi;
        memcpy(&bpi, packetInfoMessage.data(), sizeof(BasicPacketInfo) );
        if(bpi.isIPv4)
        {
          HostSP sourceHost(new Host(bpi.sourceIP, bpi.sourceIPAddress));
          HostSP destHost(new Host(bpi.destinationIP, bpi.destinationIPAddress));
          
          _model.newCommunication(sourceHost, destHost);
        }
      }

      // Check zmq control queue.
      zmq::message_t controlMessage;
      gotMessage = controlQueueSubscription.recv(&controlMessage, ZMQ_DONTWAIT);
      if(gotMessage)
      {
        std::string command(reinterpret_cast<const char*>(controlMessage.data()));
        
        // did we get stop command?
        if(command == "STOP")
          keepGoing = false;
      }
    }
  }
}