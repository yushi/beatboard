package bbbackend;


import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.Descriptors.FieldDescriptor;

import bb.Bbprotobuf.IrcLog;

/**
 * BBBackend Log 管理クラス<br>
 * ActiveMQ にエンキューされたログをデキューし HbaseIrcLogClient 経由で
 * Hbase に保存する
 */
public class IRCLogCollector
{
  public static final Log LOG = LogFactory.getLog( "bbbackend.IRCLogCollector" );

  public IRCLogCollector() {
  }
  
  public void startCollection() {
    // ActiveMQReceiver.recv
    // parseIrclog
    // HbaseIrcLogClient.insert
  }

  /**
   * ActiveMQ からデキューした protobuf 形式のデータを IrcLog クラスに
   * パースして返すメソッド。<br>
   * パースできない場合は InvalidProtocolBufferException を投げる。
   * 
   * @param data
   * @return パース成功ならば IrcLog
   * @throws InvalidProtoclBufferException 
   */
  private static IrcLog parseIrcLogProtoBuf( final byte data[] ) throws InvalidProtoclBufferException {
    IrcLog irclog = null;
    try
    {
      irclog = IrcLog.parseFrom( data );
    }
    catch ( InvalidProtocolBufferException e )
    {
      LOG.info( "InvalidProtocolBufferException" );
      e.printStackTrace();      
      throw new InvalidProtoclBufferException();
    }
    printIrcLog( irclog );
    
    return irclog;
  }

  /**
   * IrcLog クラスを print するメソッド
   * 
   * @param irclog
   */
  private static void printIrcLog( IrcLog irclog )
  {
    LOG.info( "Parse irclog... " );    
    Map<FieldDescriptor, Object> irclogfields = irclog.getAllFields();
    for(Map.Entry<FieldDescriptor, Object> entry : irclogfields.entrySet()) {
      LOG.info( "key: " + entry.getKey().getName());
      LOG.info( "value: " + entry.getValue() );
    }
  }
  
}
