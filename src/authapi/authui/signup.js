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
        if (jsondata["result"] == 1)
        {
            $("#signup").toggle();
            update_status("Account has been success fully Created.<br>" +
                         "Are you having problems with the login form?<br>" +
                          "Please use this " + 
                          "<a href=\"./login.html\">direct link</a>.");
            setTimeout( function () {
                window.location.replace("./login.html");
            }, 5000)
        }
        else if (jsondata["result"] == 2)
        {
            update_status("Username has already been taken");
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
    var mail = $("#m").val();
    if (name.length != 0 && pass.length != 0 && mail.length != 0)
    {
        remove_signup_result();
        //var url = "./signup.cgi?n=" + name + "&p=" + pass + "&m=" + mail;
        //$.get(url, function(data){
        //    func(data);
        //});
        $.post("./signup.cgi", {"n":name, "p":pass, "m": mail},
               function(data) {
                   func(data);
               });
    }
    else
    {
        update_status("Please try to input the 3 words shown above.");
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