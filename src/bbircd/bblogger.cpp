#include "bblogger.h"

char* BeatBoard::BBLogger::logfile = NULL;

BeatBoard::BBLogger::BBLogger() {
  return;
}

void BeatBoard::BBLogger::debug(const string message) {
  this->printLog("DEBUG", message);
  return;
}

void BeatBoard::BBLogger::info(const string message) {
  this->printLog("INFO", message);
  return;
}

void BeatBoard::BBLogger::printLog(const string level, const string message) {
  if(this->lout == NULL){
    this->lout = new std::fstream(this->logfile, std::ios::out);
  }
  
  *(this->lout) << this->get_current_time_str() << " ";
  *(this->lout) << level.data() << " ";
  *(this->lout) << message.data() << endl;
  this->lout->flush();
  return;
}

string BeatBoard::BBLogger::get_current_time_str() {
  time_t t = time(NULL);
  struct tm *tm;
  char str[81];

  tm = localtime(&t);
  strftime(str, sizeof(str), "[%Y-%m-%d %H:%M:%S]", tm);

  return string(str);
}

