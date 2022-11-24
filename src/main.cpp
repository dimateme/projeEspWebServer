#include <Arduino.h>
#include <iostream>
#include <string>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include "TemperatureStub.h"
#include <ArduinoJson.h>

#include "myFunctions.cpp" //fonctions utilitaires

//test oled
#include <Adafruit_SSD1306.h>

const byte PORT = 80;

#include "MyOled.h"
MyOled *myOled = NULL;

#include "MyOledViewWifiAp.h"
MyOledViewWifiAp *myOledViewWifiAp = NULL;

uint8_t addrI2C = 0x3C;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
uint8_t RST=-1;

using namespace std;

#include <HTTPClient.h>
#include <WiFiManager.h>

WiFiManager wm;
#define WEBSERVER_H


//sensor dhht22
#include <DHT.h>
#define DHTPIN 27
#define DHTTYPE    DHT22
TemperatureStub *myTemp;

//mes boutons
#include "MyButton.h"
MyButton *myButtonAction = NULL;
MyButton *myButtonReset = NULL;

//Définition des trois leds
#define GPIO_PIN_LED_LOCK_ROUGE         12 //GPIO12
#define GPIO_PIN_LED_OK_GREEN             26 //GPIO14
#define GPIO_PIN_LED_HEAT_BLUE        25 //GPIO27
 

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";

char bufferTemperature[100];//le buffer permet de stocker la valeur de la température et de la convertir en string
char bufferCompteur[100];//le buffer permet de stocker la valeur du compteur et de la convertir en string
int demarrageFoure = 0; //C'est une variable qui prend la valeur 7 si le four est démarré
float temperature=0.0f;//la température du sensor dht22
bool demarrerFour = false; //Elle permet de savoir si le four est démarré. Si true alors le four est démarré
bool lireTemperature = false; //La temperature est lue si elle est true
int compteur = 20; //Le compteur de temps
const float temperatureMax = 27.5f; //La température maximale
const float temperatureMin = 22.5f; //La température minimale

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
      }
        sprintf(bufferCompteur, "%d", 20);
        return(bufferCompteur);
        
    }
    std::string result = "";
    return result;
    }

void setup() { 
    Serial.begin(9600);
    delay(100);

    //initialisation de myOledViewWifiAp
   myOledViewWifiAp = new MyOledViewWifiAp();
 
  // initialisation de l'écran oled
    myOled = new MyOled(&Wire, RST,SCREEN_HEIGHT, SCREEN_WIDTH);
    myOled->init(addrI2C,true);

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
       
            //Affichage de l'accès du reseau wifi sur l'ecran oled
            myOledViewWifiAp->setNomDuSysteme(stringRandom.c_str());
            myOledViewWifiAp->setSsIDDuSysteme(ssIDRandom.c_str());
            myOledViewWifiAp->setPasseDuSysteme(PASSRandom.c_str());
            myOled->displayView(myOledViewWifiAp);
            Serial.println("Erreur de connexion.");
         }
    else {
            for (size_t i = 0; i < 2; i++)
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
    myButtonAction = new MyButton();        //Pour lire le bouton actions
    myButtonAction->init(T8);
    int sensibilisationButtonAction = myButtonAction->autoSensibilisation();
    
    myButtonReset = new MyButton();         //Pour lire le bouton hard reset
    myButtonReset->init(T9);
    int sensibilisationButtonReset = myButtonReset->autoSensibilisation();
    
    //température
    myTemp = new TemperatureStub();
    myTemp->init(DHTPIN, DHT22);

   

    // ----------- Routes du serveur ----------------
    myServer = new MyServer(PORT);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);

}

void loop() {
  if (lireTemperature == true)
  {
    temperature = myTemp->getTemperature();
    delay(1000);
    sprintf(bufferTemperature, "%4.1f °C", temperature);
    //Serial.println(bufferTemperature);
    
  }
  Serial.println(demarrageFoure);
  if(demarrageFoure == 7){
    while (compteur>=0)
    {
      if ((temperatureMax >= temperature) && (temperatureMin >= 22.5))
      {
        delay(1000);
        compteur--;
        sprintf(bufferCompteur, "%d", compteur);
      }
      sprintf(bufferCompteur, "%d", compteur);
      Serial.println(compteur);
      
      
    }
    if (compteur < 0){
      demarrageFoure = 0;
      compteur = 20;
    }
    
  }
  
      
  //int buttonActionT8 = myButtonT8->checkMyButton();
  int buttonAction  = myButtonAction->checkMyButton();
  if(buttonAction  > 2){
    Serial.println("Button Action pressed");
    //ledOn = false;
  }
  //Gestion du bouton Reset
    int buttonReset = myButtonReset->checkMyButton();
  if(buttonReset > 2)  {  //Si appuyé plus de 30 secondes
        Serial.println("Button Reset pressed\n");
        //Le bouton hard reset a été appuyé
        Serial.println("Button Hard reset pressed\n");
        Serial.println("Suppression des réglages et redémarrage...\n");
        ///wm.resetSettings();
        ///ESP.restart();
    }
  
      delay(500);
  }
