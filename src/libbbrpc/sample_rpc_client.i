%module sample_rpc_client
%{
#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>
#include "sample.pb.h"
%}
%{
extern void DoSearch();
extern void DoSearch2(char *);
extern const char* DoSearch3(char *);
%}

extern void DoSearch();
extern void DoSearch2(char *);
extern const char* DoSearch3(char *);