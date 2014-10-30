//
//  ModelInputQueueProcessor.h
//  Network
//
//  Created by Dan Hutchinson on 15/10/2014.
//
//

#ifndef __Network__ModelInputQueueProcessor__
#define __Network__ModelInputQueueProcessor__

#include <string>

#include <mutex>
#include <future>
#include <thread>
#include <memory>

#include <zmq.hpp>

#include "NetworkModel.h"

namespace netviz
{
  class ModelInputQueueProcessor
  {
  public:
    ModelInputQueueProcessor(zmq::context_t &context, NetworkModel &model);
    ~ModelInputQueueProcessor();

    void startProcessing();
    void stopProcessing();
    
  private:
    ModelInputQueueProcessor(const ModelInputQueueProcessor &rhs);
    const ModelInputQueueProcessor &operator=(const ModelInputQueueProcessor &rhs);
    
    static std::string generateControlQueueName(int controlQueueId);
    
    void processorThread(int controlQueueId);
    
    std::mutex _objectMutex;
    
    zmq::context_t &_context;
    int _controlQueueId;
    
    std::promise<bool> _procesorReady;
    bool _processing;
    std::thread _processorThread;
    
    NetworkModel &_model;
  };

  typedef std::shared_ptr<ModelInputQueueProcessor> ModelInputQueueProcessorSP;
}

#endif
