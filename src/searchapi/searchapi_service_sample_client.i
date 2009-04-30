%module searchapi_service_sample_client
%{
#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>
#include "searchapi.pb.h"
#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"
#include "searchapi_service_common.h"
%}
%{
extern void DoSearchDummy();
extern const char* DoSearch(char *);
%}

extern void DoSearchDummy();
extern const char* DoSearch(char *);