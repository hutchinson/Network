#include "Logger.h"

#ifdef DEBUG
#include <mutex>
#include <iomanip>
#include <ctime>
#include <sstream>
#endif

namespace netviz
{
#ifdef DEBUG
  namespace
  {
    std::mutex gLogMutex;
  }

  void LOG_DEBUG(const std::string &message)
  {
    std::lock_guard<std::mutex> lock(gLogMutex);

    std::stringstream ss;
    std::time_t result = std::time(NULL);
    ss << "[DEBUG] " << std::put_time(std::localtime(&result), "%c %Z") << " : " << message << "\n";
    std::string logMessage = ss.str();
    std::cout << logMessage;
  }
#else
  void LOG_DEBUG(const std::string &message)
  {

  }
#endif
}