#include <zmq.hpp>
#include <iostream>

#include "PacketController/PacketDecoder.h"
#include "PacketController/PacketListener.h"
#include "Model/ModelInputQueueProcessor.h"
#include "Model/NetworkModel.h"

int main(int argc, const char * argv[])
{
  zmq::context_t zmqContext;

  netviz::NetworkModel model;

  netviz::PacketDecoder packetDecoder(zmqContext);
  netviz::PacketListener packetListner(zmqContext);
  netviz::ModelInputQueueProcessor modelInputQueueProcessor(zmqContext, model);
  
  std::cout << "PacketControllerDriver" << std::endl;

  packetListner.startListening("en1");
  packetDecoder.startDecoding();
  modelInputQueueProcessor.startProcessing();
  
  char stop;
  std::cout << "Stop listening? (y/n) \n";
  while(stop != 'y')
    std::cin >> stop;
  
  return EXIT_SUCCESS;
}

