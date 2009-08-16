var loading = 0;
var nick = null;
var active_channel = null;
var noexec = 0;
var debug = 0;

$.ajaxSetup({'timeout': 300000});

function connectServer(){
    var server = $('#server').val();
    var nick = $('#nick').val();
    var port = $('#port').val();
    connect(server, nick, port);
    return false;
}

function joinChannel(elem){
    var channel = $('#channel').val();
    var nick = $('#nick').val();
    join(channel, nick);
    return false;
}

function sendMessage(elem){
    var target = $('#target').val();
    var message = $('#message').val();
    
    if(message.length != 0){
        privmsg(target, message, nick);
        $('#message').val('');
    }
    return false;
}

function checkLoader(){
    if(loading == 0){
        loading = 1;
        readMessage(nick);
        
    }
    update_debuginfo('checkLoader');
}


function connect(server, nickname, port){
    nick = nickname;
    $.cookie('nick',nickname);
    $.cookie('server',server);
    $.cookie('port',port);
    debug_log('connect req');
    $.post('/api/CONNECT',
           {
               'server': server,
               'nick' : nick ,
               'port' : port
           },
           function(data){
               debug_log('connect res');
               eval('obj=' + data);
               $('#status').html(obj['reason']);
               if(obj['status'].match('^OK$')){
                   $('#connect').toggle();
                   $('#join_channel').load('join_channel.html');
                   $('#nick').val(nick);
               }
           });
    
}

function join(channel, nick){
    var url = '/api/JOIN';
    $.cookie('channel',channel);
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
                   $('#send_message').load('send_message.html', null, function(){
                                               $('#nick').val(nick);
                                               $('#target').val(active_channel);
                                               $('#join_channel').hide();
                                               $('#head').append(channel);
                                               setInterval(checkLoader, 1000);
                                           });
               }
           });
}

function privmsg(target, message, nick){
    var url = '/api/SPEAK';
    debug_log('privmsg req');
    $.post(url,
           {
               'message':message,
               'channel':target, //escape(target),
               'nick':nick,
           },
           function(data){
               debug_log('privmsg res');
               $('#messages').append(nick + ':' + replace_centity_ref(message) + '<br />');
               window.scrollBy( 0, screen.height );
           });
}

function readMessage(nick){
    var url = '/api/READ';
    debug_log('read req');
    $.ajax({
               'type': 'POST',
               'url': url,
               'data': {'nickname':nick},
               cache: false,
               success: function(data){
                   debug_log('read response succeess');
                   try{
                       eval('received=' + data);
                       var messages = received[active_channel];
                       for(var i = 0; i < messages.length; i+=2){
                           var match_result = messages[i].match('(.+)!.*');
                           if(match_result){
                               $('#messages').append(match_result[1] + ': ');
                               $('#messages').append(replace_centity_ref(messages[i+1]) + '<br />');
                           }else{
                               $('#status').html('JOINERS: ' + messages[i+1]);
                           }
                       }
                   }catch(e){
                       alert('error in read received')
                   }
                   loading = 0;
                   window.scrollBy( 0, screen.height );
                   update_debuginfo('read success');
               },
               error: function(XMLHttpRequest, textStatus, errorThrown){
                   debug_log('read response error');
                   loading = 0;
               }
           });
}

function getopt(){
    var args = new Object();
    var args_string = location.href.split('?')[1];
    if(!args_string){
        return args;
    }
    var args_array = args_string.split('&');
    for(var i = 0; i < args_array.length; i++){
        var arg = args_array[i].split('=');
        args[arg[0]] = arg[1];
    }
    return args;
}

function replace_centity_ref(message) {
    var centity_ref_array = new Array();
    centity_ref_array.push(new Array(RegExp('&', 'g'), '&amp;'));
    centity_ref_array.push(new Array(RegExp('\'', 'g'), '&quot;'));
    centity_ref_array.push(new Array(RegExp('<', 'g'), '&lt;'));
    centity_ref_array.push(new Array(RegExp('>', 'g'), '&gt;'));
    for(var i = 0; i < centity_ref_array.length; i++) {
        message = message.replace(centity_ref_array[i][0],
                                  centity_ref_array[i][1]);
    }
    return message;
}

function init(){
    var args = getopt();
    if(args['debug']){
        debug = new Object();
    }else{
        $('#debug').hide();
        $('#debug_menu').hide();
        $('#debug_log').hide();
    }

    nick = $.cookie('nick');
    if($.cookie('server') && $.cookie('port') && $.cookie('nick')){
        connect($.cookie('server'), $.cookie('nick'), $.cookie('port'));
    }
    if($.cookie('channel')){
        join($.cookie('channel'), $.cookie('nick'));
    }
    update_debuginfo('init');
    debug_log('init');
}

function update_debuginfo(from){
    if(debug){
        var debug_text = '';
        debug_text += 'loading = ' + loading + '<br />';
        debug_text += 'last updated = ' + (new Date).toLocaleString() + '<br />';
        if(from){
            debug_text += 'updated by ' + from + '<br />';
        }
        $('#debug_info').html(debug_text);
    }
    return;
}

function debug_log(message){
    if(debug){
        var debug_text = (new Date).toLocaleString() + ':  ' + message + '<br />';
        $('#debug_log').append(debug_text);
    }
    return;
}

function delete_cookie(){
    $.cookie('server',null);
    $.cookie('nick', null);
    $.cookie('port', null);
    $.cookie('channel', null);
}
init();
