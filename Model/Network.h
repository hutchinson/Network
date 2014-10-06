//
//  Network.h
//  Network
//
//  Created by Dan Hutchinson on 04/10/2014.
//
//

#ifndef __Network__Network__
#define __Network__Network__

#include "Model/Host.h"
#include "Model/Packet.h"

namespace netviz
{
  class Network
  {
  public:
    Network(HostSP homeAdapater);
    
    void sendPacketHome(HostSP from, PacketSP packet);

    void sendPacketToHost(HostSP to, PacketSP packet);

  private:
    HostSP _home;
  };
}

#endif
