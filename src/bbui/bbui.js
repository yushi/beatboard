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
var cookie_expire = new Date();
var scrollPosition = {};
var privmsgQueue = [];
var privmsgSending = 0;
var video_seq_num = 0;
cookie_expire.setDate( cookie_expire.getDate()+7 );

$.ajaxSetup({'timeout': 1000 * 60 * 3} ); // 3 minutes

function connectServer(){
    var server = $('#server').val();
    var nick = $('#nick').val();
    var port = $('#port').val();
    BBAPI.connect(server, nick, port);
    return false;
}

function joinChannel(elem){
    var channel = $('#channel').val();
    BBAPI.join(channel, nick);
    return false;
}

function sendMessage(ev){
    var shift = typeof ev.modifiers == 'undefined' ? ev.shiftKey : ev.modifiers & Event.SHIFT_MASK;
    if(shift || ev.keyCode != 13){
        return false;
    }
    var message_str = $('#message').val();
    
    if(message_str.length != 0){
        var messages = message_str.split("\n");
        for(var i in messages){
            if(messages[i] != ''){
                addPrivmsg(active_channel, messages[i], nick);                            
            }
        }
        if(!privmsgSending){
            privmsgFromQueue(1);            
        }
        $('#message').val('');
    }
    return true;
}

function updateNotifyTitle(){
    var notify_message = ['new', 'n e w'];
    $("title").text(notify_message[notify_index]);
    notify_index = notify_index ? 0 : 1;
}

function checkLoader(){
    BBAPI.readMessage(nick, function(){
                          var rColumnHight = document.getElementById(active_channel+"_users").style.height;
                          rColumnHight = rColumnHight+10;
                      });

    if(new_message){
        updateNotifyTitle();
    }else{
        $("title").text("BeatBoard");
    }
    update_debuginfo('checkLoader');
}

function addPrivmsg(target, message, nick){
    privmsgQueue.push([target, message, nick]);
}

function privmsgFromQueue(nowait){
    if(!privmsgSending){
        privmsgSending = 1;        
    }

    var wait_time = 2000; // 2sec
    if(nowait){
        wait_time = 0;
    }
    var next = privmsgQueue.shift();
    if(!next){
        privmsgSending = 0;
        return;
    }
    var target = next[0];
    var message = next[1];
    var nick = next[2];

    setTimeout(function(){BBAPI.privmsg(target, message, nick, privmsgFromQueue)},wait_time);
}

function addObjectEmbedTag(channel, objectTag){
    var tag = '<div id="video_container" >'
        + '<div id="video_bar" onmouseout="javascript:setParentToDisdraggable(this);" onmouseover="javascript:setParentToDraggable(this);" >'
        + '<input type="checkbox" onclick="javascript:setParentToggleFixed(this)"/>'
        + '<input type="checkbox" onclick="javascript:setObjectToggleVisible(this)"/>'
        + '</div>'
        + objectTag
        + '</div>'
        + '<br /><br />';

    $('#messagebox > #\\' + channel).append(tag);

}

function addUstreamEmbedTag(room, channel){
    var url = '/api/tp/ust/json/channel/' + room + '/getEmbedTag';
    debug_log('ust embed req');
    $.ajax({
               'type': 'GET',
               'url': url,
               cache: false,
               success: function(data){
                   eval('received=' + data);
                   addObjectEmbedTag(channel, received['results']);
               },
               error: function(XMLHttpRequest, textStatus, errorThrown){
                   debug_log('ust embed tag response error');
               }
           });
}

function addYoutubeEmbedTag(videoId, channel){
    var objectTag = '<object id="video" width="320" height="260">'
        + '<param name="movie" value="http://www.youtube.com/v/'
        + videoId + '" />'
        + '<param name="wmode" value="transparent" />'
        + '<embed src="http://www.youtube.com/v/'
        + videoId 
        + '" type="application/x-shockwave-flash" wmode="transparent" width="320" height="260" />'
        + '</object>';
    addObjectEmbedTag(channel, objectTag);
}

function addNico2EmbedTag(videoId, channel){
    var objectTag = '<div id="video_' + ++video_seq_num + '"></div><script type="text/javascript" src="http://ext.nicovideo.jp/thumb_watch/sm'
        + videoId 
        + '"></script>';
    document.write_org = document.write;
    document.write = function(arg){document.getElementById('video_' + video_seq_num ).innerHTML = arg;};
    addObjectEmbedTag(channel, objectTag);
}

function setParentToDraggable(elem){
    if($($(elem).parent().get(0)).css('position') == 'fixed'){
	$($(elem).parent().get(0)).draggable().draggable('enable');
    }
}

function setParentToDisdraggable(elem){
    if($($(elem).parent().get(0)).css('position') == 'fixed'){
	$($(elem).parent().get(0)).draggable().draggable('disable');
    }
}

function setParentToggleFixed(elem){
    var target = $($($(elem).parent().get(0)).parent().get(0));
    target.fadeOut(1000, function(){
	    if(target.css('position') != 'fixed'){
		target.css('left','100px');
		target.css('top','100px');
		target.css('position', 'fixed');
	    }else{
		target.css('position', 'static');
	    }
	    target.fadeIn(1000);	    
	});
    
}

function setObjectToggleVisible(elem){
    var target = $($(elem).parent().parent().children().get(1));
    target.toggle();
}

