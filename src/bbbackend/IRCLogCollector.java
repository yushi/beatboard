package bbbackend;


import java.util.Map;

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
    // ActiveMQReceiver.recv
    // parseIrclog
    // HbaseIrcLogger.insert
  }

  /**
   * ActiveMQ からデキューした protobuf 形式のデータを IrcLog クラスに
   * パースして返すメソッド。
   * パースできない場合に発生する InvalidProtocolBufferException は
   * 内部で処理する。
   * 
   * @param data
   * @return パース成功ならば IrcLog、パース失敗ならば null
   */
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
