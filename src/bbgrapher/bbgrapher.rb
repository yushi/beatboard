require 'date'
class LogParser
  def initialize
  end

  def parse_line(line)
  end

  def pre_process
  end

  def post_process
  end
end

class BBIRCDLogParser < LogParser
  def initialize
    require 'rubygems'
    require 'gruff'
    @data = {
      :connect => {
        :request => [],
      },
      :read => {
        :request => [],
      },
    }
  end
  
  def parse_line(line)
    l = line.scan(/\[(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})\]\s(\S+)\s(.*)/)[0]
    date = DateTime.strptime(l[0], "%Y-%m-%d %H:%M:%S")
    return unless l[1] == 'DEBUG'
    
    e = l[2].split(/\s/)
    case e.shift
    when 'READ'
      case e[0] 
      when 'request'
        sid = e[2].split(':')[1]
        @data[:read][:request] << [date, sid]
      when 'finished'
      end
    when 'CONNECT'
      nick = e[0].split(':')[1]
      @data[:connect][:request] << [date, nick]
    end
  end

  def graph_connect
    g = Gruff::Line.new
    g.title = "BBIRCD Connect" 

    nicks = @data[:connect][:request].map{|e| e[1]}.uniq
    nicks.each{|nick|
      datetimes = @data[:connect][:request].map{|e| e[0] if e[1] == nick}
      day_freq = []
      days = datetimes.map{|d| d.day}
      days.each{|d|
        day_freq[d] = 0 unless day_freq[d]
        day_freq[d] += 1
      }
      #p day_freq
      g.data(nick, day_freq)
    }
    g.labels = {1 => '1', 2 => '2', 8 => '8', 31 => '31'}
    g.y_axis_label = "num requests"
    g.x_axis_label = "day"
    g.minimum_value = 0
    g.write('/usr/bb/htdocs/graph/connect.png')
    p "outed"
    return
  end
  
  def post_process
    #require 'pp'
    #pp @data
    self.graph_connect
  end
end

class MrTailer
  BUFFSIZE = 1024
  def initialize(filename)
    @read_buffer = ""
    @lines = []
    @f = File.open(filename, 'r')
  end
  
  def has_line?
    return @lines.size != 0
  end

  def readline
    return @lines.shift
  end

  def poll
    begin
      begin
        read_data =  @f.read_nonblock(BUFFSIZE)
        @read_buffer += read_data
      end while read_data.size > 0
    rescue EOFError
    end
    while @read_buffer.include?("\n")
      pos = @read_buffer.index("\n")
      @lines << @read_buffer[0, pos+1]
      @read_buffer = @read_buffer[pos+1, @read_buffer.size - pos]
    end
  end
end

log = '/usr/bb/var/log/bbircd.log'
t = MrTailer.new(log)
parser = BBIRCDLogParser.new

while true
  t.poll
  if t.has_line?
    while t.has_line?
      parser.parse_line(t.readline)
    end
    parser.post_process
  end
  sleep 0.1
end

