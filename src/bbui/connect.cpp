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
    form_iterator server = cgi.getElement("server");
    form_iterator nick = cgi.getElement("nick");
    form_iterator port = cgi.getElement("port");
    BeatBoard::BBIRCD::EventQueue queue;
    BeatBoard::BBIRCD::Event ev;
    ev.set_type(BeatBoard::BBIRCD::TYPE_CONNECT);
    ev.add_value(server->getValue());
    ev.add_value(nick->getValue());
    ev.add_value(port->getValue());
    queue.enqueue(ev);
  } catch(const exception& e) {
    //handle the error
  }
}
