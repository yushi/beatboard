#include "irc_channel.h"
int BeatBoard::IRCChannel::notify_timelimit = 10; //sec

BeatBoard::IRCChannel::IRCChannel(){
}

void BeatBoard::IRCChannel::addMessage(string from, 
                                       string message){
  this->addMessage(from, message, 0);
}

void BeatBoard::IRCChannel::addMessage(string from, 
                                               string message,
                                               time_t time){
  this->removeOldMessages();

  if(time == 0){
    struct timeval now;
    int ret = gettimeofday(&now, NULL);
    if(ret != 0){
      perror("addMessage failed");
      return;
    }
    time = now.tv_sec;
  }

  messages.push_back(make_pair(time, from));
  messages.push_back(make_pair(time, message));
}

void BeatBoard::IRCChannel::removeOldMessages(){
  struct timeval now;

  int ret = gettimeofday(&now, NULL);
  if(ret != 0){
    perror("addMessage failed");
    return;
  }

  vector< pair<time_t, string> >::iterator i = (this->messages).begin();
  while( i != (this->messages).end()){
    if( (now.tv_sec - i->first) > notify_timelimit){
      i = (this->messages).erase(i);
    }else{
      ++i;
    }
  }
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
  this->removeOldMessages();
  vector<string> ret;
  vector< pair<time_t, string> >::iterator i = (this->messages).begin();
  while( i != (this->messages).end()){
    ret.push_back(i->second);
    ++i;
  }
  (this->messages).clear();

  return ret;
}

bool BeatBoard::IRCChannel::hasMessage(){
  this->removeOldMessages();
  return ((this->messages).size() != 0);
}

