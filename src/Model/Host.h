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

#include <time.h>

namespace netviz
{
  class Host
  {
  public:
    Host(uint32_t ip, const std::string &hostIP)
    : _ip(ip)
    , _ipString(hostIP)
    , _hostName(Host::hostNameFromIP(ip))
    , _packetsSent(0)
    , _packetsReceived(0)
    , _firstSeen(0)
    {
      time(&_firstSeen);
    }

    uint32_t ip() const { return _ip; }
    const std::string &hostIP() const { return _ipString; }

    const std::string &hostName() const { return _hostName; }

    void packetSent() { ++_packetsSent; }
    void packetReceived() { ++_packetsReceived; }

    const uint64_t packetsSent() const { return _packetsSent; }
    const uint64_t packetsReceived() const { return _packetsReceived; }

    const time_t firstSeen() const { return _firstSeen; }

  public:
    // TODO: This method is NOT threadsafe
    static std::string hostNameFromIP(uint32_t ip);

  private:
    uint32_t _ip;
    std::string _ipString;
    std::string _hostName;

    uint64_t _packetsSent;
    uint64_t _packetsReceived;
    
    time_t _firstSeen;
  };

  typedef std::shared_ptr<Host> HostSP;
}

#endif
