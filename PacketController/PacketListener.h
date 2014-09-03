#ifndef __Network__PacketListener__
#define __Network__PacketListener__

#include <mutex>
#include <thread>

#include <zmq.hpp>

namespace netviz
{
  class PacketListener
  {
  public:
    PacketListener(zmq::context_t &context);
    ~PacketListener();
    
    void startListening();
    void stopListening();
    
  private:
    PacketListener(const PacketListener &rhs);
    
    void listenerThread();
    
    zmq::context_t &_context;
    bool _listening;
    
    std::mutex _objectMutex;
    
    std::thread _listenerThread;
  };
}

#endif
