/*var workspace = Blockly.inject('blockly',
    {toolbox: document.getElementById('toolbox')});

Blockly.Blocks['string_length'] = {
    init: function() {
        this.appendValueInput('VALUE')
            .setCheck('String')
            .appendField('length of');
        this.setOutput(true, 'Number');
        this.setColour(160);
        this.setTooltip('Returns number of letters in the provided text.');
        this.setHelpUrl('http://www.w3schools.com/jsref/jsref_length_string.asp');
    }
};

Blockly.JavaScript['string_length'] = function(block) {
    // String or array length.
    var argument0 = Blockly.JavaScript.valueToCode(block, 'VALUE',
        Blockly.JavaScript.ORDER_FUNCTION_CALL) || '\'\'';
    return [argument0 + '.length', Blockly.JavaScript.ORDER_MEMBER];
};*/

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

    $("#lcdsend").click(function(){
        send("LCD_"+$("#lcd").val());
        $("#lcd").val("");
    });

    $("#loopsend").click(function () {
        var body = "";
        for (var i = 1; i <= 5; i++) {
            var cmd = $("#select"+i).val();
            var tim = $("#num"+i).val() != "" ? $("#num"+i).val() : 1;
            if (cmd != "NOTHING") {
                body += cmd + " " + tim + "\n";
            }
        }
        send(body);
    })
});

var connected = false;

function send(cmd) {
    if (cmd == 'Test') {connected = true;}
    if (connected) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                $("#response").html(this.responseText);
            }
        };
        xhttp.open("POST", "http://"+$("#ip").val(), true);
        xhttp.send("ACE-Break__\n"+cmd+"\n");
    }
}