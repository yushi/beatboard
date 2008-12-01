package test.bbbackend;

import static org.junit.Assert.*;

import java.io.UnsupportedEncodingException;

import javax.jms.JMSException;

import org.junit.Before;
import org.junit.Test;

import bbbackend.ActiveMQReceiver;

public class ActiveMQReceiverTest
{
  private final String destURI = "TEST.FOO";
  private ActiveMQReceiver receiver = null;
  
  @Before
  public void setUp() throws Exception
  {
    receiver = new ActiveMQReceiver(destURI);
  }


  @Test
  public void testRecv() throws JMSException, UnsupportedEncodingException
  {
    final String msg = "This is Message!!";
    ActiveMQSender.send(destURI, msg.getBytes());
    byte[] message = receiver.recv();
    assertEquals( msg, new String(message, "utf-8") );
  }

}
