SET(CONTROLLER_HEADERS ethertype.h
                       IDGenerator.h
                       PacketDecoder.h
                       PacketFormat.h
                       PacketListener.h
                       RawPacketBuffer.h
                       ZmqQueueNames.h
                       )

SET(CONTROLLER_SOURCE IDGenerator.cpp
                      PacketDecoder.cpp
                      PacketListener.cpp
                      RawPacketBuffer.cpp
                      ZmqQueueNames.cpp
                      )

INCLUDE_DIRECTORIES(${THIRDPARTY_ZMQ_INCLUDE})

ADD_LIBRARY(PacketController ${CONTROLLER_HEADERS} ${CONTROLLER_SOURCE})
