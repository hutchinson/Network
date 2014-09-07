#include "ZmqQueueNames.h"

namespace netviz
{
  const char *NEW_PACKET_QUEUE()
  {
    return "inproc://new_packet_queue";
  }
}