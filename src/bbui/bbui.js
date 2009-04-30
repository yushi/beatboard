function connectServer(){
    var server = $("#server").val();
    var nick = $("#nick").val();
    var port = $("#port").val();
    connect(server, nick, port);
    $("#connect").toggle();
    $("body").append(
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
    $("body").append(
        '<form id="message_form" onsubmit="javascript:return sendMessage(this);">' +
            '<input id="nick" type="hidden" value="' + nick + '"></input>' +
            '<input id="target" type="hidden" value="' + channel + '"></input>' +
            'message<input id="message" type="text" ></input>' +
            '</form>'
    );
    return false;
}

function sendMessage(elem){
    var target = $("#target").val();
    var message = $("#message").val();
    var nick = $("#nick").val();
    privmsg(target, message, nick);
    return false;
}


function connect(server, nick, port){
    $.get("./connect.cgi?server=" + server + "&nick=" + nick + "&port=" + port, function(data){
    });
}

function join(channel, nick){
    var url = './join.cgi?channel=' + escape(channel) + "&nick=" + nick;
    $.get(url, function(data){
    });
}

function privmsg(target, message, nick){
    var url = "./privmsg.cgi?message=" + message + "&target=" + escape(target) + "&nick=" + nick;
    $.get(url, function(data){
    });
}

function init(){
}
init();