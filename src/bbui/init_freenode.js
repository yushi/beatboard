function init(){
    var args = getopt();
    $(window).blur(function(){ focused = 0 });
    $(window).focus(function(){ focused = 1; new_message = 0; $('#message').focus()});
    if(args['debug']){
        debug = new Object();
    }else{
        $('#debug').hide();
        $('#debug_menu').hide();
        $('#debug_log').hide();
    }
    
    if(!$.cookie('nick')){
        nick = prompt('please enter your nickname');        
    }else{
        nick = $.cookie('nick');
    }

    connect("irc.freenode.net", nick, "6667");

    var requested = location.pathname;
    var channel = null;
    var channelRegex = new RegExp("");
    channelRegex.compile(/^\/(.*)\//);
    var channelResult  = requested.match(channelRegex);
    if(channelResult){
        channel = channelResult[1];
    }


    if(channel){
        joinWait("#" + channel, nick);
    }


    update_debuginfo('init');
    debug_log('init');
}

function joinWait(channel, nick){
    if(!isUIBlocking){
        join(channel, nick);
    }else{
        setTimeout(function(){
                       joinWait(channel, nick)
                   },
                   1000);
    }
    
}

init();
