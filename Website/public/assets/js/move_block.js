[{
  "type": "move",
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
}]