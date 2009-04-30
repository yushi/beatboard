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
    cout << HTTPHTMLHeader() << endl;
    form_iterator name = cgi.getElement("name");
    if(name != cgi.getElements().end()) {
      cout << "Your name: " << name->getValue() << endl;
    }
    BeatBoard::BBIRCD::EventQueue queue;
    BeatBoard::BBIRCD::Event a;
    a.set_type(BeatBoard::BBIRCD::TYPE_PRIVMSG);
    a.add_value(name->getValue());
    queue.enqueue(a);
  } catch(const exception& e) {
    //handle the error
  }
}
