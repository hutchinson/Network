//
//  Network.cpp
//  Network
//
//  Created by Dan Hutchinson on 04/10/2014.
//
//

#include "Network.h"

namespace netviz
{
  Network::Network(HostSP homeAdapater)
  : _home(homeAdapater)
  {
    
  }
  
  void Network::sendPacketHome(HostSP from, PacketSP packet)
  {
    
  }
  
  void Network::sendPacketToHost(HostSP to, PacketSP packet)
  {
    
  }
}