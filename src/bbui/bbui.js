var loading = 0;
var nick = null;
var active_channel = null;
var noexec = 0;
var debug = 0;
var no_refferer = 1;
var focused = 1;
var new_message = 0;
var notify_index = 0;
var background_color = '#ddeedd';
var message_background_color = 'white';
var highlight_color = '#bbeebb';
var notify_color = '#eebbbb';
var font_color = 'black';

$.ajaxSetup({'timeout': 0});

function connectServer(){
    var server = $('#server').val();
    var nick = $('#nick').val();
    var port = $('#port').val();
    connect(server, nick, port);
    return false;
}

function joinChannel(elem){
    var channel = $('#channel').val();
    join(channel, nick);
    return false;
}

function sendMessage(elem){
    var message = $('#message').val();
    
    if(message.length != 0){
        if(privmsg(active_channel, message, nick)){
            $('#message').val('');
        }
    }
    return false;
}

function updateNotifyTitle(){
    var notify_message = ['new', 'n e w'];
    $("title").text(notify_message[notify_index]);
    notify_index = notify_index ? 0 : 1;
}
function checkLoader(){
    if(loading == 0){
        loading = 1;
        readMessage(nick);
        
    }
    if(new_message){
        updateNotifyTitle();
    }else{
        $("title").text("BeatBoard");
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
               }
               if(obj['users']){
                   for(var channel in obj['users']){
                       createChannelUI(channel, 1);
                       for(var i = 0; i < obj['users'][channel].length; i++){
                           $('#\\' + channel + '_users').append('<div class="user">' + obj['users'][channel][i] + '</div>');
                       }
                   }
               }
               $('#send_message').load('send_message.html',null,function(){$('#message').focus();});
	       
               setInterval(checkLoader, 1000);
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
               }
               createChannelUI(active_channel, 1);
           });
}

function privmsg(target, message, nick){
    var url = '/api/SPEAK';
    if(target == null){
        return;
    }
    debug_log('privmsg req');
    $.post(url,
           {
               'message':message,
               'channel':target, //escape(target),
               'nick':nick,
           },
           function(data){
               debug_log('privmsg res');
               addMessage(nick, active_channel,message);
               window.scrollBy( 0, screen.height );
           });
    return true;
}

function readSuccess(data){
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
    loading = 0;
    window.scrollBy( 0, screen.height );
    update_debuginfo('read success');
}

function readMessage(nick){
    var url = '/api/READ';
    debug_log('read req');
    $.ajax({
               'type': 'POST',
               'url': url,
               'data': {'nickname':nick},
               cache: false,
               success: readSuccess,
               error: function(XMLHttpRequest, textStatus, errorThrown){
                   debug_log('read response error');
                   loading = 0;
               }
           });
}

function addUstreamEmbedTag(room, channel){
    var url = '/api/tp/ust/json/channel/' + room + '/getEmbedTag?key=AD8032366E40D6D4BFA76066C699D32C';
    debug_log('ust embed req');
    $.ajax({
               'type': 'GET',
               'url': url,
               cache: false,
               success: function(data){
		 eval('received=' + data);
		 $('#messagebox > #\\' + channel).append(received['results'] + '<br /><br />');
	       },
               error: function(XMLHttpRequest, textStatus, errorThrown){
                   debug_log('ust embed tag response error');
                   loading = 0;
               }
           });
}

function addYoutubeEmbedTag(videoId, channel){
  var tag = '<object width="200" height="150"><param name="movie" value="http://www.youtube.com/v/' + videoId + '"></param><param name="wmode" value="transparent"></param><embed src="http://www.youtube.com/v/' + videoId + '" type="application/x-shockwave-flash" wmode="transparent" width="200" height="150"></embed></object><br /><br />';
  $('#messagebox > #\\' + channel).append(tag);
}

function addImgEmbedTag(url, channel){
  var tag = '<img src=' + url + ' /><br />';
  $('#messagebox > #\\' + channel).append(tag);
}



