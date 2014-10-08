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
  
  struct BasicPacketInfo
  {
    uint16_t ethernetFrameType;
    char destinationMacAddress[18];
    char sourceMacAddress[18];
  };
  
  ////////////////////////////////////////////////////////////////////////////////
  // Functions
  ////////////////////////////////////////////////////////////////////////////////
  inline void fillPacketInfoIPv4(const IPv4Header *ipv4Header, BasicPacketInfo &bpi)
  {
    
  }
  
  inline void fillPacketInfoEthernet(const EthernetHeader *ethernetHeader, BasicPacketInfo &bpi)
  {
    memset(&bpi, 0, sizeof(BasicPacketInfo));

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
