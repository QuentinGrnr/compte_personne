#include <SoftwareSerial.h>
#include "grove_two_rgb_led_matrix.h"

#define FLAG1 7
#define FLAG2 8



// Déclaration des pins utilisés 
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
const byte DISPLAY_COLOR= 0x33; // Couleur sur la matrice RGB
GroveTwoRGBLedMatrixClass screen1(GROVE_TWO_RGB_LED_MATRIX_DEF_I2C_ADDR, 1), screen2(GROVE_TWO_RGB_LED_MATRIX_DEF_I2C_ADDR+1, 1); // Déclaration de l'objet qui permet d'interagir avec les matrices



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

// Partie Matrice
  Wire.begin();
  waitForMatrixReady();
  uint16_t VID=0, VID2=0;
  VID = screen1.getDeviceVID();
  VID2 = screen2.getDeviceVID();
  if (VID != 0x2886){ // 0x2886 -> probablement l'adresse du port I2C
    Serial.println("Can not detect led matrix 1!!!");
  }
  if (VID2 != 0x2886){ // 0x2886 -> probablement l'adresse du port I2C
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
    lastScreenUpdate = millis(); // On sauvegarde le temps
    Serial.print(compteur);
    Serial.print(" | ");
    Serial.print(flag1);
    Serial.print(" | ");
    Serial.println(flag2);
  }

// Envoie le nombre de personnes en Bluetooth
  if(0||(lastBlueetoothSend+BLUETOOTH_LATENCY)<lastMillis){
    Serial.println("Transmission BT");    
    bluetooth.print(compteur);
    Serial.println("Transmit BT");
    lastBlueetoothSend=lastMillis;
  }




  digitalWrite(FLAG1, digitalRead(CAPT1));
  digitalWrite(FLAG2, digitalRead(CAPT2));


  
  if(Serial.available()) { // Vérifie si des données sont disponibles
    compteur = Serial.read(); // Lit les données série et les stocke dans une variable
    // Faites quelque chose avec la valeur lue, par exemple :
    Serial.print("Valeur lue : ");
    Serial.println(compteur);
  }


}





void Capt1_Interrupt(){
  Serial.println("--------------1");
  if(flag2){ // Le capteur côté intérieur de la pièce a été déclenché
    compteur--; // La personne est bien sortie
    flag2=false; // On est plus en attente de l'activation du capteur
    captLastTrigger=millis(); // Sauvegarde du temps
  }
  else if(!flag1&&(millis()>(captLastTrigger+captLatency))){ // Ce capteur n'a pas encore été déclenché ET la latence est respectée
    flag1=true; // En attente de l'activation du second capteur, le premier reste en attente
    captLastTrigger=millis(); // Sauvegarde du temps
  }

};

void Capt2_Interrupt(){
  Serial.println("--------------2");
  if(flag1){
    compteur++;
    flag1=false;
    captLastTrigger=millis();
  }
  else if(!flag2&&(millis()>(captLastTrigger+captLatency))){
    flag2=true;
    captLastTrigger=millis();
  }

};
