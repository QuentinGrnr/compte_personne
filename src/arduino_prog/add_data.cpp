//programme d'ajout de donnée dans une db par wifi a mettre a jour une fois la db créée

#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

//WiFi
const char* ssid = "nom_du_reseau";
const char* password = "mot_de_passe_du_reseau";

//base de données
IPAddress server_addr(192, 168, 1, 100); // Adresse IP du serveur MySQL
int server_port = 3306; // Port MySQL
const char* user = "nom_utilisateur"; // Nom d'utilisateur MySQL
const char* password_mysql = "mot_de_passe_mysql"; // Mot de passe MySQL
const char* database = "nom_base_de_donnees"; // Nom de la base de données MySQL
const char* table = "nom_table"; // Nom de la table MySQL

// Variables pour les données à envoyer
int current_person = 0;
//==========/le programme incrémente et décrémente current_person grace aux capteurs\==========\\

void setup() {
    // Connexion WiFi
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connexion WiFi en cours");
    }
    Serial.println("WiFi connecté");
}

void loop() {
    // Connexion base de données
    WiFiClient client;
    MySQL_Connection conn(&client);
    if (conn.connect(server_addr, server_port, user, password_mysql, database)) {
        Serial.println("Connexion à la base de données réussie !");

        // Insertion données dans la table
        MySQL_Cursor* cursor = new MySQL_Cursor(&conn);
        char query[100];
        sprintf(query, "INSERT INTO %s (colonne1) VALUES (%d)", table, current_person);//colone1 a remplacer par le vrais nom
        cursor->execute(query);
        delete cursor;

        // Déconnexion base de données
        conn.close();
        Serial.println("Déconnexion de la base de données");
    } else {
        Serial.println("Erreur de connexion à la base de données");
    }
    delay(10000);
}