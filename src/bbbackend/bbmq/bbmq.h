// % g++ -G -o libbbmq.so -dynamiclib -Kpic bbmq.cpp bbprotobuf.pb.cc -I/usr/local/include/activemq-cpp-2.2/ -L/usr/local/lib -lactivemq-cpp -lprotobuf

#ifndef BBMQ_H
#define BBMQ_H

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/BytesMessage.h>
#include <iostream>
#include <string>
#include <exception>

#include "bbprotobuf.pb.h"

namespace BBMQ {
    
  class Client {
  private:
      std::string brokerURI;
      std::string destURI;
      cms::Connection* connection;
      cms::Session* session;
      cms::Destination* destination;
      cms::MessageProducer* producer;

  public:
      Client( const std::string& brokerURI, const std::string& destURI);
      virtual ~Client();
 
 void send (const std::string& data);
      std::string recv ();
      
  private:
      void cleanup();
  };

  std::string create_protobuf_data() throw();
}

#endif BBMQ_H
