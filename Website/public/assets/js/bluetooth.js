// Get references to UI elements
let terminalContainer = document.getElementById('terminal');

// Selected device object cache
let deviceCache = null;

// Launch Bluetooth device chooser and connect to the selected
function bt_connect() {
    log("Connecting to BT device");
    return (deviceCache ? Promise.resolve(deviceCache) :
        requestBluetoothDevice()).
        then(device => connectDeviceAndCacheCharacteristic(device)).
        then(characteristic => startNotifications(characteristic)).
        then(() => bt_send("Connect?")).
        catch(error => log(error));
}

function requestBluetoothDevice() {
    return navigator.bluetooth.requestDevice({
      filters: [{services: [0xFFE0]}],
    }).
        then(device => {
          deviceCache = device;

          // Added line
        deviceCache.addEventListener('gattserverdisconnected',
        handleDisconnection);
  
          return deviceCache;
        });
}

function handleDisconnection(event) {
    let device = event.target;
  
    log('"' + device.name +
        '" bluetooth device disconnected, trying to reconnect...');
    bt_connected = false;
    connectDeviceAndCacheCharacteristic(device).
        then(characteristic => startNotifications(characteristic)).
        then(() => bt_send("Connect?")).
        catch(error => log(error));
  }

// Characteristic object cache
let characteristicCache = null;

// Connect to the device specified, get service and characteristic
function connectDeviceAndCacheCharacteristic(device) {
  if (device.gatt.connected && characteristicCache) {
    return Promise.resolve(characteristicCache);
  }

  return device.gatt.connect().
      then(server => {
        return server.getPrimaryService(0xFFE0);
      }).
      then(service => {
        return service.getCharacteristic(0xFFE1);
      }).
      then(characteristic => {
        characteristicCache = characteristic;

        return characteristicCache;
      });
}

// Enable the characteristic changes notification
function startNotifications(characteristic) {
  
    return characteristic.startNotifications().
        then(() => {
          characteristic.addEventListener('characteristicvaluechanged',
            handleCharacteristicValueChanged);
        });
}

// Output to terminal
function log(data, type = '') {
    terminalContainer.insertAdjacentHTML('beforeend',
        '<div' + (type ? ' class="' + type + '"' : '') + '>' + data + '</div>');
}

// Disconnect from the connected device
function disconnect() {
    if (deviceCache) {
      log('Disconnecting from "' + deviceCache.name + '" bluetooth device...');
      deviceCache.removeEventListener('gattserverdisconnected',
          handleDisconnection);
  
      if (deviceCache.gatt.connected) {
        deviceCache.gatt.disconnect();
        log('"' + deviceCache.name + '" bluetooth device disconnected');
      }
      else {
        log('"' + deviceCache.name +
            '" bluetooth device is already disconnected');
      }
      bt_connected = false;
    }
  // Added condition
  if (characteristicCache) {
    characteristicCache.removeEventListener('characteristicvaluechanged',
        handleCharacteristicValueChanged);
    characteristicCache = null;
  }
    deviceCache = null;
  }

// Intermediate buffer for incoming data
let readBuffer = '';

// Data receiving
function handleCharacteristicValueChanged(event) {
    let value = event.target.value;//new TextDecoder().decode(event.target.value);
  //receive(value);
  for (let i = 0; i < value.byteLength; i++) {
    var c = (String.fromCharCode(parseInt('0x' + ('00' + value.getUint8(i).toString(16)).slice(-2))));
    if (c === '\n') {
      let data = readBuffer.trim();
      readBuffer = '';

      if (data) {
        receive(data);
      }
    }
    else {
      readBuffer += c;
    }
  }
    
 }
  
  // Received data handling
  function receive(data) {
    decipher_response(data);
  }

function bt_send(data, log_data = '') {
    data = String(data);

  if (!data || !characteristicCache) {
    return;
  }

  data += '\n';

  if (data.length > 20) {
    let chunks = data.match(/(.|[\r\n]){1,20}/g);

    writeToCharacteristic(characteristicCache, chunks[0]);

    for (let i = 1; i < chunks.length; i++) {
      setTimeout(() => {
        writeToCharacteristic(characteristicCache, chunks[i]);
      }, i * 100);
    }
  }
  else {
    writeToCharacteristic(characteristicCache, data);
  }

  log(log_data == '' ? data : log_data, 'out');
//log(log_data, 'out');
  }

  function writeToCharacteristic(characteristic, data) {
    characteristic.writeValue(new TextEncoder().encode(data));
  }