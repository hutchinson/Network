//
//  PacketFormat.h
//  Network
//
//  Created by Dan Hutchinson on 06/10/2014.
//
//

#ifndef Network_PacketFormat_h
#define Network_PacketFormat_h

#include <arpa/inet.h>
#include <stdint.h>
#include <sstream>
#include <iomanip>

namespace netviz
{
  ////////////////////////////////////////////////////////////////////////////////
  // Types
  ////////////////////////////////////////////////////////////////////////////////
  const int OTHER = 0;
  const int TCP = 1;
  const int UDP = 2;
  const int ICMP = 3;
  const int NUM_PACKET_TYPES = (ICMP + 1);

  const int ETHERNET_MAC_ADDRESS_LEN = 6;

  struct EthernetHeader
  {
    uint8_t destinationHost[ETHERNET_MAC_ADDRESS_LEN];
    uint8_t sourceHost[ETHERNET_MAC_ADDRESS_LEN];
    uint16_t etherType; // IPv4, ARP, Wake-on-LAN etc.
  };

  /* IP header */
  struct IPv4Header {
    u_char ip_vhl;                          /* version << 4 | header length >> 2 */
    u_char ip_tos;                          /* type of service */
    u_short ip_len;                         /* total length */
    u_short ip_id;                          /* identification */
    u_short ip_off;                         /* fragment offset field */
#define IP_RF 0x8000                        /* reserved fragment flag */
#define IP_DF 0x4000                        /* dont fragment flag */
#define IP_MF 0x2000                        /* more fragments flag */
#define IP_OFFMASK 0x1fff                   /* mask for fragmenting bits */
    u_char ip_ttl;                          /* time to live */
    u_char ip_p;                            /* protocol */
    u_short ip_sum;                         /* checksum */
    struct in_addr ip_src,ip_dst;           /* source and dest address */
  };
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)
  
  //////////////////////////////////////////////////////////////////////////////
  
  struct BasicPacketInfo
  {
    uint16_t ethernetFrameType;
    char destinationMacAddress[18];
    char sourceMacAddress[18];

    uint8_t isIPv4;

    char destinationIPAddress[16];
    uint32_t destinationIP;
    char sourceIPAddress[16];
    uint32_t sourceIP;
    
    int ipProtocolType;
  };
  
  ////////////////////////////////////////////////////////////////////////////////
  // Functions
  ////////////////////////////////////////////////////////////////////////////////

  // Return the nth (1, 2, 3, 4) octet from the IP address.
  enum Octet { One = 0, Two = 1, Three = 2, Four = 3 };
  inline uint32_t getIPv4Octet(Octet octet, uint32_t ip)
  {
    int shiftBy = octet * 8;
    uint32_t mask = 0x000000FF << shiftBy;
    uint32_t value = (mask & ip) >> shiftBy;
    return value;
  }

  inline void fillPacketInfoIPv4(const IPv4Header *ipv4Header, BasicPacketInfo &bpi)
  {
    // Weirdly, the IP packet header's source and destination addresses are
    // already little endian, hmm should probably check this...
    in_addr_t sourceIP = ipv4Header->ip_src.s_addr; /* = ntohl(ipv4Header->ip_src.s_addr); */
    bpi.sourceIP = sourceIP;
    in_addr_t destIP = ipv4Header->ip_dst.s_addr; /* ntohl(ipv4Header->ip_dst.s_addr); */
    bpi.destinationIP = destIP;

    std::stringstream ss;
    ss << (0x000000FF & sourceIP) << "."
       << ((0x0000FF00 & sourceIP) >> 8) << "."
       << ((0x00FF0000 & sourceIP) >> 16) << "."
       << ((0xFF000000 & sourceIP) >> 24);

    std::string sourceIPStr = ss.str();
    memcpy(bpi.sourceIPAddress, sourceIPStr.c_str(), sourceIPStr.length());

    ss.clear();
    ss.str("");

    ss << (0x000000FF & destIP) << "."
    << ((0x0000FF00 & destIP) >> 8) << "."
    << ((0x00FF0000 & destIP) >> 16) << "."
    << ((0xFF000000 & destIP) >> 24);
    
    std::string destIPStr = ss.str();
    memcpy(bpi.destinationIPAddress, destIPStr.c_str(), destIPStr.length());
    
    if(ipv4Header->ip_p == 0x01)
      bpi.ipProtocolType = ICMP;
    else if(ipv4Header->ip_p == 0x06)
      bpi.ipProtocolType = TCP;
    else if(ipv4Header->ip_p == 0x11)
      bpi.ipProtocolType = UDP;
    else
      bpi.ipProtocolType = OTHER;
  }
  
  inline void fillPacketInfoEthernet(const EthernetHeader *ethernetHeader, BasicPacketInfo &bpi)
  {
    bpi.ethernetFrameType = ntohs(ethernetHeader->etherType);
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for(int i = 0; i < ETHERNET_MAC_ADDRESS_LEN; ++i)
    {
      ss << std::setw(2) << std::hex << static_cast<int>(ethernetHeader->destinationHost[i]);
      if(i != ETHERNET_MAC_ADDRESS_LEN - 1)
        ss << ":";
    }
    memcpy(bpi.destinationMacAddress, ss.str().c_str(), 17);
    
    ss.str("");
    ss.clear();
    
    ss << std::hex << std::setfill('0');
    for(int i = 0; i < ETHERNET_MAC_ADDRESS_LEN; ++i)
    {
      ss << std::setw(2) << std::hex << static_cast<int>(ethernetHeader->sourceHost[i]);
      if(i != ETHERNET_MAC_ADDRESS_LEN - 1)
        ss << ":";
    }
    memcpy(bpi.sourceMacAddress, ss.str().c_str(), 17);
  }
}
#endif
