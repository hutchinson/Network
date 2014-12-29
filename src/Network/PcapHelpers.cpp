//
//  PcapHelpers.cpp
//  Network
//
//  Created by Dan Hutchinson on 29/10/2014.
//
//

#include "PcapHelpers.h"

#include <pcap/pcap.h>
#include <iostream>

namespace Helpers
{
  void AvailableInterfaces(InterfaceList &interfaces)
  {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *deviceList = NULL;

    if(pcap_findalldevs(&deviceList, errbuf))
    {
      std::cerr << errbuf << std::endl;
      return;
    }
    
    pcap_if_t *device = deviceList;
    while(device)
    {
      NetworkInterfaceSP interface( new NetworkInterface(device->name ? device->name : "",
                                                         device->description ? device->description : "",
                                                         device->flags & PCAP_IF_LOOPBACK,
                                                         false,
                                                         false));
      interfaces.push_back(interface);
      device = device->next;
    }
    pcap_freealldevs(deviceList);
  }
}