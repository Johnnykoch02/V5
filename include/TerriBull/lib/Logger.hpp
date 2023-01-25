#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <stream>

using namespace std;

class Logger {
public:
  Logger(std::string fileName);
  ~Logger();

  void logEvent(std::string event);
  void logError(std::string error);
  void logSuccess(std::string success);

private:
  std::fstream logFile;
};

Logger::Logger(std::string fileName) {
  logFile.open(fileName, std::ios::out);
}

Logger::~Logger() {
  logFile.close();
}

void Logger::logEvent(std::string event) {
  logFile << "Event: " << event << std::endl;
}

void Logger::logError(std::string error) {
  logFile << "Error: " << error << std::endl;
}

void Logger::logSuccess(std::string success) {
  logFile << "Success: " << success << std::endl;
}

#endif // LOGGER_H