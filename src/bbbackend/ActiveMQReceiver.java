package bbbackend;

import javax.jms.JMSException;
import javax.jms.Queue;
import javax.jms.QueueConnection;
import javax.jms.QueueReceiver;
import javax.jms.QueueSession;
import javax.jms.BytesMessage;

import org.apache.activemq.ActiveMQConnection;
import org.apache.activemq.ActiveMQConnectionFactory;

/**
 * 指定された ActiveMQ のキューに接続し、メッセージを取得してくるクラス
 */
public class ActiveMQReceiver
{
  private ActiveMQConnectionFactory factory;
  private QueueConnection connection;
  private QueueSession session;
  private Queue queue;
  private QueueReceiver receiver;
  private final long TIMEOUT = 1000;

  /**
   * ActiveMQReceiver のコンストラクタ
   * 指定された ActiveMQ のキューに接続する
   * 
   * @param destURI
   * @throws JMSException
   */
  public ActiveMQReceiver( final String destURI ) throws JMSException
  {
    factory = new ActiveMQConnectionFactory( ActiveMQConnection.DEFAULT_BROKER_URL );
    connection = factory.createQueueConnection();
    session = connection.createQueueSession( false, QueueSession.AUTO_ACKNOWLEDGE );
    queue = session.createQueue( destURI );

    receiver = session.createReceiver( queue );
    connection.start();
  }

  /**
   * ActiveMQ よりメッセージをうけとるメソッド
   * 
   * @return byte[] ba
   * @throws JMSException
   * @throws ActiveMQReceiverTimeout 
   */
  public byte[] recv() throws JMSException, ActiveMQReceiverTimeout
  {
//    BytesMessage msg = ( BytesMessage ) receiver.receive();
    BytesMessage msg = ( BytesMessage ) receiver.receive(TIMEOUT);
    if (msg == null) {
      throw new ActiveMQReceiverTimeout();
    }
    
    long len = msg.getBodyLength();
    byte[] ba = new byte[( int ) len];
    msg.readBytes( ba );
    return ba;
  }

  /**
   * ActiveMQ との接続を切断するメソッド
   * @throws JMSException
   */
  public void close() throws JMSException
  {
    receiver.close();
    session.close();
    connection.close();
  }

}