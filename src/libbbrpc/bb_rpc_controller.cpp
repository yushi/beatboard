#include "bb_rpc_controller.h"

BeatBoard::BBRpcController::BBRpcController()
{
}

BeatBoard::BBRpcController::~BBRpcController()
{
}

void
BeatBoard::BBRpcController::Reset()
{
}

void
BeatBoard::BBRpcController::StartCancel()
{
}

void
BeatBoard::BBRpcController::SetFailed( const std::string& reason )
{
}

void
BeatBoard::BBRpcController::NotifyOnCancel( google::protobuf::Closure* callback )
{
}

std::string
BeatBoard::BBRpcController::ErrorText() const
{
  return NULL;
}

bool
BeatBoard::BBRpcController::Failed() const
{
  return false;
}

bool
BeatBoard::BBRpcController::IsCanceled() const
{
  return false;
}
