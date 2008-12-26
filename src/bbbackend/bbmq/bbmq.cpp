#include "bbmq.h"

std::string
BBMQ::create_protobuf_data() throw() {
  std::string data = "";
  bb::IrcLog irclog;

  irclog.set_channel( "channel" );
  irclog.set_time( "time" );
  irclog.set_identifier( "identifier" );
  irclog.set_message( "message" );

  if ( !irclog.SerializeToString( &data ) ) {
	throw "Failed to parse irclog";
  }

  return data;
}

BBMQ::Client::Client( const std::string& brokerURI, const std::string& destURI)
{
  this->brokerURI = brokerURI;
  this->destURI = destURI;
}

BBMQ::Client::~Client () {
  cleanup();
}

void BBMQ::Client::send (const std::string& data) {
  try {
	activemq::core::ActiveMQConnectionFactory* connectionFactory =
	  new activemq::core::ActiveMQConnectionFactory( brokerURI );

	connection = connectionFactory->createConnection();
	connection->start();

	delete connectionFactory;

	session = connection->createSession( cms::Session::AUTO_ACKNOWLEDGE );

	destination = session->createQueue( destURI );

	producer = session->createProducer( destination );
	producer->setDeliveryMode( cms::DeliveryMode::NON_PERSISTENT );

	std::cout << data.size() << std::endl;
	unsigned char buf[data.size()];

	for ( unsigned int i = 0; i < data.size(); i++ ) {
	  buf[i] = data[i];
	}

	cms::BytesMessage* message = session->createBytesMessage( buf, sizeof buf );

	producer->send( message );
	delete message;

  } catch ( cms::CMSException& e ) {
	e.printStackTrace();
  }
}

std::string BBMQ::Client::recv () {
  std::cerr << "WORLD" << std::endl;
  return "HOGE";
}

void BBMQ::Client::cleanup () {

  delete destination;
  destination = NULL;

  delete producer;
  producer = NULL;

  session->close();
  connection->close();

  delete session;
  session = NULL;

  delete connection;
  connection = NULL;

}
