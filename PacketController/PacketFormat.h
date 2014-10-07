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

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
inline void printEthernetHeader(const EthernetHeader *ethernetHeader, std::stringstream &ss)
{
  ss << "EtherType: 0x" << std::hex << static_cast<int>( ntohs(ethernetHeader->etherType) );
  
  ss << " | Source: ";
  for(int i = 0; i < ETHERNET_MAC_ADDRESS_LEN; ++i)
  {
    ss << std::hex << static_cast<int>(ethernetHeader->destinationHost[i]);
    if(i != ETHERNET_MAC_ADDRESS_LEN - 1)
      ss << ":";
  }

  ss << " --> ";
  
  ss << "Destination: ";
  for(int i = 0; i < ETHERNET_MAC_ADDRESS_LEN; ++i)
  {
    ss << std::hex << static_cast<int>(ethernetHeader->destinationHost[i]);
    if(i != ETHERNET_MAC_ADDRESS_LEN - 1)
      ss << ":";
  }
  ss << "\n";
}
#endif
