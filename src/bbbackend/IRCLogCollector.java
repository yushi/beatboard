package bbbackend;


import java.io.FileDescriptor;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.Descriptors.FieldDescriptor;

import bb.Bbprotobuf.IrcLog;

/**
 * BBBackend Log 管理クラス
 * ActiveMQ にエンキューされたログをデキューし HbaseIrcLogClient 経由で
 * Hbase に保存する
 */
public class IRCLogCollector
{
  public static final Log LOG = LogFactory.getLog( "bbbackend.IRCLogCollector" );

  public IRCLogCollector() {
  }
  
  public void startCollection() {
    
  }

  private static String popIrcLog() {
    return null;
  }
  
  private static IrcLog parseIrcLogProtoBuf( final byte data[] ) {
    IrcLog irclog = null;
    try
    {
      irclog = IrcLog.parseFrom( data );
    }
    catch ( InvalidProtocolBufferException e )
    {
      LOG.info( "InvalidProtocolBufferException" );
      printIrcLog( irclog );
      e.printStackTrace();
    }
    printIrcLog( irclog );
    
    return irclog;
  }

  private static void printIrcLog( IrcLog irclog )
  {
    LOG.info( "Parse irclog... " );    
    Map<FieldDescriptor, Object> irclogfields = irclog.getAllFields();
    for(Map.Entry<FieldDescriptor, Object> entry : irclogfields.entrySet()) {
      LOG.info( "key: " + entry.getKey().getName());
      LOG.info( "value: " + entry.getValue() );
    }
  }
  
  private static String insertIrcLog() {
    return null;
  }
}
