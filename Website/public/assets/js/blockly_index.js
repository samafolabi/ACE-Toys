/* TODO: Change toolbox XML ID if necessary. Can export toolbox XML from Workspace Factory. */
var toolbox = document.getElementById("toolbox");

var options = { 
	toolbox : toolbox, 
	collapse : true, 
	comments : true, 
	disable : true, 
	maxBlocks : Infinity, 
	trashcan : true, 
	horizontalLayout : false, 
	toolboxPosition : 'start', 
	css : true, 
	media : 'https://blockly-demo.appspot.com/static/media/', 
	rtl : false, 
	scrollbars : true, 
	sounds : true, 
	oneBasedIndex : true, 
	grid : {
		spacing : 20, 
		length : 1, 
		colour : '#888', 
		snap : false
	}
};

Blockly.Blocks['move'] = {
    init: function() {
      this.jsonInit({
    "message0": "move %1 %2 %3 seconds",
    "args0": [
      {
        "type": "field_dropdown",
        "name": "direction",
        "options": [
          [
            "forward",
            "FWD"
          ],
          [
            "backward",
            "REV"
          ],
          [
            "left",
            "LFT"
          ],
          [
            "right",
            "RGT"
          ],
          [
            "stop",
            "STP"
          ]
        ]
      },
      {
        "type": "input_dummy"
      },
      {
        "type": "input_value",
        "name": "time",
        "check": "Number",
        "align": "CENTRE"
      }
    ],
    "inputsInline": true,
    "previousStatement": null,
    "nextStatement": null,
    "colour": 230,
    "tooltip": "Move a particular direction or not at all",
    "helpUrl": ""
  })}};

  Blockly.Blocks['end'] = {init: function() {
    this.jsonInit({
        "message0": "end",
        "previousStatement": null,
        "colour": 230,
        "tooltip": "",
        "helpUrl": ""
      })
  }};
/* Inject your workspace */ 
var workspace = Blockly.inject("blockly", options);

/* Load Workspace Blocks from XML to workspace. Remove all code below if no blocks to load */

/* TODO: Change workspace blocks XML ID if necessary. Can export workspace blocks XML from Workspace Factory. */
var workspaceBlocks = document.getElementById("workspaceBlocks"); 

/* Load blocks to workspace. */
Blockly.Xml.domToWorkspace(workspaceBlocks, workspace);

Blockly.JavaScript['move'] = function(block) {
    var direction = block.getFieldValue('direction');
    var time = Blockly.JavaScript.valueToCode(block, 'time', Blockly.JavaScript.ORDER_ATOMIC);
    time = time == "" ? 1 : time;
    // TODO: Assemble JavaScript into code variable.
    var code = 'send_build('+direction+', '+time+');\n';
    return code;
  };

  workspace.registerButtonCallback("myFWD", function(){send("FWD");})
  workspace.registerButtonCallback("myREV", function(){send("REV");})
  workspace.registerButtonCallback("myLFT", function(){send("LFT");})
  workspace.registerButtonCallback("myRGT", function(){send("RGT");})
  workspace.registerButtonCallback("mySTP", function(){send("STP");})
  workspace.registerButtonCallback("myConnect", function(){connect();})
var cde;
  function myUpdateFunction(event) {
    cde = Blockly.JavaScript.workspaceToCode(workspace);
    document.getElementById('textarea').value = cde;
  }
  workspace.addChangeListener(myUpdateFunction);

  

Blockly.JavaScript['end'] = function(block) {
    // TODO: Assemble JavaScript into code variable.
    var code = 'end();\n';
    return code;
  };

  

var builder = "";

function send_build(direction, time) {
    builder += "" + direction + " " + time + "\n";
}

function end() {
    alert(builder);
}