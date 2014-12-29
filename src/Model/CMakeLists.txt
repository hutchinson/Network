SET(MODEL_HEADERS "Host.h"
                  "ModelInputQueueProcessor.h"
                  "NetworkModel.h"
                  "Packet.h"
                  )

SET(MODEL_SOURCE "Host.cpp"
                 "ModelInputQueueProcessor.cpp"
                 "NetworkModel.cpp"
                 "Packet.cpp"
                 )

INCLUDE_DIRECTORIES(${THIRDPARTY_ZMQ_INCLUDE})

ADD_LIBRARY(Model ${MODEL_HEADERS} ${MODEL_SOURCE})
