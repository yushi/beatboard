var loading = 0;
var nick = null;

function connectServer(){
    var server = $("#server").val();
    var nick = $("#nick").val();
    var port = $("#port").val();
    connect(server, nick, port);
    $("#connect").toggle();
    $("#join_channel").append(
        '<form  id="join_form" onsubmit="javascript:return joinChannel();">' +
            '<input id="nick" type="hidden" value="' + nick + '"></input>' +
            'channel<input id="channel" type="text" value="#yushi"></input>' + 
            '</form>'
    );

    return false;
}
    
function joinChannel(elem){
    var channel = $("#channel").val();
    var nick = $("#nick").val();
    join(channel, nick);
    $("#send_message").append(
        '<form id="message_form" onsubmit="javascript:return sendMessage(this);">' +
            '<input id="nick" type="hidden" value="' + nick + '"></input>' +
            '<input id="target" type="hidden" value="' + channel + '"></input>' +
            'message<input id="message" type="text" ></input>' +
            '</form>'
    );
    $("#join_channel").toggle();
    $("#head").append(channel);
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
        readMessage(nick);
        loading = 1;
    }
}


function connect(server, nickname, port){
    nick = nickname;
    $.get("/api/CONNECT?server=" + server + "&nick=" + nick + "&port=" + port, function(data){
        //alert(data);
        setInterval(checkLoader, 1000);
    });
    
}

function join(channel, nick){
    var url = '/api/JOIN?channel=' + escape(channel) + "&nick=" + nick;
    $.get(url, function(data){
        //alert(data);
    });
}

function privmsg(target, message, nick){
    var url = "/api/SPEAK?message=" + message + "&channel=" + escape(target) + "&nick=" + nick;
    $.get(url, function(data){
        $("#messages").append(nick + ":" + message + "<br />");
    });
}

function readMessage(nick){
    var url = "/api/READ?nick=" + nick;
    $.get(url, function(data){
        alert(data);
        eval("received=" + data);
        //alert(received);
        //$("#messages").append(data + "<br />");
        $("#messages").append(received["#yushi"] + "<br />");
        loading = 0;
    });
}


function init(){
}
init();