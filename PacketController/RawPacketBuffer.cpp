#include "RawPacketBuffer.h"

#include <cassert>
#include <cstring>
#include <iostream>

namespace netviz
{
  RawPacketBuffer::RawPacketBuffer(size_t bufferSize)
  : _bufferSize(bufferSize), _buffer(NULL)
  {
    this->_buffer = reinterpret_cast<u_char*>(malloc(bufferSize));
  }

  RawPacketBuffer::~RawPacketBuffer()
  {
    if(this->_buffer)
      free(this->_buffer);
    this->_buffer = NULL;
  }

  size_t RawPacketBuffer::getBufferSize() const
  {
    return this->_bufferSize;
  }

  const u_char *RawPacketBuffer::getBuffer() const
  {
    return this->_buffer;
  }

  const int *RawPacketBuffer::getLinkLayerHeaderType() const
  {
    return reinterpret_cast<const int*>(this->_buffer);
  }

  const struct pcap_pkthdr *RawPacketBuffer::getPcapHeader() const
  {
    struct pcap_pkthdr *packetHeader = \
      reinterpret_cast<struct pcap_pkthdr*>((this->_buffer + sizeof(int)));
    return packetHeader;
  }

  const u_char *RawPacketBuffer::getPacketData() const
  {
    u_char *packetData = (this->_buffer + sizeof(int) + sizeof(struct pcap_pkthdr));
    return packetData;
  }

  void RawPacketBuffer::setLinkLayerHeaderType(int linkLayerHeaderType)
  {
    int *linkLayerType = reinterpret_cast<int*>(this->_buffer);
    *linkLayerType = linkLayerHeaderType;
  }

  void RawPacketBuffer::setPcapHeader(const struct pcap_pkthdr *header)
  {
    memcpy((this->_buffer + sizeof(int)), header, sizeof(struct pcap_pkthdr));
  }

  void RawPacketBuffer::setPacketData(const u_char *data, size_t length)
  {
    u_char *packetData = this->_buffer + sizeof(int) + sizeof(struct pcap_pkthdr);
    memcpy(packetData, data, length);
  }

  void RawPacketBuffer::setFromRawBuffer(const void *data, size_t length)
  {
    assert(length == this->_bufferSize);
    memcpy(this->_buffer, data, length);
  }
}