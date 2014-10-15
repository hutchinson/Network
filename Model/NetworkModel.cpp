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
  {
    
  }
  
  void NetworkModel::addHost(HostSP host)
  {
    if( _hosts.find(host->hostIP()) == _hosts.end() )
      _hosts[host->hostIP()] = host;
    
    std::cout << "Got message from: " << host->hostIP() << std::endl;
  }
}