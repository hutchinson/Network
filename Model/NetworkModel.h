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

    // Called each time the overall packet statistics changes (quite freqeuently)
    // e.g. % TCP/%UDP packets.
    virtual void packetStatisticsChanged(const std::vector<double> &newStats) = 0;
  };
  typedef std::shared_ptr<NetworkModelDelegate> NetworkModelDelegateSP;
  
  //////////////////////////////////////////////////////////////////////////////
  
  class NetworkModel
  {
    typedef std::map<uint32_t, HostSP> HostMap;
    
  public:
    NetworkModel();

    // Methods for the PacketController to commuicate with
    void newCommunication(HostSP from, HostSP to);

    // Methods for the UI controller to commuicate with.
    void setDelegate(NetworkModelDelegateSP delegate);

  private:
    NetworkModel(const NetworkModel &rhs);
    NetworkModel &operator=(const NetworkModel &rhs);

    bool _recordHost(HostSP host);
    
    void _emitNewHostAdded(HostSP host)
    {
      if(_delegate)
        _delegate->newHostAdded(host);
    }

    // TODO: when netviz::Host is fleshed out we can probably just use a set
    // here.
    HostMap _hosts;
    NetworkModelDelegateSP _delegate;
    std::mutex _objectMutex;
  };
  
  typedef std::shared_ptr<NetworkModel> NetworkModelSP;
}

#endif /* defined(__Network__NetworkModel__) */
