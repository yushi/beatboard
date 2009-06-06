function update_status(data) {
    $("p.ret").remove();
    $("div.status").find("p.s").append("<p class=ret style=\"display:none;\">" + data + "</p>");
    $("p.ret").slideDown("slow")
}

function append_signup_result(data) {
    //eval("var jsondata =" + data);
    var jsondata = eval("("+data+")");
    if (jsondata != null)
    {
        if (jsondata["result"] == 3)
        {
            $("#login").toggle();
            update_status("Login Successful<br>" +
                         "publish cookie here.");
            setTimeout( function () {
                window.location.replace("./login.html");
            }, 5000)
        }
        else if (jsondata["result"] == 4)
        {
            update_status("Wrong Username/Email and Password combination");
        }
    }
}

function remove_signup_result() {
    children = $("p.s").children();
    children.each( function(n) {
        $(children[n]).remove();
    });
}

function addbutton_event() {
    $("input.buttonTest").click( function() {
        call_api( function(data) {
            append_signup_result(data);
        });
    });
}

function call_api(func) {
    var name = $("#n").val();
    var pass = $("#p").val();
    if (name.length != 0 && pass.length != 0)
    {
        remove_signup_result();
        $.post("./login.cgi", {"n":name, "p":pass},
               function(data) {
                   func(data);
               });
    }
    else
    {
        update_status("Please try to input the 2 words shown above.");
    }
}

function send_query() {
    call_api( function(data) {
        append_signup_result(data);
    });
}

function main() {
    addbutton_event()
}

main();