#include <SoftwareSerial.h>

// Pin utilisée pour la communication Bluetooth
#define RX_PIN 2
#define TX_PIN 3

// Initialise le port série Bluetooth
SoftwareSerial bluetooth(RX_PIN, TX_PIN);

// variable contenant le nombre de personnes
int personnes = 42;

void setup() {
    // Initialise la communication série à 9600 bauds
    Serial.begin(9600);
    // Initialise la communication Bluetooth à 9600 bauds
    bluetooth.begin(9600);
}

void loop() {
    // Envoie le nombre de personnes en Bluetooth
    bluetooth.print(personnes);
    delay(1000);
}
