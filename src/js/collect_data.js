const btSerial = new (require('bluetooth-serial-port')).BluetoothSerialPort();

// Adresse MAC de l'Arduino Bluetooth
const address = '00:00:00:00:00:00';

// UUID du service Bluetooth
const uuid = '00001101-0000-1000-8000-00805F9B34FB';

// Connecte à l'Arduino Bluetooth
btSerial.connect(address, 1, function() {
    console.log('Connected to Arduino Bluetooth.');

    // Récupère la valeur envoyée par l'Arduino
    btSerial.on('data', function(buffer) {
        const data = buffer.readUInt8(0);
        console.log('Received data: ' + data);
    });
}, function() {
    console.log('Failed to connect to Arduino Bluetooth.');
});



// Ferme la connexion Bluetooth
btSerial.close();

