//
//  NetworkModel.h
//  Network
//
//  Created by Dan Hutchinson on 15/10/2014.
//
//

#ifndef __Network__NetworkModel__
#define __Network__NetworkModel__

#include "Host.h"
#include "Packet.h"

#include <string>
#include <memory>
#include <map>
#include <mutex>
#include <vector>

namespace netviz
{

  class NetworkModelDelegate
  {
  public:
    virtual ~NetworkModelDelegate() {}

    // Called each time a previously unseen host is added to the model
    virtual void newHostAdded(HostSP newHost) = 0;

    virtual void newPacket(HostSP from, PacketSP packet, HostSP to) = 0;
    
    // Called each time the overall packet statistics changes (quite freqeuently)
    // e.g. % TCP/%UDP packets.
    virtual void packetStatisticsChanged(uint64_t totalPackets, const std::vector<uint64_t> &packetTypeBreakdown) = 0;
  };
  typedef std::shared_ptr<NetworkModelDelegate> NetworkModelDelegateSP;
  
  //////////////////////////////////////////////////////////////////////////////
  
  class NetworkModel
  {
    typedef std::map<uint32_t, HostSP> HostMap;
    
  public:
    NetworkModel();

    // Methods for the PacketController to commuicate with
    void newCommunication(HostSP from, PacketSP packet, HostSP to);

    // Methods for the UI controller to commuicate with.
    void setDelegate(NetworkModelDelegateSP delegate);

  private:
    NetworkModel(const NetworkModel &rhs);
    NetworkModel &operator=(const NetworkModel &rhs);

    bool _recordHost(HostSP host);
    void _updatePacketStats(PacketSP packet);

    void _emitNewHostAdded(HostSP host)
    {
      if(_delegate)
        _delegate->newHostAdded(host);
    }

    // TODO: when netviz::Host is fleshed out we can probably just use a set
    // here.
    HostMap _hosts;
    NetworkModelDelegateSP _delegate;
    
    uint64_t _totalPackets;
    std::vector<uint64_t> _packetTypeBreakDown;

    std::mutex _objectMutex;
  };
  
  typedef std::shared_ptr<NetworkModel> NetworkModelSP;
}

#endif /* defined(__Network__NetworkModel__) */
