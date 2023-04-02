#include "grove_two_rgb_led_matrix.h"


// Déclaration des pins utilisés
const byte BP1=2;
const byte BP2=3;
const byte Led1=7;
const byte Led2=6;

// Pour éviter les rebonds
const byte latency=250; // Latence sur les boutons
volatile unsigned long lastTime = 0; // Sauvegarder le timecode du dernier click sur un bouton

// Gestion du compte
volatile bool flag1= false; // Sauvegarder si quelqu'un est passé en face du premier détecteur
volatile bool flag2= false; // pour le second capteur
volatile byte compteur = 0; // Variable du compteur

// Gestion de l'affichage
volatile unsigned long lastScreenUpdate = 0; // gestion de la latence de l'affichage
const byte screenLatency=1000; // Latence sur les boutons
const byte DISPLAY_COLOR= 0x33; // Couleur sur la matrice RGB
GroveTwoRGBLedMatrixClass matrix; // Déclaration de l'objet qui permet d'interagir avec la matrice


void waitForMatrixReady() {
  delay(1000);
}

void setup() {
  Serial.begin(115200);
// Déclaration des modes des pins
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(BP1, INPUT);
  pinMode(BP2, INPUT);
// Affectation des interruptions aux pins
  attachInterrupt(digitalPinToInterrupt(BP1),BP1_Interrupt,RISING);
  attachInterrupt(digitalPinToInterrupt(BP2),BP2_Interrupt,RISING);

// Partie Matrice
  Wire.begin();
  waitForMatrixReady();
  uint16_t VID = 0;
  VID = matrix.getDeviceVID();
  if (VID != 0x2886){ // 0x2886 -> probablement l'adresse du port I2C
    Serial.println("Can not detect led matrix!!!");
    while (1);
  }
  Serial.println("Matrix init success!!!");

// On considère qu'au démarrage, personne n'est dans la pièce
  //compteur=0;
}

void loop() {
// Temporaire
  digitalWrite(Led1, flag1); // LED1 représente le status de BP1
  digitalWrite(Led2, flag2); // LED2 représente le status de BP2

// Partie définitive
  if((lastScreenUpdate+screenLatency)<millis()){
    matrix.displayNumber(compteur, 1000, false, DISPLAY_COLOR); // On affiche la nouvelle valeur
    lastScreenUpdate = millis(); // On sauvegarde le temps
    // Serial.println(compteur)
  }
}

void BP1_Interrupt(){

  if(flag2){
    compteur--; // La personne est bien sortie
    flag2=false; // On est plus en attente de l'activation du capteur
    lastTime=millis(); // Sauvegarde du temps
  }
  else if(!flag1&&(millis()>(lastTime+latency))){
    flag1=true; // En attente de l'activation du second capteur, le premier reste en attente
    lastTime=millis(); // Sauvegarde du temps
  }

};

void BP2_Interrupt(){

  if(flag1){
    compteur++;
    flag1=false;
    Serial.println(compteur);
    lastTime=millis();
  }
  else if(!flag2&&(millis()>(lastTime+latency))){
    flag2=true;
    lastTime=millis();
  }

};
