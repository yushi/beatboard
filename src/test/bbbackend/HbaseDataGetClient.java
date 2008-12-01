package test.bbbackend;


import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Scanner;
import org.apache.hadoop.hbase.io.RowResult;


public class HbaseDataGetClient
{
  private String tablename;
  private HTable table;


  public HbaseDataGetClient( final String tablename ) throws IOException
  {
    HBaseConfiguration config = new HBaseConfiguration();
    table = new HTable( config, tablename );
  }


  public HashMap<String, ArrayList<String>> getData() throws IOException
  {
    HashMap<String, ArrayList<String>> ret = new HashMap<String, ArrayList<String>>();

    Scanner scanner = table.getScanner( new String[]
      { "channel:columnQualifier1", "time:columnQualifier1", "message:columnQualifier1", "identifier:columnQualifier1" } );

    for ( RowResult result : scanner )
    {
      ArrayList<String> record = new ArrayList<String>();
      
      // print out the row we found and the columns we were looking for
      String row = new String( result.getRow() );
      String channel = new String(result.get( "channel:columnQualifier1".getBytes() ).getValue());      
      String time = new String(result.get( "time:columnQualifier1".getBytes() ).getValue());
      String identifier = new String(result.get( "identifier:columnQualifier1".getBytes() ).getValue());
      String message = new String(result.get( "message:columnQualifier1".getBytes() ).getValue());

      System.err.println( "Found row: " + row + " with value: " + channel + " " + time + " " + identifier + " " + message);
      
      record.add( channel );
      record.add( time );
      record.add( identifier );
      record.add( message );
      ret.put( row, record );
    }

    scanner.close();
    return ret;
  }
}
