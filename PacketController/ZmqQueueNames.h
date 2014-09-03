#ifndef __Network__ZmqQueueNames__
#define __Network__ZmqQueueNames__

////////////////////////////////////////////////////////////////////////////////
// Define queue names here as functions, therefore they ever become set via a
// settings file the logic can be easily changed to reflect the change.
////////////////////////////////////////////////////////////////////////////////

const char *PACKET_LISTENER_CONTROL_QUEUE()
{
  return "inproc://packet_listener_control_queue";
}

const char *NEW_PACKET_QUEUE()
{
  return "inproc://new_packet_queue";  
}


#endif
