#include "IDGenerator.h"

namespace netviz {
  namespace
  {
    std::atomic_int gNextID;
  }
  
  int GetUniqueID()
  {
    return gNextID++;
  }
}