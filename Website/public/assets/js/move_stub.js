Blockly.JavaScript['move'] = function(block) {
  var dropdown_direction = block.getFieldValue('direction');
  var value_time = Blockly.JavaScript.valueToCode(block, 'time', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = '...;\n';
  return code;
};