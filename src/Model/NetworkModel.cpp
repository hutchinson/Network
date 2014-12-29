//
//  NetworkModel.cpp
//  Network
//
//  Created by Dan Hutchinson on 15/10/2014.
//
//

#include "NetworkModel.h"

#include "PacketController/PacketFormat.h"
#include <iostream>

namespace netviz {
  NetworkModel::NetworkModel()
  : _hosts()
  , _delegate()
  , _totalPackets(0)
  , _packetTypeBreakDown()
  , _objectMutex()
  {
    _packetTypeBreakDown.reserve(NUM_PACKET_TYPES);
    _packetTypeBreakDown[TCP] = 0;
    _packetTypeBreakDown[UDP] = 0;
    _packetTypeBreakDown[ICMP] = 0;
    _packetTypeBreakDown[OTHER] = 0;
  }

  void NetworkModel::newCommunication(HostSP from, PacketSP packet, HostSP to)
  {
    std::lock_guard<std::mutex> lock(_objectMutex);
    
    // Record the hosts if we haven't seen them before and tell the view we've
    // got new hosts.
    HostSP fromHost = _recordHost(from);
    fromHost->packetSent();

    HostSP toHost = _recordHost(to);
    toHost->packetReceived();
    
    // Let the view know we've got an new packet.
    if(_delegate)
      _delegate->newPacket(fromHost, packet, toHost);
    
    _updatePacketStats(packet);
  }

  void NetworkModel::setDelegate(NetworkModelDelegateSP delegate)
  {
    std::lock_guard<std::mutex> lock(_objectMutex);
    _delegate = delegate;
  }

  // Record the new host we've just been told about
  HostSP NetworkModel::_recordHost(HostSP host)
  {
    HostMap::iterator foundHost = _hosts.find(host->ip());
    if(foundHost != _hosts.end())
       return foundHost->second;

    _hosts[host->ip()] = host;
    _emitNewHostAdded(host);
    return host;
  }
  
  void NetworkModel::_updatePacketStats(PacketSP packet)
  {
    ++_totalPackets;
    _packetTypeBreakDown[packet->packetType()]++;
    
    if(_delegate)
      _delegate->packetStatisticsChanged(_totalPackets, _packetTypeBreakDown);
  }
}
