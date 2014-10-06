//
//  PacketFormat.h
//  Network
//
//  Created by Dan Hutchinson on 06/10/2014.
//
//

#ifndef Network_PacketFormat_h
#define Network_PacketFormat_h

#include <stdint.h>

const int ETHERNET_MAC_ADDRESS_LEN = 6;

struct EthernetHeader
{
  uint8_t destinationHost[ETHERNET_MAC_ADDRESS_LEN];
  uint8_t sourceHost[ETHERNET_MAC_ADDRESS_LEN];
  uint16_t etherType; // IPv4, ARP, Wake-on-LAN etc.
};



#endif
