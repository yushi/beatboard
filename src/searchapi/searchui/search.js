function append_search_result(data) {
    //eval("var jsondata =" + data);
    var jsondata = eval("("+data+")");
    if (jsondata.messages != null)
    {
        for (i = 0; i < jsondata.messages.length; i++) {
            var classid = "m" + i;
            $("div.contents").find("p.s").append("<p class=\"" + classid + "\" style=\"display:none;\">" + jsondata.messages[i] + "</p>");
            $("p." + classid).slideDown("slow")
        }
    }
}

function remove_search_result() {
    children = $("p.s").children();
    children.each( function(n) {
        $(children[n]).remove();
    });
}

function addbutton_event() {    
    $("input.buttonTest").click( function() {
        call_api( function(data) {
            append_search_result(data);
        });
    });
}

function call_api(func) {
    var test = $("#q").val();
    var url = "/api/search?q=" + encodeURIComponent(test);
    remove_search_result();
    $.get(url, function(data){
        func(data);
    });
}

function send_query() {
    call_api( function(data) {
        append_search_result(data);
    });
}

function main() {
    addbutton_event()
}

main();