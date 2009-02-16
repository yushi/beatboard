require 'bbirc'
nick = 'test_user'
chan = '#beatboard'

a = Bbirc::IRCConnection.new(nick)
a.connectIRCServer("127.0.0.1",6667)
a.joinIRCChannel(chan)
a.privMSG(chan,"test-")

Bbirc::IRCConnection::bb_event_dispatch
