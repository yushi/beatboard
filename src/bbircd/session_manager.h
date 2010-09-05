#ifndef __SESSION_MANAGER__
#define __SESSION_MANAGER__

#include <map>
#include <vector>
#include <string>
#include <irc_connection.h>
#include "bblogger.h"
#include <uuid/uuid.h>
#include "http_api_notifier.h"
using namespace std;
/*
 * session_id -> nick
 *
 * notify from irc_connection (MessageCollctor)
 *  `-> session_ids
 *         `-> store message buffer
 *                   `-> response for waiting http connection
 *
 */
//TODO: session expire

namespace BeatBoard {
  class MessageCollector : public Notifier {
  public:
    MessageCollector(string session_id,
                     map<string, vector<string> >* sessionBuffer,
                     bool* notifiable);
    virtual ~MessageCollector();
    bool notify(map<string, vector<string> >* arg);
  private:
    string session_id;
    map<string, vector<string> >* sessionBuffer;
    bool* notifiable;
  };

  class SessionManager {
  public:
    SessionManager();
    virtual ~SessionManager();
    void setUpTimer();
    string createSession(IRCConnection* conn); // return session_id
    void setHTTPAPIReadNotifier(string session_id, HTTPAPIReadNotifier* notifier);
    IRCConnection* getIRCConnectionBySessionId(string session_id);
    IRCConnection* getIRCConnectionByNick(string nick);
    bool isExistNick(string nick);
    void destroyAllSession();
    struct event timer;
    map<string, bool> notifiableSessionId;
    map<string, HTTPAPIReadNotifier*> waitingConnections;
    // { 'session_id' : {'channel' : ["messaage" ...] } }
    map<string, map<string, vector<string> > > sessionBuffers;
  private:
    string createSessionId();



    map<string, IRCConnection*> nick2ircConnection;
    map<string, string> sessionId2nick;

  };
}

#endif /* __SESSION_MANAGER__ */
