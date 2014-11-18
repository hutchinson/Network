//
//  Host.cpp
//  Network
//
//  Created by Dan Hutchinson on 04/10/2014.
//
//

#include "Host.h"

#include <sstream>

#include <sys/socket.h>
#include <netdb.h>

namespace netviz
{
  std::string Host::hostNameFromIP(uint32_t ip)
  {
    struct sockaddr_in socketAddress;
    memset(&socketAddress, 0x00, sizeof(struct sockaddr_in));
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = ip;
    char node[NI_MAXHOST];
    memset(node, 0, sizeof(node));

    int result = getnameinfo((struct sockaddr*)&socketAddress, sizeof(socketAddress), node, sizeof(node), NULL, 0, 0);
    if(result)
    {
      std::stringstream ss;
      ss << "<Host lookup error: " << gai_strerror(result);
      return ss.str();
    }
    return std::string(node);
  }
}