#include "bblogger.h"

BeatBoard::BBLogger::BBLogger(){
  return;
}

void BeatBoard::BBLogger::debug(const std::string message){
  this->printLog( "DEBUG", message);
  return;
}

void BeatBoard::BBLogger::info(const std::string message){
  this->printLog( "INFO", message);
  return;
}

void BeatBoard::BBLogger::printLog(const std::string level, const std::string message){
  std::cout << level.data() << ": " << message.data() << std::endl;
  return;
}
