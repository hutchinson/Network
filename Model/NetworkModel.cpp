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

    _recordHost(from);
    _recordHost(to);
  }

  void NetworkModel::addNewHostListener(NewHostListenerSP listener)
  {
    std::lock_guard<std::mutex> lock(_objectMutex);
    _newHostListeners.push_back(listener);
  }

  void NetworkModel::removeNewHostListener(NewHostListenerSP listener)
  {
    std::lock_guard<std::mutex> lock(_objectMutex);

    for(std::vector<NewHostListenerSP>::iterator it = _newHostListeners.begin();
        it != _newHostListeners.end();)
    {
      if((*it) == listener)
        it = _newHostListeners.erase(it);
      else
        ++it;
    }
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