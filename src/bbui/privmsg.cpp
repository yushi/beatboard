#include <exception>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <bb_ircd_event_queue.h>

using namespace std;
using namespace cgicc;
int
main(int argc, char **argv) {
  try {
    cgicc::Cgicc cgi;
    form_iterator target = cgi.getElement("target");
    form_iterator message = cgi.getElement("message");
    form_iterator nick = cgi.getElement("nick");
    BeatBoard::BBIRCD::EventQueue queue;
    BeatBoard::BBIRCD::Event ev;
    ev.set_type(BeatBoard::BBIRCD::TYPE_PRIVMSG);
    ev.add_value(target->getValue());
    ev.add_value(message->getValue());
    ev.add_value(nick->getValue());
    queue.enqueue(ev);
  } catch(const exception& e) {
    //handle the error
  }
}
