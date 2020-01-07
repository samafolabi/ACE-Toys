$(document).ready(function(){
    for (var i = 1; i <= 5; i++) {
        $("#table").append("<tr><td><select id='select"+i+"'>" +
            "<option>NOTHING</option>" +
            "<option>FWD</option>" +
            "<option>REV</option>" +
            "<option>LFT</option>" +
            "<option>RGT</option>" +
            "<option>STP</option>" +
        "</select></td><td><input id='num"+i+"' type='number' /></td></tr>");
    }

    $("#loopsend").click(function () {
        var body = "";
        var x = 0;
        for (var i = 1; i <= 5; i++) {
            var cmd = $("#select"+i).val();
            var tim = $("#num"+i).val() != "" ? $("#num"+i).val() : 1;
            if (cmd != "NOTHING") {
                body += "" + cmd + tim + "_";
                x++;
            }
        }
        send("LOOP_" + x + "_" + body);
    })
});

var wifi_connected = false,
    bt_connected = false;
var ip = "";
var operation = false; //false = bt, true = wifi

function connect() {
    if (operation) {
      /*var addr = cde.substr(cde.indexOf("ip_addr = '")+11);
      addr = addr.substr(0,addr.indexOf("'"));*/
      if (ip == "") {
        if ($("#ip").val() == "") {
          log("Need to set IP Address, since not already set");
          return;
        } else {
          ip = $("#ip").val();
        }
      }
      log("Connecting to " + ip);
      send("Test");
    } else {
      bt_connect();
      //deal with wifi and bt responses, and bt error response, and disconnections
    }
}

function switch_operation() {
  if (operation) {
    operation = false;
    $("#operation").html("Bluetooth");
    $("#ip").hide();
  } else {
    operation = true;
    $("#operation").html("Wi-Fi");
    $("#ip").show();
  }
}

function send(cmd) {
    if ((wifi_connected && operation) || cmd == "Test") {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                decipher_response(this.responseText);
            }
        };
        xhttp.open("POST", "http://"+ip, true);
        xhttp.send("ACE-Break__\n"+cmd+"\n");
        if (cmd != "Test") log(cmd, 'out');
    } else if (bt_connected && !operation) {
      bt_send(cmd);
    } else {alert("Not Connected yet");}
}

function decipher_response(response) {
  if (response.indexOf("Connected to Wi-Fi") != -1) {
    wifi_connected = true;
  } else if (response.indexOf("Connected to BT") != -1) {
    bt_connected = true;
  }
  log(response, 'in');
}