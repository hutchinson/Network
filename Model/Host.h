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
    Host(uint32_t ip, const std::string &hostIP)
    : _ip(ip)
    , _ipString(hostIP)
    , _hostName(Host::hostNameFromIP(ip))
    {}

    uint32_t ip() const { return _ip; }
    const std::string &hostIP() const { return _ipString; }

    const std::string &hostName() const { return _hostName; }

    // TODO: This method is NOT threadsafe
    static std::string hostNameFromIP(uint32_t ip);

  private:
    uint32_t _ip;
    std::string _ipString;
    std::string _hostName;
  };
  
  typedef std::shared_ptr<Host> HostSP;
}

#endif
