var loading = 0;
var nick = null;
var active_channel = null;
var noexec = 0;
$.ajaxSetup({'timeout': 300000});
function connectServer(){
    var server = $("#server").val();
    var nick = $("#nick").val();
    var port = $("#port").val();
    connect(server, nick, port);
    return false;
}

function joinChannel(elem){
    var channel = $("#channel").val();
    var nick = $("#nick").val();
    join(channel, nick);
    return false;
}

function sendMessage(elem){
    var target = $("#target").val();
    var message = $("#message").val();
    var nick = $("#nick").val();
    
    if(message.length != 0){
        privmsg(target, message, nick);
        $("#message").val("");
    }
    return false;
}

function checkLoader(){
    if(loading == 0){
        loading = 1;
        readMessage(nick);
        
    }
}


function connect(server, nickname, port){
    nick = nickname;
    $.cookie('nick',nickname);
    $.cookie('server',server);
    $.cookie('port',port);
    $.post("/api/CONNECT",
           {
               'server': server,
               "nick" : nick ,
               "port" : port
           },
           function(data){
               eval("obj=" + data);
               $("#status").html(obj["reason"]);
               if(obj["status"].match('^OK$')){
                   $("#connect").toggle();
                   $("#join_channel").append(
                       '<form  id="join_form" onsubmit="javascript:return joinChannel();">' +
                           '<input id="nick" type="hidden" value="' + nick + '"></input>' +
                           'channel<input id="channel" type="text" value="#yushi"></input>' + 
                           '</form>'
                   );
               }
           });
    
}

function join(channel, nick){
    var url = '/api/JOIN';
    $.cookie('channel',channel);
    active_channel = channel;
    $.post(url,
           {
               'channel':channel, //escape(channel),
               'nick' : nick,
           },
           function(data){
               eval("obj=" + data);
               $("#status").html(obj["reason"]);
               if(obj["status"].match('^OK$')){
                   $("#send_message").append(
                       '<form id="message_form" onsubmit="javascript:return sendMessage(this);">' +
                           '<input id="nick" type="hidden" value="' + nick + '"></input>' +
                           '<input id="target" type="hidden" value="' + channel + '"></input>' +
                           'message<input id="message" type="text" ></input>' +
                           '</form>'
                   );
                   $("#join_channel").toggle();
                   $("#head").append(channel);
                   setInterval(checkLoader, 1000);
               }
           });
}

function privmsg(target, message, nick){
    var url = "/api/SPEAK";
    $.post(url,
           {
               'message':message,
               'channel':target, //escape(target),
               'nick':nick,
           },
           function(data){
               $("#messages").append(nick + ":" + message + "<br />");
               window.scrollBy( 0, screen.height );
           });
}

function readMessage(nick){
    var url = "/api/READ";
    $.ajax({
               'type': 'POST',
               'url': url,
               'data': {'nickname':nick},
               cache: false,
               success: function(data){
                   //$("#debug").append(data + "<br />");
                   try{
                       eval("received=" + data);
                       var messages = received[active_channel];
                       for(var i = 0; i < messages.length; i+=2){
                           var match_result = messages[i].match("(.+)!.*");
                           if(match_result){
                               $("#messages").append(match_result[1] + ": ");
                               $("#messages").append(messages[i+1] + "<br />");
                           }else{
                               $("#status").html("JOINERS: " + messages[i+1]);
                           }
                       }
                   }catch(e){
                       
                   }
                   loading = 0;
                   window.scrollBy( 0, screen.height );
               },
               error: function(XMLHttpRequest, textStatus, errorThrown){
                   loading = 0;
               }
           });
}

function getopt(){
    var args = new Object();
    var args_string = location.href.split('?')[1];
    if(!args_string){
        return;
    }
    var args_array = args_string.split('&');
    for(var i = 0; i < args_array.length; i++){
        var arg = args_array[i].split("=");
        args[arg[0]] = arg[1];
    }
    return args;
}

function init(){
    args = getopt();
    if($.cookie('server') && $.cookie('port') && $.cookie('nick')){
        connect($.cookie('server'), $.cookie('nick'), $.cookie('port'));
    }
    if($.cookie('channel')){
        join($.cookie('channel'), $.cookie('nick'));
    }
}
init();
