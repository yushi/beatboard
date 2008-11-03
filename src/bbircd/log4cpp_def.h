#ifndef LOG4CPPDEF_H_
#define LOG4CPPDEF_H_

// log4cpp
#include <log4cpp/Portability.hh>
#ifdef WIN32
#include 
#endif

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/NDC.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/SimpleConfigurator.hh>

// 設定ファイルのパス
#define INIFILE4LOG "./log4cpp.init"
// アプリ名
#define APPNAME4LOG "bbircd"

//extern log4cpp::Category& logger = log4cpp::Category::getInstance(std::string(APPNAME4LOG));

#endif /*LOG4CPPDEF_H_*/
