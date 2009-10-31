#include <iostream>
#include <fstream>
#include <string>

#include "../sample.pb.h"
#include "../bb_rpc_service.h"
#include "../bb_rpc_controller.h"

class BBSampleRpc : public BeatBoard::BBRpcService {
private:
    sample::Response response;

public:
    BBSampleRpc( sample::RpcService* service );
    bool readRequest( const std::string& data );
    bool writeResult( std::string& data );
    void callServiceFunc( sample::Request& request );
    void Done();

};
