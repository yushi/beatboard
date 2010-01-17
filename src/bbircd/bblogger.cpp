#include "bblogger.h"

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
  cout << this->get_current_time_str() << " ";
  cout << level.data() << " ";
  cout << message.data() << endl;
  return;
}

string BeatBoard::BBLogger::get_current_time_str() {
  time_t t = time(NULL);
  struct tm *tm;
  char str[81];

  tm = localtime(&t);
  strftime(str, sizeof(str), "[%Y-%m-%d %H:%I:%S]", tm);

  return string(str);
}

