#ifndef __Network__RawPacketBuffer__
#define __Network__RawPacketBuffer__

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <pcap.h>

namespace netviz
{
  class RawPacketBuffer
  {
  public:
    RawPacketBuffer(size_t bufferSize);
    ~RawPacketBuffer();

    size_t getBufferSize() const;
    const u_char *getBuffer() const;

    // Indivdual components of raw packet.
    int getLinkLayerHeaderType() const;
    const struct pcap_pkthdr *getPcapHeader() const;
    const u_char *getPacketData() const;

    void setLinkLayerHeaderType(int linkLayerHeaderType);
    void setPcapHeader(const struct pcap_pkthdr *header);
    void setPacketData(const u_char *data, size_t length);

    void setFromRawBuffer(const void *data, size_t length);
  private:
    RawPacketBuffer(const RawPacketBuffer &rhs);
    const RawPacketBuffer &operator=(RawPacketBuffer &rhs);

    size_t _bufferSize;
    u_char *_buffer;
  };
}

#endif
