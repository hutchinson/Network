#include <zmq.hpp>
#include <iostream>

#include "PacketController/PacketListener.h"

int main(int argc, const char * argv[])
{
  zmq::context_t zmqContext;
  netviz::PacketListener packetListner(zmqContext);
  
  std::cout << "PacketControllerDriver" << std::endl;
  
  packetListner.startListening("en1");
  
  char stop;
  std::cout << "Stop listening? (y/n) \n";
  while(stop != 'y')
    std::cin >> stop;
  
  return EXIT_SUCCESS;
}

