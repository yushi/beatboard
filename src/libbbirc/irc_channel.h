#include <vector>
#include <string>
#include <iostream>
using namespace std;
namespace BeatBoard{
  class IRCChannel{
  public:
    IRCChannel();
    void addMessage(string from, string message);
    void addUser(string user);
    void addUserJoin(string user);
    void delUser(string user);
    void addUserEnd();
    vector<string> getUsers();
    vector<string> getMessages();
    bool hasMessage();
  private:
    bool userlist_receiving;
    vector<string> users;
    string* new_message_from;
    string* new_message_body;
    vector<string> messages_backup;    
  };
}  