function extractLink(str, channel){
    var ustRegex = new RegExp("");
    ustRegex.compile(/https?:\/\/www\.ustream\.tv\/channel\/(\S+)/);
    var youtubeRegex = new RegExp("");
    youtubeRegex.compile(/https?:\/\/www\.youtube\.com\/watch\S+v=(\S+)&?/);
    var imgRegex = new RegExp("");
    imgRegex.compile(/https?:\/\/\S+\.(jpe?g|png|gif|bmp)/);

    var urlRegex = new RegExp("");
    urlRegex.compile(/https?:\/\/\S+/);
    var match_result = str.match(urlRegex);
    if(match_result){
	var ustChannel = null;
	if(ustChannel = str.match(ustRegex)){
	  addUstreamEmbedTag(ustChannel[1], channel);
	}
	var youtubeVideoId = null;
	if(youtubeVideoId = str.match(youtubeRegex)){
	  addYoutubeEmbedTag(youtubeVideoId[1], channel);
	}
	var imgURL = null;
        if(imgURL = str.match(imgRegex)){
          addImgEmbedTag(imgURL[0], channel);
        }

        if(no_refferer){
            //IE not supported
	    var html = '<html><head><script type="text/javascript"><!--\n'
	        + 'document.write(\'<meta http-equiv="refresh" content="0;url='+match_result[0]+'">\');'
	        + '// --><'+'/script></head><body></body></html>';
            str = str.replace(urlRegex, "<a target=\"_blank\" href=\"data:text/html;charset=utf-8,'" +encodeURIComponent(html) + "\" >" + match_result[0] + "</a>");
        }else{
            str = str.replace(urlRegex, "<a target=\"_blank\" href=\"" + match_result[0] + "\" >" + match_result[0] + "</a>");
        }
    }
    return str;
}

function addChannel(channel){
    
}

function selectChannel(channel){
    active_channel = channel;
    var channel_divs = $('#messagebox > *');
    for( var i = 0; i < channel_divs.length; i++){
        if( channel_divs[i].id != channel ){
            $(channel_divs[i]).hide();
            $($('#channels > #\\' + channel_divs[i].id)[0]).css('background-color',background_color);
        }else{
            $(channel_divs[i]).show();
            $($('#channels > #\\' + channel_divs[i].id)[0]).css('background-color',highlight_color);
        }
    }
}

function createChannelUI(channel, active){
    if(!$('#messagebox > #\\' + channel)[0]){
        $('#messagebox').prepend('<div id="' + channel + '" ><div class="users" id="' + channel + '_users"></div></div>');
        $('#channels').prepend('<span id="' + 
                               channel + 
                               '" onclick="javascript:selectChannel(\'' + 
                               channel + 
                               '\')" >' + 
                               channel + 
                               '</span>');
        if(!active){
            $('#messagebox > #\\' + channel).hide();
        }else{
            selectChannel(channel);
        }
    }
}

function zeroPadding(num){
    var ret = '';
    if(num < 10){
        ret += '0' + num;
    }else{
        ret += num;
    }
    return ret;
}
function getCurrentTime(){
    var now = new Date();
    return (1900 + now.getYear()) + 
        '/' + 
        zeroPadding(now.getMonth()  + 1 ) + 
        '/' + 
        zeroPadding(now.getDate())  + 
        ' ' + 
        zeroPadding(now.getHours()) + 
        ':' + 
        zeroPadding(now.getMinutes());
}

function toggleTime(elem, flag){
    var color = flag ? font_color : message_background_color;
    $(elem.childNodes[1]).css('color', color);
}
function addMessage(speaker, channel, message){
    var escaped_nick = replace_centity_ref(speaker);
    var escaped_message = replace_centity_ref(message);
    createChannelUI(channel);
    $('#messagebox > #\\' + channel).append( 
        '<div id="line" onmouseover="javascript:toggleTime(this, 1)" onmouseout="javascript:toggleTime(this, 0)">' + 
            '<div id="usermessage">' + 
	'<span id="speaker">' + escaped_nick + '</span>: ' + 
	extractLink(escaped_message, channel) + 
            '</div><div id="time">' + 
            getCurrentTime() + '</div>');
    
    if(channel != active_channel){
        $($('#channels > #\\' + channel)[0]).css('background-color',notify_color);
    }

    if(!focused){
        new_message = 1;
    }
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
