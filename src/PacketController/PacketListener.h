#ifndef __Network__PacketListener__
#define __Network__PacketListener__

#include <iostream>

#include <mutex>
#include <future>
#include <thread>
#include <memory>

#include <zmq.hpp>

namespace netviz
{
  class PacketListener
  {
  public:
    PacketListener(zmq::context_t &context);
    ~PacketListener();
    
    void startListening(const std::string &deviceName);
    void stopListening();
    
  private:
    PacketListener(const PacketListener &rhs);
    void listenerThread(const std::string &listenOn, int controlQueueId);
    
    static std::string generateControlQueueName(int controlQueueId);
    
    std::mutex _objectMutex;
    
    zmq::context_t &_context;
    int _controlQueueId;
    std::promise<bool> _listenerReady;
    bool _listening;

    std::thread _listenerThread;
  };
  typedef std::shared_ptr<PacketListener> PacketListenerSP;
}

#endif
