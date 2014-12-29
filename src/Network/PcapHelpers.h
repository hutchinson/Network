//
//  PcapHelpers.h
//  Network
//
//  Created by Dan Hutchinson on 29/10/2014.
//
//

#ifndef __Network__PcapHelpers__
#define __Network__PcapHelpers__

#include <vector>
#include <string>
#include <memory>

namespace Helpers
{

  struct NetworkInterface
  {
    NetworkInterface(const std::string &_name, const std::string &_description,
                     bool _isLoopback, bool _isUp, bool _isRunning)
    : name(_name)
    , description(_description)
    , isLoopback(_isLoopback)
    , isUp(_isUp)
    , isRunning(_isRunning){ }
    
    std::string name;
    std::string description;

    // Also grab the list of addresses.

    bool isLoopback;
    bool isUp;
    bool isRunning;
  };
  typedef std::shared_ptr<NetworkInterface> NetworkInterfaceSP;
  typedef std::vector<NetworkInterfaceSP> InterfaceList;

  void AvailableInterfaces(InterfaceList &interfaces);
}

#endif /* defined(__Network__PcapHelpers__) */
