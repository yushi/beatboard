function addbutton_event() {
    $("input.buttonTest").click( function() {
        call_api( function(data) {
            alert(data);
        });
    });
}

function call_api(func) {
    var test = $("#test").val();
    var url = "./samplerpc.cgi?test=" + test;
    $.get(url, function(data){
        func(data);
    });
}

function send_query() {
    call_api( function(data) {
        alert(data);
    });
}

function main() {
    addbutton_event()
}

main();