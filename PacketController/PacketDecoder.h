#ifndef __Network__PacketDecoder__
#define __Network__PacketDecoder__

#include <string>

#include <mutex>
#include <future>
#include <thread>

#include <zmq.hpp>

#include "RawPacketBuffer.h"

namespace netviz
{
  class PacketDecoder
  {
  public:
    PacketDecoder(zmq::context_t &context);
    ~PacketDecoder();

    void startDecoding();
    void stopDecoding();

  private:
    PacketDecoder(const PacketDecoder &rhs);
    const PacketDecoder &operator=(const PacketDecoder &rhs);

    static std::string generateControlQueueName(int controlQueueId);

    void decoderThread(int controlQueueId);
    
    void decodePacket(const RawPacketBuffer &packet /*, zmq input queue to send to */);

    std::mutex _objectMutex;

    zmq::context_t &_context;
    int _controlQueueId;

    std::promise<bool> _decoderReady;
    bool _decoding;
    std::thread _decoderThread;
  };
}

#endif
