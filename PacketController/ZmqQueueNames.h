#ifndef __Network__ZmqQueueNames__
#define __Network__ZmqQueueNames__

////////////////////////////////////////////////////////////////////////////////
// Define global queue names here as functions, therefore they ever become
// set via a settings file the logic can be easily changed to reflect the
// change.
////////////////////////////////////////////////////////////////////////////////

const char *NEW_PACKET_QUEUE()
{
  return "inproc://new_packet_queue";  
}


#endif
