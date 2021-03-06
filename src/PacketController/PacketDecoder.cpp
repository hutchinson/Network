#include "PacketDecoder.h"
#include "ZmqQueueNames.h"
#include "IDGenerator.h"

#include "Logging/Logger.h"

#include <iostream>
#include <sstream>
#include <pcap.h>

#include "ethertype.h"

namespace netviz
{
  PacketDecoder::PacketDecoder(zmq::context_t &context)
  : _objectMutex()
  , _context(context)
  , _controlQueueId(-1)
  , _decoderReady()
  , _decoding(false)
  , _decoderThread()
  {

  }

  PacketDecoder::~PacketDecoder()
  {
    this->stopDecoding();
  }

  std::string PacketDecoder::generateControlQueueName(int controlQueueId)
  {
    std::stringstream ss;
    ss << "inproc://packet_decoder_control_queue_" << controlQueueId;
    std::string queueName = ss.str();
    return queueName;
  }

  void PacketDecoder::startDecoding()
  {
    std::lock_guard<std::mutex> lock(this->_objectMutex);
    if(this->_decoding)
      return;

    int controlQueueID = netviz::GetUniqueID();
    this->_decoderReady = std::promise<bool>();
    std::future<bool> decoderReadyFuture = this->_decoderReady.get_future();

    this->_decoderThread = \
      std::thread(&netviz::PacketDecoder::decoderThread, this, controlQueueID);

    // Wait on the listener thread future until it's booted up!
    decoderReadyFuture.wait();
    if(decoderReadyFuture.get() == true)
    {
      this->_decoding = true;
      this->_controlQueueId = controlQueueID;
    }
    else
    {
      this->_decoding = false;
      this->_controlQueueId = -1;
    }
  }

  void PacketDecoder::stopDecoding()
  {
    std::lock_guard<std::mutex> lock(this->_objectMutex);
    if(!this->_decoding)
      return;

    // Send Stop message to control queue.
    zmq::socket_t controlQueuePublisher(this->_context, ZMQ_PUB);
    std::string queueName = PacketDecoder::generateControlQueueName(this->_controlQueueId);
    controlQueuePublisher.bind(queueName.c_str());

    const char *stopCommand = "STOP";
    zmq::message_t stopMessage(5);
    memcpy(stopMessage.data(), stopCommand, (strlen(stopCommand) + 1));

    controlQueuePublisher.send(stopMessage);

    this->_decoderThread.join();
    this->_controlQueueId = -1;
  }

  void PacketDecoder::decoderThread(int controlQueueId)
  {
    zmq::socket_t newPacketQueueSubscription(this->_context, ZMQ_SUB);
    newPacketQueueSubscription.connect(netviz::NEW_PACKET_QUEUE());
    int readTimeoutMilliseconds = 200;
    newPacketQueueSubscription.setsockopt(ZMQ_RCVTIMEO, &readTimeoutMilliseconds, sizeof(int));

    zmq::socket_t modelInputQueuePublishder(this->_context, ZMQ_PUB);
    modelInputQueuePublishder.bind(netviz::MODEL_PACKET_INPUT_QUEUE());
    
    zmq::socket_t controlQueueSubscription(this->_context, ZMQ_SUB);
    std::string queueName = PacketDecoder::generateControlQueueName(controlQueueId);
    controlQueueSubscription.connect(queueName.c_str());

    // Subscribe to everything.
    newPacketQueueSubscription.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
    controlQueueSubscription.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);

    // Tell our calling thread all went well.
    this->_decoderReady.set_value(true);

    bool keepGoing = true;
    while(keepGoing)
    {
      // Check the new packet queue.
      zmq::message_t newPacketMessage;
      bool gotMessage = newPacketQueueSubscription.recv(&newPacketMessage);
      if(gotMessage)
      {
        size_t zmqMessageSize = newPacketMessage.size();
        RawPacketBuffer rawPacketBuffer(zmqMessageSize);
        rawPacketBuffer.setFromRawBuffer(newPacketMessage.data(), zmqMessageSize);

        // Process
        BasicPacketInfo basicPacketInfo;
        decodePacket(rawPacketBuffer, basicPacketInfo);
        
        // Send onto the model input queue.
        // did get packet -> then publish
        zmq::message_t decodedPacketMessage( sizeof(BasicPacketInfo) );
        memcpy(decodedPacketMessage.data(), &basicPacketInfo, sizeof(BasicPacketInfo) );
        modelInputQueuePublishder.send(decodedPacketMessage);
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
  
  void PacketDecoder::decodePacket(const RawPacketBuffer &packet, BasicPacketInfo &bpi)
  {
    memset(&bpi, 0x00, sizeof(BasicPacketInfo));

    // Only deal with ethernet packets for the minute.
    if(packet.getLinkLayerHeaderType() != DLT_EN10MB)
      return;

    // Take a look at the ethernet header.
    const EthernetHeader *ethernetHeader =\
      reinterpret_cast<const EthernetHeader*>(packet.getPacketData());
    fillPacketInfoEthernet(ethernetHeader, bpi);
    
    // For now, only continue on for IPv4
    if(bpi.ethernetFrameType != ETHERTYPE_IP)
      return;
    bpi.isIPv4 = 0xFF;

    const IPv4Header *ipv4Header =\
      reinterpret_cast<const IPv4Header*>( packet.getPacketData() + sizeof(EthernetHeader) );
    fillPacketInfoIPv4(ipv4Header, bpi);
  }
}