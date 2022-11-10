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
 

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;
//DHT dht();
TemperatureStub *myTemp;
//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";

char bufferTemperature[100];
char bufferCompteur[100];
int demarrageFoure = 0;
float temp=0;


bool demarrerFour = false;
bool lireTemperature = false;
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
    
    
    if (string(actionToDo.c_str()).compare(string("askTempFour")) == 0) {
      if (lireTemperature == true)
      {
        return(bufferTemperature); 
      }
    
    }
    
    if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {
      demarrerFour = true;
      demarrageFoure = 7;
      return("");
    }
    if (string(actionToDo.c_str()).compare(string("askCompteur")) == 0) {
      if (demarrageFoure == 7)
      {
        return(bufferCompteur);
      }else
      {
        sprintf(bufferCompteur, "%d", 20);
        return(bufferCompteur);
      }
        
    }
  
    std::string result = "";
    return result;
    }

void setup() { 
    Serial.begin(9600);
    delay(100);
    //Initialisation des LED statuts
    pinMode(GPIO_PIN_LED_LOCK_ROUGE, OUTPUT);
    pinMode(GPIO_PIN_LED_OK_GREEN, OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_BLUE, OUTPUT);
  
   lireTemperature=true;
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
            for (size_t i = 0; i < 3; i++)
            {
                digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, HIGH);
                digitalWrite(GPIO_PIN_LED_OK_GREEN, HIGH);
                digitalWrite(GPIO_PIN_LED_HEAT_BLUE, HIGH);
                delay(1000);
                digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
                digitalWrite(GPIO_PIN_LED_OK_GREEN, LOW);
                digitalWrite(GPIO_PIN_LED_HEAT_BLUE, LOW);
                delay(1000);
            }
            
          Serial.println("Connexion Établie.");
        }

//gestion des boutons
    myButtonReset = new MyButton();         //Pour lire le bouton hard reset
    myButtonReset->init(T9);
    int sensibilisationButtonReset = myButtonReset->autoSensibilisation();
    

    
    
    //température
    myTemp = new TemperatureStub();
    myTemp->init(27, DHT22);
    // ----------- Routes du serveur ----------------
    myServer = new MyServer(80);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);

}

void loop() {
  if (lireTemperature == true)
  {
    temp = myTemp->getTemperature();
    delay(1000);
    sprintf(bufferTemperature, "%4.1f °C", temp);
    Serial.println(bufferTemperature);
    
  }
  if(demarrageFoure == 7){
    while (compteur>0)
    {
      if ((24.5 >= temp) && (temp >= 22.5))
      {
        delay(1000);
        compteur--;
        sprintf(bufferCompteur, "%d", compteur);
      }
      sprintf(bufferCompteur, "%d", compteur);
      
      
    }
    if (compteur < 0){
      demarrageFoure = 0;
      compteur = 20;
    }
  }
      
  
  //int buttonActionT8 = myButtonT8->checkMyButton();
  int buttonReset = myButtonReset->checkMyButton();
  if(buttonReset > 2){
    Serial.println("Button Action pressed");
    //ledOn = false;
  }
  
      delay(500);
  }
