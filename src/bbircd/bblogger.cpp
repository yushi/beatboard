#include "bblogger.h"
BeatBoard::BBLogger::BBLogger(){
  this->logger = &log4cpp::Category::getInstance(std::string(APPNAME4LOG));
  log4cpp::SimpleConfigurator::configure(INIFILE4LOG);
  return;
}

void BeatBoard::BBLogger::debug(const std::string message){
  this->logger->debug(message.data());
  return;
}
