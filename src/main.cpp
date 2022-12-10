/**
    Gestion du main.cpp
    @file main.cpp
    @author Jean paul Bai
    @version 1.0 21/09/22
*/
#include <Arduino.h>
#include <iostream>
#include <string>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include "TemperatureStub.h"
#include <ArduinoJson.h>

#include <string>
#include "myFunctions.cpp" //fonctions utilitaires

const byte PORT = 80;

#include <Adafruit_SSD1306.h>
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

#include "MyOledViewWorkingCOLD.h"
MyOledViewWorkingCOLD *myOledViewWorkingCOLD = NULL;

#include "MyOledViewWorkingHEAT.h"
MyOledViewWorkingHEAT *myOledViewWorkingHEAT = NULL;

#include "MyOledViewInitialisation.h"
MyOledViewInitialisation *myOledViewInitialisation = NULL;

//sensor dhht22
#include <DHT.h>
#define DHTPIN 27
#define DHTTYPE    DHT22
TemperatureStub *myTemp;

//Pour la gestion des boutons
#include "MyButton.h"
MyButton *myButtonAction = NULL;
MyButton *myButtonReset = NULL;

char bufferSensibiliteBoutonAction[100];
char bufferSensibiliteBoutonReset[100];

#include "MyOledViewWorkingOFF.h"
MyOledViewWorkingOFF *myOledViewWorkingOFF = NULL;

//Définition des trois leds
#define GPIO_PIN_LED_LOCK_ROUGE         12 //GPIO12
#define GPIO_PIN_LED_OK_GREEN             26 //GPIO14
#define GPIO_PIN_LED_HEAT_YELLOW        25 //GPIO27
 

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";

String nomDuSyteme = "SAC System";
String iDDuSyteme = "SD777";
String idDuSysteme = "92894";
String statusDuSyteme = "";

char bufferTemperature[100];//le buffer permet de stocker la valeur de la température et de la convertir en string
char bufferCompteur[100];//le buffer permet de stocker la valeur du compteur et de la convertir en string
int demarrageFoure = 0; //C'est une variable qui prend la valeur 7 si le four est démarré
float temperature=0.0f;//la température du sensor dht22
bool demarrerFour = false; //Elle permet de savoir si le four est démarré. Si vrai alors le four est démarré
bool lireTemperature = false; //La temperature est lue si elle est vraie
int compteur = 0; //Le compteur de temps
float temperatureMax = 0.0f; //La température maximale
float temperatureMin = 0.0f; //La température minimale
int tempsDeSechage=0; //Le temps de sechage d'un bois envoyé par le serveur
bool etatChauffage = false; //Elle permet de gérer les leds avec l'état du chauffage
float temperatureSechage=0.0f; //La température de sechage obtenur par le serveur


String ssIDRandom;

//fonction statique qui permet aux objets d'envoyer des messages (callBack) 
//  arg0 : Action 
// arg1 ... : Parametres
std::string CallBackMessageListener(string message) {
    while(replaceAll(message, std::string("  "), std::string(" ")));
    //Décortiquer le message
    string actionToDo = getValue(message, ' ', 0);
    string arg1 = getValue(message, ' ', 1);
    string arg2 = getValue(message, ' ', 2);
    string arg3 = getValue(message, ' ', 3);
    string arg4 = getValue(message, ' ', 4);
    string arg5 = getValue(message, ' ', 5);
    
    if (string(actionToDo.c_str()).compare(string("askTempFour")) == 0) {
      if (lireTemperature == true)
      {
        return(bufferTemperature); 
      }
    }

    if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {
      demarrerFour = true;
      demarrageFoure = 7; //C'est une variable qui prend la valeur 7 si le four est démarré
      return("");
    }

    if (string(actionToDo.c_str()).compare(string("askCompteur")) == 0) {
      if (demarrageFoure == 7)
      {
        return(bufferCompteur);
      }
        sprintf(bufferCompteur, "%d", 0);
        return(bufferCompteur);
        
    }
    
    char bufferTemperatureSechage[100];//le buffer permet de stocker la valeur de la température du bois et de la convertir en string
    char buffertempsDeSechage[100]; //le buffer permet de stocker la valeur du temps de sechage du bois et de la convertir en string
   
    if (string(actionToDo.c_str()).compare(string("tellCaracteristique")) == 0)
    {
      sprintf(bufferTemperatureSechage, "%s", arg2.c_str());
      temperatureSechage =atof(bufferTemperatureSechage);
      sprintf(buffertempsDeSechage, "%s", arg4.c_str());
      tempsDeSechage=atoi(buffertempsDeSechage);
    }
    
    std::string result = "";
    return result;
    }

