#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger {
private:
  ::std::fstream logFile;

public:

  Logger(::std::string fileName) {
    logFile.open(fileName, ::std::ios::out);
  }
  ~Logger() {
    logFile.close();
  }

  void logEvent(::std::string event) {
    logFile << "Event: " << event << ::std::endl;
  }
  void logError(::std::string error){
    logFile << "Error: " << error << ::std::endl;
  }
  void logSuccess(::std::string success)  {
    logFile << "Success: " << success << ::std::endl;
  }

};

#endif // LOGGER_H