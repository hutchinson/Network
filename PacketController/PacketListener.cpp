#include "PacketListener.h"
#include "ZmqQueueNames.h"

namespace netviz
{
  PacketListener::PacketListener(zmq::context_t &context)
  : _context(context), _listening(false), _objectMutex(),
  _listenerThread()
  { }
  
  PacketListener::~PacketListener()
  {
    this->stopListening();
  }
  
  void PacketListener::startListening()
  {
    std::lock_guard<std::mutex> lock(this->_objectMutex);
    if(this->_listening)
      return;
    
    this->_listenerThread = std::thread(&netviz::PacketListener::listenerThread,
                                        this);
    
    // We're now listening
    this->_listening = true;
  }
  
  void PacketListener::stopListening()
  {
    std::lock_guard<std::mutex> lock(this->_objectMutex);
    if(!this->_listening)
      return;
    
  }
  
  void PacketListener::listenerThread()
  {
    // We want to subscribe to the PACKET_LISTENER_CONTROL_QUEUE and publish
    // to the NEW_PACKET_QUEUE when we get a new packet.
    //
    //
    // 1). Set up pcap listening
    // 2). SUB to control queue.
    // 3). PUB to new packet queue
    //
    // Enter event loop.
    // while(shouldStillListen)
    //    pcap_dispatch(one packet, 200ms timeout)
    //    did get packet -> then publish
    //
    //    check control queue for stop command
    //    did get stop command
    //    set shouldStillListen == false.
  }
}