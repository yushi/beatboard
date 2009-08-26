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
    void addUserEnd();
    vector<string> getUsers();
    vector<string> getMessages();
    void recoverMessages();
    bool hasMessage();
  private:
    bool userlist_receiving;
    vector<string> users;
    vector<string> messages;
    vector<string> messages_backup;    
  };
}  