void afficherOledOff();
void afficherOledApresDemarrageButton();

void setup() { 
    Serial.begin(9600);
    delay(100);

    //initialisation de myOledViewWifiAp
   myOledViewWifiAp = new MyOledViewWifiAp();
 
  // initialisation de l'écran oled
    myOled = new MyOled(&Wire, RST,SCREEN_HEIGHT, SCREEN_WIDTH);
    myOled->init(addrI2C,true);

    //Initialisation des leds 
    pinMode(GPIO_PIN_LED_LOCK_ROUGE, OUTPUT);
    pinMode(GPIO_PIN_LED_OK_GREEN, OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_YELLOW, OUTPUT);

    lireTemperature=true; // si la variable est true alors la temperature est lue par le sensor dht22
 
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

 //Initialisation du systeme de gestion de la sensibilité des boutons
    myOledViewInitialisation = new MyOledViewInitialisation();
    
    //gestion des boutons
    myButtonAction = new MyButton();        //Pour lire le bouton actions
    myButtonAction->init(T8);
    int sensibilisationButtonAction = myButtonAction->autoSensibilisation();
    sprintf(bufferSensibiliteBoutonAction, "%d", sensibilisationButtonAction);

    myButtonReset = new MyButton();         //Pour lire le bouton hard reset
    myButtonReset->init(T9);
    int sensibilisationButtonReset = myButtonReset->autoSensibilisation();
    sprintf(bufferSensibiliteBoutonReset, "%d", sensibilisationButtonReset);
    
    myOledViewInitialisation->setNomDuSysteme(nomDuSyteme.c_str());
    myOledViewInitialisation->setIdDuSysteme(iDDuSyteme.c_str());
    myOledViewInitialisation->setSensibiliteBoutonAction(bufferSensibiliteBoutonAction);
    myOledViewInitialisation->setSensibiliteBoutonReset(bufferSensibiliteBoutonReset);
    myOled->displayView(myOledViewInitialisation);

 if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())){
       
            //Affichage de l'accès du reseau wifi sur l'ecran oled si la connection au wifiManager n'est pas établie
            myOledViewWifiAp->setNomDuSysteme(nomDuSyteme.c_str());
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
                digitalWrite(GPIO_PIN_LED_HEAT_YELLOW, HIGH);
                delay(1000);
                digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
                digitalWrite(GPIO_PIN_LED_OK_GREEN, LOW);
                digitalWrite(GPIO_PIN_LED_HEAT_YELLOW, LOW);
                delay(1000);
            }
           
          Serial.println("Connexion Établie.");
        }

    //Gestion de la temperature
    myTemp = new TemperatureStub();
    myTemp->init(DHTPIN, DHT22);
    temperature = myTemp->getTemperature();
    sprintf(bufferTemperature, "%4.1f", temperature);

   //Affichage Oled en mode ready
    myOledViewWorkingOFF = new MyOledViewWorkingOFF();

    myOledViewWorkingCOLD = new MyOledViewWorkingCOLD();
    
    myOledViewWorkingHEAT = new MyOledViewWorkingHEAT();
    // ----------- Routes du serveur ----------------
    myServer = new MyServer(PORT);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);

}

