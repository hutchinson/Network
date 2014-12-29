#include "ZmqQueueNames.h"

namespace netviz
{
  const char *NEW_PACKET_QUEUE()
  {
    return "inproc://new_packet_queue";
  }

  const char *MODEL_PACKET_INPUT_QUEUE()
  {
    return "inproc://model_packet_input_queue";   
  }
}