//programme d'ajout de donnée dans une db par wifi a mettre a jour une fois la db créée

#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Paramètres WiFi
const char* ssid = "nom_du_reseau";
const char* password = "mot_de_passe_du_reseau";

// Paramètres de la base de données
IPAddress server_addr(192, 168, 1, 100); // Adresse IP du serveur MySQL
int server_port = 3306; // Port du serveur MySQL
const char* user = "nom_utilisateur"; // Nom d'utilisateur MySQL
const char* password_mysql = "mot_de_passe_mysql"; // Mot de passe MySQL
const char* database = "nom_base_de_donnees"; // Nom de la base de données MySQL
const char* table = "nom_table"; // Nom de la table MySQL

// Variables pour les données à envoyer
int valeur1 = 123;
float valeur2 = 3.14;

void setup() {
    // Connexion WiFi
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connexion WiFi en cours...");
    }
    Serial.println("WiFi connecté !");
}

void loop() {
    // Connexion à la base de données
    WiFiClient client;
    MySQL_Connection conn(&client);
    if (conn.connect(server_addr, server_port, user, password_mysql, database)) {
        Serial.println("Connexion à la base de données réussie !");

        // Insertion des données dans la table
        MySQL_Cursor* cursor = new MySQL_Cursor(&conn);
        char query[100];
        sprintf(query, "INSERT INTO %s (colonne1, colonne2) VALUES (%d, %f)", table, valeur1, valeur2);
        cursor->execute(query);
        delete cursor;

        // Déconnexion de la base de données
        conn.close();
        Serial.println("Déconnexion de la base de données !");
    } else {
        Serial.println("Erreur de connexion à la base de données !");
    }

    // Attente avant la prochaine insertion de données
    delay(10000);
}