var loading = 0;
var nick = null;
var active_channel = null;

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
    $.get("/api/CONNECT?server=" + server + "&nick=" + nick + "&port=" + port, function(data){
        if(data.match('^OK$')){
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
    var url = '/api/JOIN?channel=' + escape(channel) + "&nick=" + nick;
    active_channel = channel;
    $.get(url, function(data){
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
    });
}

function privmsg(target, message, nick){
    var url = "/api/SPEAK?message=" + message + "&channel=" + escape(target) + "&nick=" + nick;
    $.get(url, function(data){
        $("#messages").append(nick + ":" + message + "<br />");
        window.scrollBy( 0, screen.height );
    });
}

function readMessage(nick){
    var url = "/api/READ?nick=" + nick;
    $.ajax({
        'url': url,
        cache: false,
        success: function(data){
            //$("#debug").append(data + "<br />");
            try{
                eval("received=" + data);
                var messages = received[active_channel];
                for(var i = 0; i < messages.length; i+=2){
                    var displayName = messages[i].match("(.+)!.*")[1];
                    $("#messages").append(displayName + ": ");
                    $("#messages").append(messages[i+1] + "<br />");
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


function init(){
}
init();