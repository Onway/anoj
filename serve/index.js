$(document).ready(function() {
    $("#submit").click(function() {
        data = $("#left form").serialize();
        $.ajax({
            type: "post",
            url: "cgi-bin/judger.py",
            data: data,
            success: function(ret) {
                alert("Submit Succeed");
            }
        })
        return false;
    });

    $("#right button").click(function() {
        $.ajax({
            type: "get",
            url: "cgi-bin/refresh.py",
            success: function(ret) {
                $("#rid").html(ret["rid"]);
                $("#pid").html(ret["pid"]);
                $("#codelen").html(ret["codelen"]);
                $("#debug").val(ret["debug"]);
                $("#result").html(ret["result"]);
                $("#time").html(ret["time"]);
                $("#memory").html(ret["momory"]);
                $("#msg").val(ret["msg"]);
            }
        });
        return false;
    });

    $("#inc_rid").click(function() {
        var cur = new Number($("[name=rid]").val());
        $("[name=rid]").val(cur + 1)
        return false;
    });

    $("#inc_pid").click(function() {
        var cur = new Number($("[name=pid]").val());
        $("[name=pid]").val(cur + 1)
        return false;
    });

    $("#dec_pid").click(function() {
        var cur = new Number($("[name=pid]").val());
        $("[name=pid]").val(cur - 1)
        return false;
    });

});

