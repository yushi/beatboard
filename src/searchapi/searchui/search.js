function append_search_result(data, query) {
    //eval("var jsondata =" + data);
    var jsondata = eval("("+data+")");
    if (jsondata.toString.length == 0)
    {
        var message = "No results for " + query;
        $("div.contents").find("p.s").append("<p class=\"search-result\" >" + message + "</p>");
    }
    else if (jsondata.messages != null)
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
    $("input.searchButton").click( function() {
        call_api( function(data, query) {
            append_search_result(data, query);
        });
    });
}

function call_api(func) {
    var query = $("#q").val();
    var url = "/api/search?q=" + encodeURIComponent(query);
    remove_search_result();
    $.get(url, function(data){
        func(data, query);
    });
}

function main() {
    addbutton_event()
}

main();