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
    form_iterator channel = cgi.getElement("channel");
    form_iterator nick = cgi.getElement("nick");
    BeatBoard::BBIRCD::EventQueue queue;
    BeatBoard::BBIRCD::Event ev;

    ev.set_type(BeatBoard::BBIRCD::TYPE_JOIN);
    cerr << channel->getValue() << endl;
    ev.add_value(channel->getValue());
    ev.add_value(nick->getValue());
    queue.enqueue(ev);
  } catch(const exception& e) {
    //handle the error
  }
}
