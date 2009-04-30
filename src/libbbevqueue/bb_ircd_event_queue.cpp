#include "bb_ircd_event_queue.h"

BeatBoard::BBIRCD::EventQueue::EventQueue(){
  queue = BeatBoard::ProtobufQueueMemcached();
}

BeatBoard::BBIRCD::EventQueue::~EventQueue(){
  
}

BeatBoard::BBIRCD::Event BeatBoard::BBIRCD::EventQueue::dequeue(){
  string *serializedString = this->queue.dequeue();
  BeatBoard::BBIRCD::Event ret;
  ret.ParseFromString(* serializedString);
  return ret;
}

int BeatBoard::BBIRCD::EventQueue::enqueue(BeatBoard::BBIRCD::Event event){
  string serializedString;
  event.SerializeToString(&serializedString);
  return this->queue.enqueue(serializedString);
}

