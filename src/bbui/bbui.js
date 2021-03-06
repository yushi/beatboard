var nick = null;
var active_channel = null;
var noexec = 0;
var debug = 0;
var no_refferer = 1;
var focused = 1;
var new_message = 0;
var notify_index = 0;
var message_count = 0;
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
setInterval(notifyNewMessage, 1000);

function connectServer(){
    var server = $('#server').val();
    var nick = $('#nick').val();
    var port = $('#port').val();
    var pass = $('#pass').val();
    BBAPI.connect(server, nick, port, pass);
    return false;
}

function joinChannel(elem){
    var channel = $('#channel').val();
    BBAPI.join(channel, nick);
    return false;
}

function sendMessage(){
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

function sendMessageByKeyEvent(ev){
    var shift = typeof ev.modifiers == 'undefined' ? ev.shiftKey : ev.modifiers & Event.SHIFT_MASK;
    if(shift || ev.keyCode != 13){
        return false;
    }
    return sendMessage();
}

function updateNotifyTitle(){
    var notify_message = ['new', 'n e w'];
    $("title").text("("+message_count+") "+notify_message[notify_index]);
    notify_index = notify_index ? 0 : 1;
}

function checkLoader(){
    BBAPI.readMessage(nick, function(){
                          var rColumnHight = document.getElementById(active_channel+"_users").style.height;
                          rColumnHight = rColumnHight+10;
                      });
    update_debuginfo('checkLoader');
}

function notifyNewMessage(){
    if(new_message){
        updateNotifyTitle();
    }else{
        $("title").text("BeatBoard");
        message_count = 0;
    }
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
    var objectTag = '<div id="video_' + ++video_seq_num + '"></div><script type="text/javascript" src="http://ext.nicovideo.jp/thumb_watch/'
        + videoId 
        + '"></script>';
    document.write_org = document.write;
    document.write = function(arg){document.getElementById('video_' + video_seq_num ).innerHTML = arg;};
    addObjectEmbedTag(channel, objectTag);
}

function addTwitterEmbedTag(tweetId, channel) {
    var url = '/api/tp/twitter/1/statuses/show.json?id=' + tweetId;
    debug_log('twitter embed req');
    $.ajax({
        'type': 'GET',
        'url': url,
        cache: false,
        success: function(data){
            eval('tweet=' + data);
            var tweet_data = {
                id : tweet.id,
                screen_name : tweet.user.screen_name,
                name : tweet.user.name,
                background_url : tweet.user.profile_background_image_url,
                profile_url : tweet.user.profile_image_url,
                source : tweet.source,
                timestamp : tweet.created_at.substr(0,tweet.created_at.indexOf('+')),
                content : tweet.text,
                profile_background_color : tweet.user.profile_background_color,
                permalink : "http://twitter.com/" + tweet.user.screen_name + "/status/" + tweet.id
            };
            var textbody = "<style type='text/css'>.bbpBox{background:url({background_url})#{profile_background_color};padding:20px;}</style>"
              + "<div id='tweet_{id}' class='bbpBox' style='background:url({background_url})#{profile_background_color};padding:20px;'>"
              + "<p class='bbpTweet' style='background:#fff;padding:10px 12px 10px 12px;margin:0;min-height:48px;color:#000;font-size:16px !important;line-height:22px;-moz-border-radius:5px;-webkit-border-radius:5px;'>"
              + "{content}<span class='{timestamp}' style='font-size:12px;display:block;'><a title='{timestamp}'"
              + "href='{permalink}'>{timestamp}</a> via {source}</span>"
              + "<span class='metadata' style='display:block;width:100%;clear:both;margin-top:8px;padding-top:12px;height:40px;border-top:1px solid #fff;border-top:1px solid #e6e6e6;'>"
              + "<span class='author' style='line-height:19px;'><a href='http://twitter.com/{screen_name}'>"
              + "<img src='{profile_url}' style='float:left;margin:0 7px 0 0px;width:38px;height:38px;' />"
              + "</a><strong><a href='http://twitter.com/{screen_name}'>{name}</a></strong><br/>{screen_name}"
              + "</span></span></p></div>";

            textbody = textbody.replace(/{[^{}]+}/g, function(key){
                return tweet_data[key.replace(/[{}]+/g, "")] || "";
            });
            $('#messagebox > #\\' + channel).append(textbody);
      },
      error: function(XMLHttpRequest, textStatus, errorThrown){
        debug_log('twitter embed tag response error');
      }
    });
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
        ["http://www.nicovideo.jp/watch/((?:\\S+)?\\d+)", addNico2EmbedTag],
        ["https?://www.ustream.tv/channel/(\\S+)", addUstreamEmbedTag],
        ["https?://www.youtube.com/watch\\S+v=(\\S+)&?", addYoutubeEmbedTag],
    ];

    var imgRegex = new RegExp("");
    imgRegex.compile(/https?:\/\/\S+\.(jpe?g|png|gif|bmp)/);

    var twitterRegex = new RegExp("");
    twitterRegex.compile(/^http(s)*:\/\/twitter\.com\/(\w+)\/status(es)*\/(\d+)$/i);
    var twitterRegexSharpBang = new RegExp("");
    twitterRegexSharpBang.compile(/^http(s)*:\/\/twitter\.com\/(\#\!)\/(\w+)\/status(es)*\/(\d+)$/i);

    var urlRegex = new RegExp("");
    urlRegex.compile(/(https?:\/\/(?:(?!\&nbsp).)*)(.*)/);
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

        //extract twitter
        var twitterURL = null;
        if(twitterURL = str.match(twitterRegex)){
            addTwitterEmbedTag(twitterURL[twitterURL.length-1], channel);
        } else if (twitterURL = str.match(twitterRegexSharpBang)){
            addTwitterEmbedTag(twitterURL[twitterURL.length-1], channel);
        }

        //extract link
        if(no_refferer){
            //IE not supported
	        var html = '<html><head><script type="text/javascript"><!--\n'
	            + 'document.write(\'<meta http-equiv="refresh" content="0;url='+match_result[1]+'">\');'
	            + '// --><'+'/script></head><body></body></html>';
            str = str.replace(
                urlRegex,
                "<a target=\"_blank\" href=\"data:text/html;charset=utf-8,'" 
                    + encodeURIComponent(html) 
                    + "\" >" 
                    + match_result[1] 
                    + "</a>"
                    + match_result[2]
            );
        }else{
            str = str.replace(urlRegex, "<a target=\"_blank\" href=\"" + match_result[1] + "\" >" + match_result[1] + "</a>" + match_result[2]);
        }
    }
    return str;
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
}

function selectNextChannel(){
    var channels = $("#channels").children();
    for(var i = 0; i < channels.length - 1; i++){
        if(channels[i].innerHTML == active_channel){
            selectChannel(channels[i+1].innerHTML);
            return;
        }
    }
    selectChannel(channels[0].innerHTML);
}

function selectPrevChannel(){
    var channels = $("#channels").children();
    for(var i = channels.length - 1; i > 0; i--){
        if(channels[i].innerHTML == active_channel){
            selectChannel(channels[i-1].innerHTML);
            return;
        }
    }
    selectChannel(channels[channels.length-1].innerHTML);
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

function toggleMessageInfo(elem, flag){
    var color = flag ? font_color : message_background_color;
    $(elem.childNodes[1]).css('color', color); // for time color
    $(elem.childNodes[2]).css('color', color); // for reply button color
}

function replyMessage(elem){
    PREFIX = '> ';
    if(elem.parentNode.childNodes[0].innerText == undefined){
        $('#message').val(PREFIX + elem.parentNode.childNodes[0].textContent);
    }else{
        $('#message').val(PREFIX + elem.parentNode.childNodes[0].innerText);
    }
    $('#message').focus();
}

function desktopNotifySetup(){
    if(!window.webkitNotifications){
        return;
    }

    if (window.webkitNotifications.checkPermission() == 1){
        //not permitted
        window.webkitNotifications.requestPermission();
        
    }
}

function desktopNotifyMessage(speaker, message){
    if(!window.webkitNotifications){
        return;
    }

    if (window.webkitNotifications.checkPermission() == 0){
        var notify = window.webkitNotifications.createNotification("./favicon.ico", speaker, message);
        notify.show();
        setTimeout(function(){
                       notify.cancel();
                   },
                   3000);
    }
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
        '<div id="' + (isOld ? 'oldline' : 'line') + '" onmouseover="javascript:toggleMessageInfo(this, 1)" onmouseout="javascript:toggleMessageInfo(this, 0)">' + 
            '<span id="usermessage">' + 
	        '<span id="speaker">' + escaped_nick + '</span>' + 
            '<span id="leftspace"></span>' + 
	        (isSequencial ? extractLink(escaped_message, channel) : '')+ 
            '</span><span id="time">' + 
            time + '</span>' + 
            '<span style="color: white;" id="reply"	onclick="javascript:replyMessage(this)">reply</span><div>');
    
    if(channel != active_channel){
        $($('#\\' + channel + '_tab')[0]).css('background-color',notify_color);
    }
    
    if(!focused){
        new_message = 1;
        if(isSequencial){
            message_count += 1;
        }
    }

    if(!isSequencial){
        addMessage(speaker, channel, message, isOld ? time:undefined);
    }else{
        if(!isOld){
            desktopNotifyMessage(speaker, message);            
        }
    }
}

function getLastMessageSpeaker(channel){
    var channel = $('#messagebox > #\\' + channel);
    if(!channel){
        return null;
    }
    
    var index = channel.children().size() - 1;
    if(index < 1){
        return null;
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
    var centity_ref_array = [
        ['&', '&amp;'], 
        ['\'', '&apos;'], 
        ['"', '&quot;'], 
        ['<', '&lt;'], 
        ['>', '&gt;'], 
        [' ', '&nbsp;']
    ];
    
    for(var i = 0; i < centity_ref_array.length; i++) {
        message = message.replace(RegExp(centity_ref_array[i][0],'g'), 
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
    $('#debug_log').scrollTop($('#debug_log').attr('scrollHeight'));
    return;
}

function delete_cookie(){
    $.cookie('server',null);
    $.cookie('nick', null);
    $.cookie('port', null);
    $.cookie('channel', null);
    $.cookie('pass', null);
    nick = null;
    location.reload();
}

function keyUpHandler(event){
    var ret = true;

    var KEY_ESC = 27;
    var KEY_LEFT = 37;
    var KEY_UP = 38;
    var KEY_RIGHT = 39;
    var KEY_DOWN = 40;
    var KEY_I = 73;

    //alert(event);
    if(document.activeElement == $("#message").get()[0]){
        if(event.keyCode == KEY_ESC){
            $("#message").blur();
        }
        return ret;
    }
    if(document.activeElement == $("#channel").get()[0]){
        if(event.keyCode == KEY_ESC){
            $("#channel").blur();
        }
        return ret;
    }
    if(event.keyCode == KEY_I){
        $("#message").focus();
        ret = false;
    }
    if(event.keyCode == KEY_RIGHT){
        selectNextChannel();
    }
    if(event.keyCode == KEY_LEFT){
        selectPrevChannel();
    }
    return ret;
}

function keyPressHandler(event){
    var CHARCODE_SLASH = 63;
    var ret = true;
    if(document.activeElement == $("#message").get()[0]){
        return ret;
    }
    if(document.activeElement == $("#channel").get()[0]){
        return ret;
    }

    // for "?" key only capture in press event. (firefox)
    if(event.shiftKey){
        if(event.charCode == CHARCODE_SLASH){
            var help = [
                "i:\t\tfocus message textarea", 
                "right:\tselect next channel", 
                "left:\t\tselect prev channel", 
                "?:\t\tprint this help", 
                "ESC:\t\tunfocus(in textarea only)"
            ];
            alert(help.join("\n"));
        }
    }
    return ret;
}

$(document).keyup(keyUpHandler);
$(document).keypress(keyPressHandler);
