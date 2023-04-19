/*
  Titre      : Client arduino
  Auteur     : Duvalier Tsagmo
  Date       : 29/03/2023
  Description: envoie des donnees au server java pour qu'il stocke dans la base de donnee posgreSQL
  Version    : 0.0.1
*/

#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Adafruit_AHTX0.h> // Ajouter la bibliothèque AHT20

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress serverIp(192, 168, 1, 54);
int serverPort = 2222;
const char *ssid = "UNIFI_IDO1";
const char *password = "42Bidules!";
// const char *ssid = "BELL932";
// const char *password = "CMR062023";
WiFiClient client;

Adafruit_AHTX0 aht; // Initialiser le capteur AHT20
sensors_event_t humidity, temp;

float temperature = 0;
float humidite = 0;

void setup()
{
    WiFi.begin(ssid, password);
    Serial.begin(9600);
    delay(1000);

    aht.begin(); // Initialiser le capteur AHT20
}

void loop()
{
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    if (client.connect(serverIp, serverPort))
    {
        // populate temp and humidity objects with fresh data
        temperature = temp.temperature;        // Lire la température
        humidite = humidity.relative_humidity; // Lire l'humidité

        Serial.println("Connected to server.");

        // Envoyer les données de température et d'humidité au serveur
        client.print("Temperature: ");
        client.print(temperature);
        client.print(" C, Humidity: ");
        client.print(humidite);
        client.println(" %");

        Serial.println("Sent data to server.");

        while (client.available())
        {
            Serial.println(client.readStringUntil('\n'));
        }
        client.stop();
        Serial.println("Connection closed.");
    }
    else
    {
        Serial.println("Connection failed");
        Serial.println("Connection failed. Retrying in 5 seconds...");
    }
    // Modifier le délai pour attendre 3 secondes avant de réessayer encore
    delay(3000); 
}