void loop() {
  if (lireTemperature == true)
  {
    delay(1000);
    temperature = myTemp->getTemperature();
    sprintf(bufferTemperature, "%4.1f", temperature);
    
  }
  temperatureMax = temperatureSechage + (temperatureSechage/10);
  temperatureMin = temperatureSechage - (temperatureSechage/10);

   afficherOledOff();
   delay(10);
   afficherOledApresDemarrageButton();
  
  //int buttonActionT8 = myButtonT8->checkMyButton();
  int buttonAction  = myButtonAction->checkMyButton();
  if(buttonAction  > 2){
    Serial.println("Button Action pressed");
    //ledOn = false;
  }
  //Gestion du bouton Reset
    int buttonReset = myButtonReset->checkMyButton();
  if(buttonReset > 2)  {  
        Serial.println("Button Reset pressed\n");
        //Le bouton hard reset a été appuyé
        Serial.println("Button Hard reset pressed\n");
        Serial.println("Suppression des réglages et redémarrage...\n");
        ///wm.resetSettings();
        ///ESP.restart();
    }
    delay(500);
  }


  void afficherOledOff(){//Affichage Oled en mode ready
    if (demarrageFoure==0)
    {
      digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, HIGH);
      digitalWrite(GPIO_PIN_LED_OK_GREEN, LOW);
      digitalWrite(GPIO_PIN_LED_HEAT_YELLOW, LOW);
    }
    myOledViewWorkingOFF->setParams("nomDuSysteme",nomDuSyteme.c_str());  
    myOledViewWorkingOFF->setParams("idDuSysteme",idDuSysteme.c_str());
    myOledViewWorkingOFF->setParams("temperature",bufferTemperature);
    myOledViewWorkingOFF->setParams("statusDuSysteme","Ready");
    myOledViewWorkingOFF->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
    myOled->displayView(myOledViewWorkingOFF);
    
  }
  void afficherOledApresDemarrageButton(){//Achichage Oled après demarrage du bouton

      if (demarrageFoure == 7)
      {
        etatChauffage=true;
        if(temperature >= temperatureMin){//Si la temperature est supérieur ou égal à la temperature minimale.
            etatChauffage = false;
            digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
            digitalWrite(GPIO_PIN_LED_OK_GREEN, LOW);
            digitalWrite(GPIO_PIN_LED_HEAT_YELLOW, HIGH);
            myOledViewWorkingHEAT->setParams("nomDuSysteme",nomDuSyteme.c_str());
            myOledViewWorkingHEAT->setParams("idDuSysteme",idDuSysteme.c_str());
            myOledViewWorkingHEAT->setParams("temperature",bufferTemperature);
            myOledViewWorkingHEAT->setParams("statusDuSysteme","Waiting");
            myOledViewWorkingHEAT->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
            myOled->displayView(myOledViewWorkingHEAT);
            myOled->veilleExit();
            if (compteur<=tempsDeSechage){
                    compteur++;
                    delay(200);
                    sprintf(bufferCompteur, "%d", compteur);
              }

            if (compteur > tempsDeSechage){
                      demarrageFoure = 0;
                      compteur = 0;
                     
            }
            
        }
        if((temperature >= temperatureSechage)){//Si la temperature est supérieur ou égal à la temperature de sechage du bois venant du serveur.
                
                etatChauffage = true;
                digitalWrite(GPIO_PIN_LED_OK_GREEN, HIGH);
                digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
                digitalWrite(GPIO_PIN_LED_HEAT_YELLOW, LOW);
                myOledViewWorkingHEAT->setParams("nomDuSysteme",nomDuSyteme.c_str());
                myOledViewWorkingHEAT->setParams("idDuSysteme",idDuSysteme.c_str());
                myOledViewWorkingHEAT->setParams("temperature",bufferTemperature);
                myOledViewWorkingHEAT->setParams("statusDuSysteme","Heating");
                myOledViewWorkingHEAT->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
                myOled->displayView(myOledViewWorkingHEAT);
                
                  if (compteur<=tempsDeSechage)
                  {
                    compteur++;
                    delay(200);
                    sprintf(bufferCompteur, "%d", compteur);
                  }

                  if (compteur > tempsDeSechage){
                      demarrageFoure = 0;
                      compteur = 0;
                  }
              
                
            
          }
          
          if (temperatureMin > temperature){//Si la temperature est inférieur à la temperature seuil minimale.
            compteur=0;
            demarrageFoure = 0;
            myOledViewWorkingCOLD->setParams("nomDuSysteme",nomDuSyteme.c_str());
            myOledViewWorkingCOLD->setParams("idDuSysteme",idDuSysteme.c_str());
            myOledViewWorkingCOLD->setParams("temperature",bufferTemperature);
            myOledViewWorkingCOLD->setParams("statusDuSysteme","COLD");
            myOledViewWorkingCOLD->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
            myOled->displayView(myOledViewWorkingCOLD);
            
          }
          
          if (temperatureMax < temperature){//Si la temperature est supérieur à la temperature seuil maximale.
           
            myOledViewWorkingHEAT->setParams("nomDuSysteme",nomDuSyteme.c_str());
            myOledViewWorkingHEAT->setParams("idDuSysteme",idDuSysteme.c_str());
            myOledViewWorkingHEAT->setParams("temperature",bufferTemperature);
            myOledViewWorkingHEAT->setParams("statusDuSysteme","Heating");
            myOledViewWorkingHEAT->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
            myOled->displayView(myOledViewWorkingHEAT);
          }
          
      }
      
      temperature = myTemp->getTemperature();
      sprintf(bufferTemperature, "%4.1f", temperature);
      
      
    }