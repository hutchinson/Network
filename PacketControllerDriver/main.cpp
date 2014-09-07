#include <zmq.hpp>
#include <iostream>

#include "PacketController/PacketDecoder.h"
#include "PacketController/PacketListener.h"

int main(int argc, const char * argv[])
{
  zmq::context_t zmqContext;

  netviz::PacketDecoder packetDecoder(zmqContext);
  netviz::PacketListener packetListner(zmqContext);
  
  std::cout << "PacketControllerDriver" << std::endl;
  
  packetDecoder.startDecoding();
  packetListner.startListening("en1");
  
  char stop;
  std::cout << "Stop listening? (y/n) \n";
  while(stop != 'y')
    std::cin >> stop;
  
  return EXIT_SUCCESS;
}

