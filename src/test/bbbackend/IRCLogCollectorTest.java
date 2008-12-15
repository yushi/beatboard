package test.bbbackend;


import static org.junit.Assert.*;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.junit.Before;
import org.junit.Test;

import bb.Bbprotobuf.IrcLog;
import bbbackend.IRCLogCollector;

import com.google.protobuf.ByteString;


public class IRCLogCollectorTest
{
  public static final Log LOG = LogFactory.getLog( "test.bbbackend.IRCLogCollectorTest" );
  private IRCLogCollector collector;
  private final String destURI = "TEST.FOO";
  private final String tablename = "testtable2";


  @Before
  public void setUp() throws Exception
  {
    collector = new IRCLogCollector( destURI, tablename );    
  }


  @Test
  public void testStartCollection() throws IOException
  {
    Thread thread = new Thread(collector);    
    
    IrcLog.Builder irclog = IrcLog.newBuilder();
    String channel = "#shibalab";
    irclog.setChannel( channel );
    String time = "2008/11/15 17:02";
    irclog.setTime( time );
    String name = "name";
    irclog.setIdentifier( name );
    String message = "testtest";
    irclog.setMessage( message );
    IrcLog irclog2 = irclog.build();
    ByteString data = irclog2.toByteString();
    byte[] bytedata = data.toByteArray();
    ActiveMQSender.send( destURI, bytedata );

    thread.start();

    HbaseDataGetClient datagetclient = new HbaseDataGetClient( tablename );
    HashMap<String, ArrayList<String>> ret = datagetclient.getData();

    for ( Entry<String, ArrayList<String>> entry : ret.entrySet() )
    {
      String rowkey = entry.getKey();
//      assertEquals( true, ret.containsKey( rowkey ) );
      ArrayList<String> result = ret.get( rowkey );
      assertEquals( channel, result.get( 0 ) );
      assertEquals( time, result.get( 1 ) );
      assertEquals( name, result.get( 2 ) );
      assertEquals( message, result.get( 3 ) );
    }

    collector.setReceiveloop( false );
  }


  @Test
  public void testParseIrcLogProtobuf() throws ClassNotFoundException, SecurityException, NoSuchMethodException,
    IllegalArgumentException, IllegalAccessException, InvocationTargetException
  {
    LOG.info( "start testParseIrcLogProtobuf()" );
    Class ircLogCollectorClass = Class.forName( "bbbackend.IRCLogCollector" );

    IrcLog.Builder irclog = IrcLog.newBuilder();
    String channel = "#shibalab";
    irclog.setChannel( channel );
    String time = "2008/11/15 17:02";
    irclog.setTime( time );
    String name = "name";
    irclog.setIdentifier( name );
    String message = "testtest";
    irclog.setMessage( message );
    IrcLog irclog2 = irclog.build();
    ByteString data = irclog2.toByteString();
    byte[] bytedata = data.toByteArray();

    Class argTypes[] = { byte[].class };
    Method method = ircLogCollectorClass.getDeclaredMethod( "parseIrcLogProtoBuf", argTypes );

    method.setAccessible( true );
//    IrcLog irclog3 = ( IrcLog ) method.invoke( method, bytedata );
    IrcLog irclog3 = ( IrcLog ) method.invoke( collector, bytedata );    
    assertEquals( "channel is wrong", channel, irclog3.getChannel() );
    assertEquals( "time is wrong", time, irclog3.getTime() );
    assertEquals( "identifier is wrong", name, irclog3.getIdentifier() );
    assertEquals( "message is wrong", message, irclog3.getMessage() );

    LOG.info( "end testParseIrcLogProtobuf()" );
  }

}