function addImgEmbedTag(url, channel){
  var tag = '<img src=' + url + ' /><br />';
  $('#messagebox > #\\' + channel).append(tag);
}



function extractLink(str, channel){
    var videoExtractRules = [
        ["http://www.nicovideo.jp/watch/sm(\\d+)", addNico2EmbedTag],
        ["https?://www.ustream.tv/channel/(\\S+)", addUstreamEmbedTag],
        ["https?://www.youtube.com/watch\\S+v=(\\S+)&?", addYoutubeEmbedTag],
    ];

    var imgRegex = new RegExp("");
    imgRegex.compile(/https?:\/\/\S+\.(jpe?g|png|gif|bmp)/);

    var urlRegex = new RegExp("");
    urlRegex.compile(/https?:\/\/[0-9a-zA-Z\-\+\/\;\!\*\(\),\.\_\?=]+/);
    var match_result = str.match(urlRegex);
    if(match_result){
        //extract videos
        for(var i = 0; i < videoExtractRules.length; i++){
            var videoMatchResult = null;
            var regexp = new RegExp(videoExtractRules[i][0]);
            var videoEmbedFunction = videoExtractRules[i][1];
            if(videoMatchResult = str.match(regexp)){
                videoEmbedFunction(videoMatchResult[1], channel);                
            }
        }

        //extract image
	var imgURL = null;
        if(imgURL = str.match(imgRegex)){
            addImgEmbedTag(imgURL[0], channel);
        }

        //extract link
        if(no_refferer){
            //IE not supported
	    var html = '<html><head><script type="text/javascript"><!--\n'
	        + 'document.write(\'<meta http-equiv="refresh" content="0;url='+match_result[0]+'">\');'
	        + '// --><'+'/script></head><body></body></html>';
            str = str.replace(
                urlRegex,
                "<a target=\"_blank\" href=\"data:text/html;charset=utf-8,'" 
                    + encodeURIComponent(html) 
                    + "\" >" 
                    + match_result[0] 
                    + "</a>"
            );
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
            scrollPosition[channel_divs[i].id] = document.body.scrollTop;
            $(channel_divs[i]).hide();
            $('#\\' + channel_divs[i].id + '_tab').css('background-color',background_color);
        }else{
            $(channel_divs[i]).show();
            window.scrollBy( 0, scrollPosition[channel] );
            $('#\\' + channel_divs[i].id + '_tab').css('background-color',highlight_color);
        }
    }
    $('#message').focus();
}

function createChannelUI(channel, active){
    if(!$('#messagebox > #\\' + channel)[0]){
        $('#messagebox').prepend('<div id="' + channel + '" ><div class="users" id="' + channel + '_users"></div></div>');
        $('#channels').prepend('<span id="' + 
                               channel + '_tab' +
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

function getTimeStrFromSearchResult(str){
    var ret = str.substr(0,(str.length - 3));
    ret.replace(/-/g,'/');
    return ret;
}

function getCurrentTimeStr(){
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

function addMessage(speaker, channel, message, time){
    var isOld = true;
    var isSequencial = false;
    
    if(time == undefined){
        time = getCurrentTimeStr();
        isOld = false;
    }
    
    if( getLastMessageSpeaker(channel) == speaker){
        isSequencial = true;
    }
    
    var escaped_nick = isSequencial ? '' : replace_centity_ref(speaker);
    var escaped_message = replace_centity_ref(message);
    createChannelUI(channel);
    $('#messagebox > #\\' + channel).append( 
        '<div id="' + (isOld ? 'oldline' : 'line') + '" onmouseover="javascript:toggleTime(this, 1)" onmouseout="javascript:toggleTime(this, 0)">' + 
            '<div id="usermessage">' + 
	    '<span id="speaker">' + escaped_nick + '</span>' + 
            '<span id="leftspace" />' + 
	    (isSequencial ? extractLink(escaped_message, channel) : '')+ 
            '</div><div id="time">' + 
            time + '</div>');
    
    if(channel != active_channel){
        $($('#\\' + channel + '_tab')[0]).css('background-color',notify_color);
    }
    
    if(!focused){
        new_message = 1;
    }
    
    if(!isSequencial){
        addMessage(speaker, channel, message, isOld ? time:undefined);
    }
}

function getLastMessageSpeaker(channel){
    var channel = $('#messagebox > #\\' + channel);
    if(!channel){
        return;
    }
    
    var index = channel.children().size() - 1;
    if(index < 1){
        return;
    }
    
    var nick = null;
    while(!nick){
        nick = $(channel.children().eq(index--)).children().eq(0).children().eq(0).text();
        // replace nbst(160 = \xa0) to SPACE(32)
        nick = nick.replace(RegExp('\xa0', 'g'), ' ');
    }
    return nick;
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
    centity_ref_array.push(new Array(RegExp(' ', 'g'), '&nbsp;'));
    for(var i = 0; i < centity_ref_array.length; i++) {
        message = message.replace(centity_ref_array[i][0],
                                  centity_ref_array[i][1]);
    }
    return message;
}

function update_debuginfo(from){
    if(debug){
        var debug_text = '';
        debug_text += 'loading = ' + BBAPI.isLoading() + '<br />';
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
    nick = null;
    location.reload();
}
