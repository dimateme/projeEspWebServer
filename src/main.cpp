#include <Arduino.h>
#include <iostream>
#include <string>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Arduino.h>
#include "TemperatureStub.h"
#include <ArduinoJson.h>
#include "MyButton.h"
#include "myFunctions.cpp" //fonctions utilitaires


using namespace std;

#include <HTTPClient.h>
#include <WiFiManager.h>
WiFiManager wm;
#define WEBSERVER_H
//sensor dhht22
#define DHTPIN 27
#define DHTTYPE    DHT22

//mes boutons
MyButton *myButtonAction = NULL;
MyButton *myButtonReset = NULL;
//Définition des trois leds de statut
#define GPIO_PIN_LED_LOCK_ROUGE         12 //GPIO12
#define GPIO_PIN_LED_OK_GREEN             26 //GPIO14
#define GPIO_PIN_LED_HEAT_BLUE        25 //GPIO27
 
const int led[3] = {GPIO_PIN_LED_LOCK_ROUGE, GPIO_PIN_LED_OK_GREEN, GPIO_PIN_LED_HEAT_BLUE};
//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;
//DHT dht();
TemperatureStub *myTemp;
//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";




//int random = rand() % 10 + 1;
// Set LED GPIO

// Stores LED state

char buffer[100];
char bufferCompteur[100];
bool ledOn = false;
bool liretempeture = false;
bool demarrerFour = false;
int nbSecondes =0;
int compteur = 20;
String ssIDRandom;

//fonction statique qui permet aux objets d'envoyer des messages (callBack) 
//  arg0 : Action 
// arg1 ... : Parametres
std::string CallBackMessageListener(string message) {

  
  
    while(replaceAll(message, std::string("  "), std::string(" ")));
    //Décortiquer le message
    string actionToDo = getValue(message, ' ', 0);
    string arg1 = getValue(message, ' ', 1);
    
    float temp = myTemp->getTemperature();
    sprintf(buffer, "%4.1f °C", temp);
    std::string tempDuFour = "22"; //Lire le senseur de température
    if (string(actionToDo.c_str()).compare(string("askTempFour")) == 0) {
      //Serial.println(buffer);
      return(buffer); 
      
    }
    
    if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {
      demarrerFour = true;
      return("");
    }
    if (string(actionToDo.c_str()).compare(string("askCompteur")) == 0) {
      
      
      while (compteur > 0)
      {
        if(demarrerFour==true)
        {
          compteur--;
        
        }
        sprintf(bufferCompteur, "%d", compteur);
        return(bufferCompteur);
      }
      compteur = 21;
      
      
        
    }
  
    std::string result = "";
    return result;
    }

void setup() { 
    Serial.begin(9600);
    delay(100);
   ledOn=false;
 //Connection au WifiManager
    String ssIDRandom, PASSRandom;
    String stringRandom;
    stringRandom = get_random_string(4).c_str();
    ssIDRandom = SSID;
    ssIDRandom = ssIDRandom + stringRandom;
    stringRandom = get_random_string(4).c_str();
    PASSRandom = PASSWORD;
    PASSRandom = PASSRandom + stringRandom;

char strToPrint[128];
    sprintf(strToPrint, "Identification : %s   MotDePasse: %s", ssIDRandom, PASSRandom);
    Serial.println(strToPrint);


 if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())){
        Serial.println("Erreur de connexion.");
        }
    else {
        Serial.println("Connexion Établie.");
        }

//gestion des boutons
    myButtonReset = new MyButton();         //Pour lire le bouton hard reset
    myButtonReset->init(T9);
    int sensibilisationButtonReset = myButtonReset->autoSensibilisation();
    

    //Initialisation des LED statuts
    /*pinMode(led[0], OUTPUT);
    pinMode(led[1], OUTPUT);
    pinMode(led[2], OUTPUT);*/
    pinMode(GPIO_PIN_LED_LOCK_ROUGE, OUTPUT);
    pinMode(GPIO_PIN_LED_OK_GREEN, OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_BLUE, OUTPUT);
    //température
    myTemp = new TemperatureStub();
    myTemp->init(27, DHT22);
    // ----------- Routes du serveur ----------------
    myServer = new MyServer(80);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);

}

void loop() {
  float temp = myTemp->getTemperature();
      sprintf(buffer, "%4.1f °C", temp);
      //Serial.println(buffer);
      delay(1000);
  
  //int buttonActionT8 = myButtonT8->checkMyButton();
  int buttonReset = myButtonReset->checkMyButton();
  if(buttonReset > 2){
    Serial.println("Button Action pressed");
    ledOn = false;
  }
  if(ledOn){
    delay(nbSecondes * 1000);
    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
    delay(nbSecondes * 1000);
    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
    digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);
    delay(nbSecondes * 1000);
    digitalWrite(GPIO_PIN_LED_OK_GREEN,LOW);
    digitalWrite(GPIO_PIN_LED_HEAT_BLUE,HIGH);
    delay(nbSecondes * 1000);
    digitalWrite(GPIO_PIN_LED_HEAT_BLUE,LOW);
  }

      delay(500);
  }
