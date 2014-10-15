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

namespace netviz
{
  class NetworkModel
  {
  public:
    NetworkModel();

    void addHost(HostSP host);

  private:
    NetworkModel(const NetworkModel &rhs);
    NetworkModel &operator=(const NetworkModel &rhs);

    // TODO: when netviz::Host is fleshed out we can probably just use a set
    // here.
    std::map<std::string, HostSP> _hosts;
  };
}

#endif /* defined(__Network__NetworkModel__) */
