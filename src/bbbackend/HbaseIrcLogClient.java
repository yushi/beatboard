package bbbackend;


import java.io.IOException;
import java.util.UUID;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.io.BatchUpdate;

import bb.Bbprotobuf.IrcLog;


public class HbaseIrcLogClient extends HbaseClient
{
  public static final Log LOG = LogFactory.getLog( "bbbackend.HbaseIrcLogClient" );
  private HTable table = null;


  public HbaseIrcLogClient( final String tablename ) throws IOException
  {
    HBaseConfiguration config = new HBaseConfiguration();
    table = new HTable( config, tablename );
  }


  @Override
  public void insert( final UUID rowkey, final IrcLog log ) throws IOException
  {
    StringBuilder sb = new StringBuilder();
    LOG.info( sb.append( rowkey ).append( ":" ).append( log.getTime() ).append( log.getIdentifier() ).append(
      log.getMessage() ) );
    
    BatchUpdate batchUpdate = new BatchUpdate( rowkey.toString() );
    batchUpdate.put( "channel:columnQualifier1", log.getChannel().getBytes() );    
    batchUpdate.put( "time:columnQualifier1", log.getTime().getBytes() );
    batchUpdate.put( "identifier:columnQualifier1", log.getIdentifier().getBytes() );
    batchUpdate.put( "message:columnQualifier1", log.getMessage().toString().getBytes() );
    table.commit( batchUpdate );

    try
    {
      // hbase では timestamp が同一のレコードは同一バージョンとして内容が上書きされる
      // そのため、ログの挿入漏れが起こるので意図的に timestamp をずらすために sleep する
      Thread.sleep( 5 );
    }
    catch ( InterruptedException e )
    {
      LOG.info( "InterruptedException" );
      e.printStackTrace();
    }
  }

  @Override
  public void close()
  {
    table = null;
  }
  
  private boolean checkLogLogDuplicate()
  {
    return false;
  }


}
