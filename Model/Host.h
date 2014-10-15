//
//  Host.h
//  Network
//
//  Created by Dan Hutchinson on 04/10/2014.
//
//

#ifndef __Network__Host__
#define __Network__Host__

#include <memory>
#include <string>

namespace netviz
{
  class Host
  {
  public:
    Host(const std::string &hostIP)
    : _ipString(hostIP) {}

    const std::string &hostIP() const { return _ipString; }

  private:
    std::string _ipString;
  };
  
  typedef std::shared_ptr<Host> HostSP;
}

#endif
