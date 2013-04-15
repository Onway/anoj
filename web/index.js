$(document).ready(function() {
    $("#submit").click(function() {
        $(this).attr("disabled", true);
        $(this).html("waiting");
        data = $("#left form").serialize();
        $.ajax({
            type: "post",
            url: "cgi-bin/judger.py",
            data: data,
            success: function(ret) {
                $("#submit").removeAttr("disabled");
                $("#submit").html("Submit");
            }
        })
        return false;
    });

    $("#right button").click(function() {
        url = "cgi-bin/refresh.py?rid=" + $("[name=rid]").val()
        $.ajax({
            type: "get",
            url: url,
            success: function(ret) {
                $("#rid").html(ret["rid"]);
                $("#pid").html(ret["pid"]);
                $("#codelen").html(ret["codelen"]);
                $("#debug").val(ret["debug"]);
                $("#result").html(ret["result"]);
                $("#time").html(ret["time"]);
                $("#memory").html(ret["memory"]);
                $("#msg").val(ret["msg"]);
                //$("#new_rid").click();
            }
        });
        return false;
    });

    $("#new_rid").click(function() {
        $.ajax({
            type: "get",
            url: "cgi-bin/refresh.py",
            success: function(ret) {
                $("[name=rid]").val(Number(ret) + 1);
            },
            error: function() {
                alert("Error");
            }
        });
        return false;
    });

    $("#dec_rid").click(function() {
        var cur = Number($("[name=rid]").val());
        $("[name=rid]").val(cur - 1);
        $("#right button").click();

        url = "cgi-bin/result.py?action=q&rid=" + (cur - 1);
        $.ajax({
            type: "get",
            url: url,
            success: function(ret) {
                $("[name=rid]").val(ret["rid"]);
                $("[name=pid]").val(ret["pid"]);
                $("[name=time]").val(ret["time"]);
                $("[name=memory]").val(ret["memory"]);
                $("[name=outsize]").val(ret["outsize"]);
                $("[name=lang]").val(ret["lang"]);
                $("[name=code]").val(ret["code"]);
            }
        });
        return false;
    });

    $("#inc_rid").click(function() {
        var cur = Number($("[name=rid]").val());
        $("[name=rid]").val(cur + 1);
        $("#right button").click();

        url = "cgi-bin/result.py?action=q&rid=" + (cur + 1);
        $.ajax({
            type: "get",
            url: url,
            success: function(ret) {
                $("[name=rid]").val(ret["rid"]);
                $("[name=pid]").val(ret["pid"]);
                $("[name=time]").val(ret["time"]);
                $("[name=memory]").val(ret["memory"]);
                $("[name=outsize]").val(ret["outsize"]);
                $("[name=lang]").val(ret["lang"]);
                $("[name=code]").val(ret["code"]);
            }
        });
        return false;
    });

    $("#inc_pid").click(function() {
        var cur = Number($("[name=pid]").val());
        $("[name=pid]").val(cur + 1)
        return false;
    });

    $("#dec_pid").click(function() {
        var cur = Number($("[name=pid]").val());
        $("[name=pid]").val(cur - 1)
        return false;
    });

});

