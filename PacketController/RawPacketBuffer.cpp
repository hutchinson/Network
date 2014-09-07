#include "RawPacketBuffer.h"

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

  const struct pcap_pkthdr *RawPacketBuffer::getPcapHeader() const
  {
    return reinterpret_cast<const struct pcap_pkthdr*>(this->_buffer);
  }

  const u_char *RawPacketBuffer::getPacketData() const
  {
    u_char *packetData = this->_buffer + sizeof(struct pcap_pkthdr);
    return packetData;
  }

  void RawPacketBuffer::setPcapHeader(const struct pcap_pkthdr *header)
  {
    memcpy(this->_buffer, header, sizeof(struct pcap_pkthdr));
  }

  void RawPacketBuffer::setPacketData(const u_char *data, size_t length)
  {
    u_char *packetData = this->_buffer + sizeof(struct pcap_pkthdr);
    memcpy(packetData, data, length);
  }
}