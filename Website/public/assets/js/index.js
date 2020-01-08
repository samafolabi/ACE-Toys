$(document).ready(function(){
    for (var i = 1; i <= 5; i++) {
        $("#table").append("<tr><td><select id='select"+i+"'>" +
            "<option>NOTHING</option>" +
            "<option>FWD</option>" +
            "<option>REV</option>" +
            "<option>LFT</option>" +
            "<option>RGT</option>" +
            "<option>STP</option>" +
        "</select></td><td><input id='num"+i+"' type='number' min='1' max='9' /></td></tr>");
    }

    for (var i = 1; i <= 3; i++) {
      $("#ledtable").append("<tr><td><input type='checkbox' id='red"+i+"'></td>"+
      "<td><input type='checkbox' id='green"+i+"'></td><td><input type='checkbox' id='yellow"+i+"'></td>"+
      "<td><input id='lednum"+i+"' type='number' min='1' max='9' /></td></tr>");
  }

    
});

function ledsend() {
  var body = "";
  for (var i = 1; i <= 3; i++) {
      var red = document.getElementById("red"+i).checked ? 1 : 0;
      var green = document.getElementById("green"+i).checked ? 1 : 0;
      var yellow = document.getElementById("yellow"+i).checked ? 1 : 0;
      var tim = $("#lednum"+i).val() != "" ? $("#lednum"+i).val() : 1;
      body += "" + red + green + yellow + tim + "_";
  }
  send("LED_" + body);
}

function loopsend() {
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
}

var wifi_connected = false,
    bt_connected = false;
var ip = "";
var operation = false; //false = bt, true = wifi

function connect() {
    if (operation) {
      /*var addr = cde.substr(cde.indexOf("ip_addr = '")+11);
      addr = addr.substr(0,addr.indexOf("'"));*/
      /*if (ip == "") {
        if ($("#ip").val() == "") {
          log("Need to set IP Address, since not already set");
          return;
        } else {
          ip = $("#ip").val();
        }
      }*/
      log("Connecting to " + ip);
      send("Test");
    } else {
      bt_connect();
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

var ecns = [];
var wifis = [];

function attempt_connect() {
  var index = document.getElementById("wifi_list").selectedIndex;
  if (ecns[index] == 0 || $("#pass").val() != "") {
    send("WIFI_CONNECT_"+ecns[index]+wifis[index]+";"+$("#pass").val());
  } else {alert("Please enter password");}
}

function decipher_response(response) {
  if (response.indexOf("Connected to Wi-Fi") != -1) {
    wifi_connected = true;
  } else if (response.indexOf("Connected to BT") != -1) {
    bt_connected = true;
  } else if (response.indexOf("Error_Check") != -1) {
    $("#check_button").hide();
    response = "Wifi not available at this time";
  } else if (response.indexOf("IP_GIVEN_") != -1) {
    $("#check_button").hide();
    $("#list_button").hide();
    $("#switch_button").show();
    
    $("#wifi_list").hide();
    $("#pass").hide();
    $("#attempt_button").hide();
    ip = response.substr(9);
    response = "IP address is " + ip;
  } else if (response.indexOf("ConnectionNeeded_Check") != -1) {
    $("#check_button").hide();
    $("#list_button").show();
    ip = response.substr(9);
    response = "Buggy needs to connect to wifi";
  } else if (response.indexOf("JSON:") != -1) {
    $("#list_button").hide();
    var obj = JSON.parse(response.substr(5));
    var wi_list = obj.wifis;
    for (var i = 0; i < wi_list.length; i++) {
      ecns.push(wi_list[i].charAt(0));
      wifis.push(wi_list[i].substr(1));
      $("#wifi_list").append("<option>" + wi_list[i].substr(1) +"</option>");
    }
    $("#wifi_list").show();
    $("#pass").show();
    $("#attempt_button").show();
    response = "List of available access points";
  } else if (response.indexOf("Error_Connecting") != -1) {
    response = "Unable to Connect";
  }
  log(response, 'in');
}