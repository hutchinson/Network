#include "PacketListener.h"
#include "ZmqQueueNames.h"
#include "IDGenerator.h"
#include "RawPacketBuffer.h"

#include "Logging/Logger.h"

#include <cstring>
#include <sstream>
#include <pcap/pcap.h>

namespace netviz
{
  PacketListener::PacketListener(zmq::context_t &context)
  : _objectMutex(),
    _context(context),
    _controlQueueId(-1),
    _listenerReady(),
    _listening(false),
    _listenerThread()
  { }
  
  PacketListener::~PacketListener()
  {
    this->stopListening();
  }
  
  std::string PacketListener::generateControlQueueName(int controlQueueId)
  {
    std::stringstream ss;
    ss << "inproc://packet_listener_control_queue_" << controlQueueId;
    std::string queueName = ss.str();
    return queueName;
  }
  
  void PacketListener::startListening(const std::string &deviceName)
  {
    std::lock_guard<std::mutex> lock(this->_objectMutex);
    if(this->_listening)
      return;
    
    // TODO: Generate a unique packet listener control queue name
    // Basically get an atomically incrementing integer value from a global
    // function and then append it to a string like
    // inproc://packet_listener_control_queue_{id}
    // where id == getNextUniqueControlQueueID().
    int controlQueueID = netviz::GetUniqueID();
    
    this->_listenerReady = std::promise<bool>();
    std::future<bool> listenerReadyFuture = this->_listenerReady.get_future();
    
    this->_listenerThread = \
      std::thread(&netviz::PacketListener::listenerThread, this, std::ref(deviceName), controlQueueID);
    
    // Wait on the listener thread future until it's booted up!
    listenerReadyFuture.wait();
    if(listenerReadyFuture.get() == true)
    {
      this->_listening = true;
      this->_controlQueueId = controlQueueID;
    }
    else
    {
      this->_listening = false;
      this->_controlQueueId = -1;
    }
  }
  
  void PacketListener::stopListening()
  {
    std::lock_guard<std::mutex> lock(this->_objectMutex);
    if(!this->_listening)
      return;
    
    // Send Stop message to control queue.
    zmq::socket_t controlQueuePublisher(this->_context, ZMQ_PUB);
    std::string queueName = PacketListener::generateControlQueueName(this->_controlQueueId);
    controlQueuePublisher.bind(queueName.c_str());

    const char *stopCommand = "STOP";
    zmq::message_t stopMessage(5);
    memcpy(stopMessage.data(), stopCommand, (strlen(stopCommand) + 1));
    
    controlQueuePublisher.send(stopMessage);
    
    this->_listenerThread.join();
    this->_controlQueueId = -1;
  }
  
  void PacketListener::listenerThread(const std::string &listenOn, int controlQueueId)
  {
    pcap_t *handle;
    int usePromiscuousMode = 1;
    int readTimeoutMilliseconds = 200;
    char errorBuffer[PCAP_ERRBUF_SIZE];
    
    handle = pcap_open_live(listenOn.c_str(), BUFSIZ, usePromiscuousMode, readTimeoutMilliseconds, errorBuffer);
    if(handle == NULL)
    {
      std::cerr << "Error listening on device, pcap error was: " << errorBuffer << std::endl;
      this->_listenerReady.set_value(false);
      return;
    }

    // Determine the link layer headers we'll get from this device.
    int dataLinkType = pcap_datalink(handle);

    zmq::socket_t newPacketQueuePublisher(this->_context, ZMQ_PUB);
    newPacketQueuePublisher.bind(netviz::NEW_PACKET_QUEUE());

    zmq::socket_t controlQueueSubscription(this->_context, ZMQ_SUB);
    std::string queueName = PacketListener::generateControlQueueName(controlQueueId);
    controlQueueSubscription.connect(queueName.c_str());

    // Subscribe to everything.
    controlQueueSubscription.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);

    // Tell our calling thread all went well.
    this->_listenerReady.set_value(true);

    bool keepGoing = true;
    while(keepGoing)
    {
      // Check pcap
      struct pcap_pkthdr header;
      const u_char *packetData = pcap_next(handle, &header);

      if(packetData)
      {
        // did get packet -> then publish
        size_t rawSize = sizeof(int) + sizeof(struct pcap_pkthdr) + header.caplen;
        netviz::RawPacketBuffer rawPacketBuffer(rawSize);

        rawPacketBuffer.setLinkLayerHeaderType(dataLinkType);
        rawPacketBuffer.setPcapHeader(&header);
        rawPacketBuffer.setPacketData(packetData, header.caplen);

        zmq::message_t newPacketMessage(rawPacketBuffer.getBufferSize());
        memcpy(newPacketMessage.data(), rawPacketBuffer.getBuffer(), rawPacketBuffer.getBufferSize());
        newPacketQueuePublisher.send(newPacketMessage);

//        std::stringstream ss;
//        ss << "Got message, captured " << header.caplen << " bytes" << " zmq message size to send: " << rawSize;
//        netviz::LOG_DEBUG(ss.str());
      }

      // Check zmq control queue.
      zmq::message_t controlMessage;
      bool gotMessage = controlQueueSubscription.recv(&controlMessage, ZMQ_DONTWAIT);
      if(gotMessage)
      {
        std::string command(reinterpret_cast<const char*>(controlMessage.data()));

        // did we get stop command?
        if(command == "STOP")
          keepGoing = false;
      }
    }
    pcap_close(handle);
  }
}