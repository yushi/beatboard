#include "irc_channel.h"

BeatBoard::IRCChannel::IRCChannel(){
}

void BeatBoard::IRCChannel::addMessage(string from, string message){
  this->messages.push_back(from);
  this->messages.push_back(message);
}

void BeatBoard::IRCChannel::delUser(string user){
  vector<string>::iterator i = (this->users).begin();
  if(user.find('!')){
    user = user.substr(0, user.find('!'));
  }
  while(i != (this->users).end()){
    string target = *i;
    if(target[0] == '@'){
      target = target.substr(1,target.size() - 1);
    }
    if(target == user){
      i = (this->users).erase(i);
    }else{
      ++i;
    }
  }
}

void BeatBoard::IRCChannel::addUserJoin(string user){
  if(user.find('!')){
    user = user.substr(0, user.find('!'));
  }
  this->users.push_back(user);
}

void BeatBoard::IRCChannel::addUser(string user){
  if(!this->userlist_receiving){
    this->users.clear();
    this->userlist_receiving = true;
  }
  this->users.push_back(user);
}

void BeatBoard::IRCChannel::addUserEnd(){
  if(this->userlist_receiving){
    this->userlist_receiving = false;
  }
}

vector<string> BeatBoard::IRCChannel::getUsers(){
  return this->users;
}

vector<string> BeatBoard::IRCChannel::getMessages(){
  vector<string> ret = this->messages;
  this->messages.clear();
  this->messages_backup = ret;

  return ret;
}

void BeatBoard::IRCChannel::recoverMessages(){
  this->messages = this->messages_backup;
}

bool BeatBoard::IRCChannel::hasMessage(){
  return (this->messages.size() != 0);
}

