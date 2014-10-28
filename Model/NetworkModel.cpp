//
//  NetworkModel.cpp
//  Network
//
//  Created by Dan Hutchinson on 15/10/2014.
//
//

#include "NetworkModel.h"
#include <iostream>

namespace netviz {
  NetworkModel::NetworkModel()
  : _hosts()
  , _objectMutex()
  {
    
  }
  
  void NetworkModel::newCommunication(HostSP from, HostSP to)
  {
    std::lock_guard<std::mutex> lock(_objectMutex);

    if(_recordHost(from))
      std::cout << "New host: " << from->hostIP() << std::endl;
    
    if(_recordHost(to))
      std::cout << "New host: " << to->hostIP() << std::endl;
  }

  // Record the new host we've just been told about
  // Return true if we've never seen it before.
  bool NetworkModel::_recordHost(HostSP host)
  {
    HostMap::iterator foundHost = _hosts.find(host->hostIP());
    if(foundHost != _hosts.end())
      return false;
    _hosts[host->hostIP()] = host;
    return true;
  }
  
}