#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sys/time.h>
using namespace std;
namespace BeatBoard{
  class IRCChannel{
  public:
    IRCChannel();
    void addMessage(string from, string message);
    void addMessage(string from, string message, time_t time);
    void addUser(string user);
    void addUserJoin(string user);
    bool isJoined(string user);
    void delUser(string user);
    void addUserEnd();
    vector<string> getUsers();
    vector<string> getMessages();
    bool hasMessage();
    void recoverMessages();
  private:
    static int notify_timelimit;
    void removeOldMessages();
    bool userlist_receiving;
    vector<string> users;
    vector< pair<time_t, string> > messages;
    vector< pair<time_t, string> > messages_backup;
    string* new_message_from;
    string* new_message_body;
  };
}  
