#include <SoftwareSerial.h>

// Pin utilisée pour la communication Bluetooth
#define RX_PIN 4
#define TX_PIN 5

// Initialise le port série Bluetooth
SoftwareSerial bluetooth(RX_PIN, TX_PIN);

// variable contenant le nombre de personnes
byte personnes = 10;

void setup() {
    // Initialise la communication série à 9600 bauds
    Serial.begin(38400);
    // Initialise la communication Bluetooth à 9600 bauds
    bluetooth.begin(38400);
}

void loop() {
    // Envoie le nombre de personnes en Bluetooth
    bluetooth.print(personnes);
    delay(10000);
}

