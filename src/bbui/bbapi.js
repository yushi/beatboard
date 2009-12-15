var BBAPI = function(){
    var isUIBlocking = false;
    var loading = false;

    var readSuccess = function(data){
        debug_log('read response succeess');
        try{
            eval('received=' + data);
            for(channel in received){
                var messages = received[channel];
                for(var i = 0; i < messages.length; i+=2){
                    if(messages[i] == 'JOIN'){
                        var match_result = messages[i+1].match('(.+)!.*');
                        addMessage(messages[i], channel, match_result[1]);
                        $('#\\' + channel + '_users').append('<div class="user">' + match_result[1] + '</div>');
                    }else if(messages[i].match('QUIT.*')){
                        var match_result = messages[i+1].match('(.+)!.*');
                        addMessage(messages[i], channel, match_result[1]);
                        var delete_index = null;
                        for(var j = 0; j < $('#\\' + channel + '_users').children().length; j++){
                            if ($('#\\' + channel + '_users').children()[j].textContent == match_result[1]){
                                delete_index = j;
                            }
                        }
                        $($('#\\' + channel + '_users').children()[delete_index]).remove();
                    }else if(messages[i].match('PART.*')){
                        var match_result = messages[i+1].match('(.+)!.*');
                        addMessage(messages[i], channel, match_result[1]);
                        var delete_index = null;
                        for(var j = 0; j < $('#\\' + channel + '_users').children().length; j++){
                            if ($('#\\' + channel + '_users').children()[j].textContent == match_result[1]){
                                delete_index = j;
                            }
                        }
                        $($('#\\' + channel + '_users').children()[delete_index]).remove();
                    }else{
                        var match_result = messages[i].match('(.+)!.*');
                        if(match_result){
                            addMessage(match_result[1], channel, messages[i+1]);
                        }else{
                            $('#status').html('JOINERS: ' + messages[i+1]);
                        }
                    }
                }
            }
        }catch(e){
            //alert('error in read received')
        }
        window.scrollBy( 0, screen.height );
        update_debuginfo('read success');
    }
    
    var searchRecentLog = function(channel, count){
        $.get('/api/search',
              {
                  'q': 'channel:' + channel + ' limit:' + count + ' order:desc',
              },
              function(data){
                  debug_log('search recent log');
                  if(data != null){
                      eval('obj=' + data);
                      if(obj['messages'] != null){
                          obj['messages'].reverse();
                          for(var i in obj['messages']){
                              var date = obj['messages'][i][0];
                              var channel = obj['messages'][i][1];
                              var unixtime = obj['messages'][i][2];
                              var nick = obj['messages'][i][3];
                              var body = obj['messages'][i][4];
                              
                              var nickRegex = new RegExp("");
                              nickRegex.compile(/(.+)\!.+@.+/);
                              var nickResult  = nick.match(nickRegex);
                              if(nickResult){
                                  nick = nickResult[1];
                              }
                              addMessage(nick, channel, body, getTimeStrFromSearchResult(date));
                          }
                      }
                  }
              });
    }
    
    return {
        isLoading: function(){
          return loading;  
        },
        connect: function(server, nickname, port, pass){
            isUIBlocking = true;
            $.blockUI({message: ""});
            nick = nickname;
            $.cookie('nick',nickname, {expires: cookie_expire});
            $.cookie('server',server, {expires: cookie_expire});
            $.cookie('port',port, {expires: cookie_expire});
            $.cookie('pass',pass, {expires: cookie_expire});
            debug_log('connect req');
            $.post('/api/CONNECT',
                   {
                       'server': server,
                       'nick' : nick ,
                       'port' : port, 
                       'pass' : pass
                   },
                   function(data){
                       $.unblockUI();
                       isUIBlocking = false;
                       debug_log('connect res');
                       eval('obj=' + data);
                       $('#status').html(obj['reason']);
                       if(obj['status'].match('^OK$')){
                           $('#connect').toggle();
                           $('#join_channel').load('/join_channel.html');
                       }
                       if(obj['users']){
                           for(var channel in obj['users']){
                               createChannelUI(channel, 1);
                               searchRecentLog(channel, 10);
                               for(var i = 0; i < obj['users'][channel].length; i++){
                                   $('#\\' + channel + '_users').append('<div class="user">' + obj['users'][channel][i] + '</div>');
                               }
                           }
                       }
                       $('#send_message').load('/send_message.html',null,
                                               function(){
                                                   $('#message').focus();
                                                   
                                                   $('#message').get(0).onkeydown = function(e){
                                                       return !sendMessage(e);
                                                   }
                                               });
                       setInterval(checkLoader, 1000);
                   });
        },
        join: function(channel, nick){
            var url = '/api/JOIN';
            $.cookie('channel',channel, {expires: cookie_expire});
            active_channel = channel;
            debug_log('join req');
            $.post(url,
                   {
                       'channel':channel, //escape(channel),
                       'nick' : nick,
                   },
                   function(data){
                       debug_log('join res');
                       eval('obj=' + data);
                       $('#status').html(obj['reason']);
                       if(obj['status'].match('^OK$')){
                       }
                       createChannelUI(active_channel, 1);
                       $('message').focus();
                   });
        },
        privmsg: function(target, message, nick, callback){
            var url = '/api/SPEAK';
            if(target == null){
                return;
            }
            debug_log('privmsg req');
            addMessage(nick, active_channel,message);
            window.scrollBy( 0, screen.height );
            $.post(url,
                   {
                       'message':message,
                       'channel':target, //escape(target),
                       'nick':nick,
                   },
                   function(data){
                       debug_log('privmsg res');
                       if(callback){
                           callback();
                       }
                   });
            return true;
        },
        readMessage: function(nick, callback_readafter){
            if(loading){
                return;
            }
            loading = true;
            var url = '/api/READ';
            debug_log('read req');
            $.ajax({
                       'type': 'POST',
                       'url': url,
                       'data': {'nickname':nick},
                       cache: false,
                       success: function(data){
                           readSuccess(data);
                           loading = false;
                       },
                       error: function(XMLHttpRequest, textStatus, errorThrown){
                           debug_log('read response error');
                           loading = false;
                       }
                   });
            callback_readafter();
        }
    }
}();

