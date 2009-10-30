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


init();
