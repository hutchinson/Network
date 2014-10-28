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

namespace netviz
{
  class NetworkModel
  {
  public:
    NetworkModel();

    void newCommunication(HostSP from, HostSP to);

  private:
    NetworkModel(const NetworkModel &rhs);
    NetworkModel &operator=(const NetworkModel &rhs);

    bool _recordHost(HostSP host);

    // TODO: when netviz::Host is fleshed out we can probably just use a set
    // here.
    typedef std::map<std::string, HostSP> HostMap;
    HostMap _hosts;
    
    std::mutex _objectMutex;
  };
}

#endif /* defined(__Network__NetworkModel__) */
