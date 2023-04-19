#include <SoftwareSerial.h>
#include "grove_two_rgb_led_matrix.h"



// Déclaration des pins utilisés par les capteurs
const byte CAPT1=2;
const byte CAPT2=3;
// Pin utilisée pour la communication Bluetooth
#define RX_PIN 4
#define TX_PIN 5

// Gestion des latences
const byte captLatency=250; // Latence sur les capteurs
volatile unsigned long captLastTrigger = 0; // Sauvegarder le timecode du dernier déclenchement de capteur
#define BLUETOOTH_LATENCY 10000
volatile unsigned long lastBlueetoothSend = 0; // Sauvegarder le timecode du dernier déclenchement de capteur
volatile unsigned long lastMillis = 0;

// Gestion du compte
volatile bool flag1= false; // Sauvegarder si quelqu'un est passé en face du premier détecteur
volatile bool flag2= false; // pour le second capteur
volatile byte compteur = 0; // Variable du contenant le nombre de personnes

// Gestion de l'affichage
volatile unsigned long lastScreenUpdate = 0; // gestion de la latence de l'affichage
const int screenLatency=1000; // Latence sur les boutons
const byte DISPLAY_COLOR= COLORS::green; // Couleur sur la matrice RGB
GroveTwoRGBLedMatrixClass screen1(GROVE_TWO_RGB_LED_MATRIX_DEF_I2C_ADDR, 1), screen2(GROVE_TWO_RGB_LED_MATRIX_DEF_I2C_ADDR+1, 1); // Déclaration des objets qui permettent d'interagir avec les matrices



// Initialise le port série Bluetooth
SoftwareSerial bluetooth(RX_PIN, TX_PIN);

void waitForMatrixReady() {
  delay(1000);
}


void setup() {
// Initialise la communication série à 9600 bauds
  Serial.begin(9600);
// Initialise la communication Bluetooth à 38400 bauds
  bluetooth.begin(38400);

// Déclaration des modes des pins
  pinMode(CAPT1, INPUT);
  pinMode(CAPT2, INPUT);
  pinMode(FLAG1, INPUT);
  pinMode(FLAG2, INPUT);
// Affectation des interruptions aux pins
  attachInterrupt(digitalPinToInterrupt(CAPT1),Capt1_Interrupt,RISING);
  attachInterrupt(digitalPinToInterrupt(CAPT2),Capt2_Interrupt,RISING);

// Partie Matrice LED
  Wire.begin();
  waitForMatrixReady();
  uint16_t VID=0, VID2=0;
  VID = screen1.getDeviceVID(); // VID = VendorID
  VID2 = screen2.getDeviceVID();
  if (VID != 0x2886){ // 0x2886 -> ModèleID de la matrice LED, pour vérifier que l'appareil contacté est bien une matrice LED
    Serial.println("Can not detect led matrix 1!!!");
  }
  if (VID2 != 0x2886){ 
    Serial.println("Can not detect led matrix 2!!!");
  }
  Serial.println("Matrix init success!!!");
}

void loop() {
  lastMillis = millis();

// Affichage sur les matrices LED
  if((lastScreenUpdate+screenLatency)<lastMillis){
    screen1.displayNumber(compteur/10, screenLatency, true, DISPLAY_COLOR); // On affiche l'unité du compteur (propriété de la division des int)
    screen2.displayNumber(compteur%10, screenLatency, true, DISPLAY_COLOR); // On affiche la nouvelle valeur
    lastScreenUpdate = lastMillis; // On sauvegarde le temps
    Serial.println(compteur); // On affiche le compteur dans la console série
  }

// Envoie le nombre de personnes en Bluetooth
  if((lastBlueetoothSend+BLUETOOTH_LATENCY)<lastMillis){
    bluetooth.print(compteur);
    lastBlueetoothSend=lastMillis;
    // delay(10000);
  }

}


void Capt1_Interrupt(){
  if(flag2){ // Le capteur côté intérieur de la pièce a été déclenché plus tôt
    if(compteur!=0){ // On ne peut pas retirer une personne d'une pièce vide
      compteur--; // La personne est bien sortie
    }
    flag2=false; // On est plus en attente de l'activation du capteur
    captLastTrigger=millis(); // Sauvegarde du temps
  }
  else if(!flag1&&(millis()>(captLastTrigger+captLatency))){ // Ce capteur n'a pas encore été déclenché ET la latence est respectée
    flag1=true; // En attente de l'activation du second capteur, le premier reste en attente
    captLastTrigger=millis(); // Sauvegarde du temps
  }

};

void Capt2_Interrupt(){
  if(flag1){
    if(compteur!=255){ // On ne peut pas dépasser 255 avec le type byte
      compteur--; // La personne est bien sortie
    }
    flag1=false;
    captLastTrigger=millis();
  }
  else if(!flag2&&(millis()>(captLastTrigger+captLatency))){
    flag2=true;
    captLastTrigger=millis();
  }

};
