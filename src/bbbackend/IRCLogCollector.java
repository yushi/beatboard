package bbbackend;


import java.io.IOException;
import java.util.Map;
import java.util.UUID;

import javax.jms.JMSException;

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
public class IRCLogCollector implements Runnable
{
  public static final Log LOG = LogFactory.getLog( "bbbackend.IRCLogCollector" );
  private HbaseIrcLogClient hbaseclient;
  private ActiveMQReceiver receiver;
  private Boolean receiveloop = false;
//  private final long INTERVAL = 1000;


  public IRCLogCollector( final String destURI, final String tablename )
  {
    try
    {
      receiver = new ActiveMQReceiver( destURI );
      hbaseclient = new HbaseIrcLogClient( tablename );
      receiveloop = true;
    }
    catch ( IOException e )
    {
      LOG.info( e );
      e.printStackTrace();
      throw new Error( "Could not connect Hbase" );
    }
    catch ( JMSException e )
    {
      LOG.info( e );
      e.printStackTrace();
      throw new Error( "Could not connect ActiveMQ" );
    }
  }


  public void run()
  {
    while ( receiveloop )
    {
      byte[] message;
      try
      {
        LOG.info( "receive..." );
        message = receiver.recv();
        IrcLog log = parseIrcLogProtoBuf( message );
        final UUID rowkey = UUID.randomUUID();
        LOG.info( "insert..." );
        hbaseclient.insert( rowkey, log );
      }
      catch ( ActiveMQReceiverTimeout e )
      {
        LOG.info( "timeout..." );
        e.printStackTrace();
      }
      catch ( JMSException e )
      {
        LOG.info( e );
        e.printStackTrace();
      }
      catch ( InvalidProtoclBufferException e )
      {
        LOG.info( e );
        e.printStackTrace();
      }
      catch ( IOException e )
      {
        LOG.info( e );
        e.printStackTrace();
      }
//      finally
//      {
//        try
//        {
//          LOG.info( "sleep" );;
//          Thread.sleep( INTERVAL );
//        }
//        catch ( InterruptedException e )
//        {
//          LOG.info( e );
//          e.printStackTrace();
//          break;
//        }
//      }
    }
    try
    {
      receiver.close();
    }
    catch ( JMSException e )
    {
      LOG.info( e );
      e.printStackTrace();
    }
    finally
    {
      hbaseclient.close();
    }
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
  private IrcLog parseIrcLogProtoBuf( final byte data[] ) throws InvalidProtoclBufferException
  {
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
  private void printIrcLog( IrcLog irclog )
  {
    LOG.info( "Parse irclog... " );
    Map<FieldDescriptor, Object> irclogfields = irclog.getAllFields();
    for ( Map.Entry<FieldDescriptor, Object> entry : irclogfields.entrySet() )
    {
      LOG.info( "key: " + entry.getKey().getName() );
      LOG.info( "value: " + entry.getValue() );
    }
  }


  public void setReceiveloop( Boolean receiveloop )
  {
    this.receiveloop = receiveloop;
  }

}
