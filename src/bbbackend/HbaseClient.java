package bbbackend;

import java.io.IOException;
import java.util.UUID;

import bb.Bbprotobuf.IrcLog;

public abstract class HbaseClient
{
  abstract public void insert(final UUID rowkey, final IrcLog log) throws IOException;
  abstract public void close();
}
