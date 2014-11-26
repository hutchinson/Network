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
  , _delegate()
  , _objectMutex()
  {
    
  }

  void NetworkModel::newCommunication(HostSP from, HostSP to)
  {
    std::lock_guard<std::mutex> lock(_objectMutex);

    _recordHost(from);
    _recordHost(to);
  }

  void NetworkModel::setDelegate(NetworkModelDelegateSP delegate)
  {
    std::lock_guard<std::mutex> lock(_objectMutex);
    _delegate = delegate;
  }

  // Record the new host we've just been told about
  // Return true if we've never seen it before.
  bool NetworkModel::_recordHost(HostSP host)
  {
    HostMap::iterator foundHost = _hosts.find(host->ip());
    if(foundHost != _hosts.end())
       return false;

    _hosts[host->ip()] = host;
    _emitNewHostAdded(host);
    return true;
  }
}