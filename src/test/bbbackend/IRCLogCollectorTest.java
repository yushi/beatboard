package test.bbbackend;

import static org.junit.Assert.*;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.junit.Before;
import org.junit.Test;

import bb.Bbprotobuf.IrcLog;

import com.google.protobuf.ByteString;

public class IRCLogCollectorTest
{
  public static final Log LOG = LogFactory.getLog("test.bbbackend.IRCLogCollectorTest");

  @Before
  public void setUp() throws Exception
  {
  }


  @Test
  public void testStartCollection()
  {
    //fail( "Not yet implemented" );
  }
  
  @Test
  public void testParseIrcLogProtobuf() throws ClassNotFoundException, SecurityException, NoSuchMethodException, IllegalArgumentException, IllegalAccessException, InvocationTargetException
  {
    LOG.info( "start testParseIrcLogProtobuf()" );
    Class ircLogCollectorClass = Class.forName( "bbbackend.IRCLogCollector" );    
    
    IrcLog.Builder irclog = IrcLog.newBuilder();
    String channel = "#shibalab";
    irclog.setChannel( channel );
    String time = "2008/11/15 17:02";
    irclog.setTime( time );
    String name = "name";
    irclog.setIdentifier(name);
    String message = "testtest";
    irclog.setMessage( message );
    IrcLog irclog2 = irclog.build();    
    ByteString data = irclog2.toByteString();
    byte [] bytedata = data.toByteArray();
    
    Class argTypes[] = {byte[].class};
    Method method = ircLogCollectorClass.getDeclaredMethod( "parseIrcLogProtoBuf", argTypes );
    
    method.setAccessible( true );
    IrcLog irclog3 = ( IrcLog ) method.invoke( method, bytedata );
    assertEquals( "channel is wrong", channel,  irclog3.getChannel());
    assertEquals( "time is wrong", time,  irclog3.getTime());
    assertEquals( "identifier is wrong", name,  irclog3.getIdentifier());
    assertEquals( "message is wrong", message,  irclog3.getMessage());
    
    LOG.info( "end testParseIrcLogProtobuf()" ); 
  }

}
