#ifndef BBRPCSERVICE_H
#define BBRPCSERVICE_H

#include <string>
#include <iostream>

namespace BeatBoard {

  class BBRpcService {
  protected:
    void* service;

  public:
    BBRpcService( void* service );
    virtual ~BBRpcService();
    virtual bool readRequest( const std::string& data );
    virtual bool writeResult( std::string& data );
    virtual void Done();
  };
}

#endif
