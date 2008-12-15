package test.bbbackend;

import static org.junit.Assert.*;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.UUID;

import org.junit.Before;
import org.junit.Test;

import bb.Bbprotobuf.IrcLog;
import bbbackend.HbaseIrcLogClient;;

public class HbaseIrcLogClientTest
{
  public HbaseIrcLogClient insertclient;
  public HbaseDataGetClient datagetclient;

  @Before
  public void setUp() throws Exception
  {
    // HQL LOG
    // > create 'testtable2', {NAME=>'channel', VERSIONS=>100}, {NAME=>'time', VERSIONS=>100}, {NAME=>'identifier', VERSIONS=>100}, {NAME=>'message', VERSIONS=>100}
    String tablename = "testtable2";
    insertclient = new HbaseIrcLogClient(tablename);
    datagetclient = new HbaseDataGetClient(tablename);
  }


  @Test
  public void testInsert() throws IOException
  {
    IrcLog.Builder irclogbuilder = IrcLog.newBuilder();
    String channel = "#shibalab";
    irclogbuilder.setChannel( channel );
    String time = "2008/11/15 17:02";
    irclogbuilder.setTime( time );
    String name = "name";
    irclogbuilder.setIdentifier(name);
    String message = "testtest";
    irclogbuilder.setMessage( message );
    IrcLog irclog = irclogbuilder.build();
    
    final UUID rowkey = UUID.randomUUID();
    
    insertclient.insert( rowkey, irclog );
    
    HashMap<String, ArrayList<String>> ret = datagetclient.getData();
    assertEquals(true, ret.containsKey( rowkey.toString() ));
    
    ArrayList<String> result = ret.get( rowkey.toString() );    
    assertEquals( channel, result.get( 0 ) );
    assertEquals( time, result.get( 1 ) );
    assertEquals( name, result.get( 2 ) );
    assertEquals( message, result.get( 3 ) );
  }

}
