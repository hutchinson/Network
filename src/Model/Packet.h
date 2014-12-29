//
//  Packet.h
//  Network
//
//  Created by Dan Hutchinson on 04/10/2014.
//
//

#ifndef __Network__Packet__
#define __Network__Packet__

#include <memory>

namespace netviz
{
  class Packet
  {
  public:
    Packet(int packetType)
    : _packetType(packetType) {}
    
    int packetType() const { return _packetType; }

  private:
    int _packetType;
  };
  
  typedef std::shared_ptr<Packet> PacketSP;
}

#endif
