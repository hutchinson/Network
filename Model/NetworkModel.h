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
  class NewHostListener
  {
  public:
    virtual ~NewHostListener() {}
    virtual void newHostAdded(HostSP newHost) = 0;
  };
  typedef std::shared_ptr<NewHostListener> NewHostListenerSP;
  
  class NetworkModel
  {
  public:
    NetworkModel();

    // Methods for the PacketController to commuicate with
    void newCommunication(HostSP from, HostSP to);
    
    // Methods for the UI controller to commuicate with.
    void addNewHostListener(NewHostListenerSP listener);
    void removeNewHostListener(NewHostListenerSP listener);

  private:
    NetworkModel(const NetworkModel &rhs);
    NetworkModel &operator=(const NetworkModel &rhs);

    bool _recordHost(HostSP host);
    
    void _emitNewHostAdded(HostSP host)
    {
      for(std::vector<NewHostListenerSP>::iterator it = _newHostListeners.begin();
          it != _newHostListeners.end();
          ++it)
      {
        (*it)->newHostAdded(host);
      }
    }

    // TODO: when netviz::Host is fleshed out we can probably just use a set
    // here.
    typedef std::map<uint32_t, HostSP> HostMap;
    HostMap _hosts;

    std::vector<NewHostListenerSP> _newHostListeners;

    std::mutex _objectMutex;
  };
  
  typedef std::shared_ptr<NetworkModel> NetworkModelSP;
}

#endif /* defined(__Network__NetworkModel__) */
