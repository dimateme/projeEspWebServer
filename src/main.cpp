#include <Arduino.h>
#include <iostream>
#include <string>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include "TemperatureStub.h"
#include <ArduinoJson.h>

#include <string>
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

#include "MyOledViewWorkingCOLD.h"
MyOledViewWorkingCOLD *myOledViewWorkingCOLD = NULL;

#include "MyOledViewWorkingHEAT.h"
MyOledViewWorkingHEAT *myOledViewWorkingHEAT = NULL;

//sensor dhht22
#include <DHT.h>
#define DHTPIN 27
#define DHTTYPE    DHT22
TemperatureStub *myTemp;

#include "MyOledViewInitialisation.h"
MyOledViewInitialisation *myOledViewInitialisation = NULL;

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
#define GPIO_PIN_LED_HEAT_BLUE        25 //GPIO27
 

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";

String nomDuSyteme = "SAC System";
String iDDuSyteme = "SD777";
String ipDuSyteme = "172.16.11.224";
String idDuSysteme = "92894";
String statusDuSyteme = "";

char bufferTemperature[100];//le buffer permet de stocker la valeur de la température et de la convertir en string
char bufferCompteur[100];//le buffer permet de stocker la valeur du compteur et de la convertir en string
int demarrageFoure = 0; //C'est une variable qui prend la valeur 7 si le four est démarré
float temperature=0.0f;//la température du sensor dht22
bool demarrerFour = false; //Elle permet de savoir si le four est démarré. Si true alors le four est démarré
bool lireTemperature = false; //La temperature est lue si elle est true
int compteur = 0; //Le compteur de temps
float temperatureMax = 0.0f; //La température maximale
 float temperatureMin = 0.0f; //La température minimale
int tempsDeSechage=0;
bool etatChauffage = false; //Elle permet de savoir si le four est démarré. Si true alors le four est démarré
float temperatureSechage=0.0f;

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
      demarrageFoure = 7;
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
    
    char bufferTemperatureSechage[100];
    char buffertempsDeSechage[100];
   
    if (string(actionToDo.c_str()).compare(string("tellCaracteristique")) == 0)
    {
      sprintf(bufferTemperatureSechage, "%s", arg2.c_str());
      temperatureSechage =atof(bufferTemperatureSechage);
      sprintf(buffertempsDeSechage, "%s", arg4.c_str());
      tempsDeSechage=atoi(buffertempsDeSechage);
      //Serial.println(temperatureBois);
      //Serial.println(buffertempsDeSechage);
      
      /* code */
    }
    
    std::string result = "";
    return result;
    }

void afficherOledOff();
void afficherOledApresDemarrageButton();
void setup() { 
    Serial.begin(9600);
    delay(100);

   etatChauffage = false;
    //initialisation de myOledViewWifiAp
   myOledViewWifiAp = new MyOledViewWifiAp();
 
  // initialisation de l'écran oled
    myOled = new MyOled(&Wire, RST,SCREEN_HEIGHT, SCREEN_WIDTH);
    myOled->init(addrI2C,true);

    //Initialisation des LED statuts
    pinMode(GPIO_PIN_LED_LOCK_ROUGE, OUTPUT);
    pinMode(GPIO_PIN_LED_OK_GREEN, OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_BLUE, OUTPUT);

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
       
            //Affichage de l'accès du reseau wifi sur l'ecran oled
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
                digitalWrite(GPIO_PIN_LED_HEAT_BLUE, HIGH);
                delay(1000);
                digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
                digitalWrite(GPIO_PIN_LED_OK_GREEN, LOW);
                digitalWrite(GPIO_PIN_LED_HEAT_BLUE, LOW);
                delay(1000);
            }
           
          Serial.println("Connexion Établie.");
        }

    
    

    //température
    myTemp = new TemperatureStub();
    myTemp->init(DHTPIN, DHT22);
    temperature = myTemp->getTemperature();
    sprintf(bufferTemperature, "%4.1f", temperature);
   //Affichage Oled en mode ready
    myOledViewWorkingOFF = new MyOledViewWorkingOFF();
    
    //delay(1000);


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


  void afficherOledOff(){
    if (demarrageFoure==0)
    {
      digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, HIGH);
      digitalWrite(GPIO_PIN_LED_OK_GREEN, LOW);
      digitalWrite(GPIO_PIN_LED_HEAT_BLUE, LOW);
    }
    
    
    myOledViewWorkingOFF->setParams("nomDuSysteme",nomDuSyteme.c_str());  
    myOledViewWorkingOFF->setParams("idDuSysteme",idDuSysteme.c_str());
    myOledViewWorkingOFF->setParams("temperature",bufferTemperature);
    myOledViewWorkingOFF->setParams("statusDuSysteme","Ready");
    myOledViewWorkingOFF->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
    myOled->displayView(myOledViewWorkingOFF);
    
  }
  void afficherOledApresDemarrageButton(){
      if (demarrageFoure == 7)
      {
        if(temperature >= temperatureMin){
            if (etatChauffage==false)
            {
              digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
              digitalWrite(GPIO_PIN_LED_OK_GREEN, LOW);
              digitalWrite(GPIO_PIN_LED_HEAT_BLUE, HIGH);
            }
            
            myOledViewWorkingHEAT->setParams("nomDuSysteme",nomDuSyteme.c_str());
            myOledViewWorkingHEAT->setParams("idDuSysteme",idDuSysteme.c_str());
            myOledViewWorkingHEAT->setParams("temperature",bufferTemperature);
            myOledViewWorkingHEAT->setParams("statusDuSysteme","Waiting");
            myOledViewWorkingHEAT->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
            myOled->displayView(myOledViewWorkingHEAT);
            myOled->veilleExit();
            
        }
        if((temperature >= temperatureSechage)){
                etatChauffage=true;
                digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
                digitalWrite(GPIO_PIN_LED_OK_GREEN, HIGH);
                digitalWrite(GPIO_PIN_LED_HEAT_BLUE, LOW);
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
          
          if (temperatureMin > temperature){
            compteur=0;
            demarrageFoure = 0;
            myOledViewWorkingCOLD->setParams("nomDuSysteme",nomDuSyteme.c_str());
            myOledViewWorkingCOLD->setParams("idDuSysteme",idDuSysteme.c_str());
            myOledViewWorkingCOLD->setParams("temperature",bufferTemperature);
            myOledViewWorkingCOLD->setParams("statusDuSysteme","COLD");
            myOledViewWorkingCOLD->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
            myOled->displayView(myOledViewWorkingCOLD);
            //myOled->veilleDelay(10);
          }
          
          if (temperatureMax < temperature)
          {
  
            myOledViewWorkingHEAT->setParams("nomDuSysteme",nomDuSyteme.c_str());
            myOledViewWorkingHEAT->setParams("idDuSysteme",idDuSysteme.c_str());
            myOledViewWorkingHEAT->setParams("temperature",bufferTemperature);
            myOledViewWorkingHEAT->setParams("statusDuSysteme","Heating");
            myOledViewWorkingHEAT->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
            myOled->displayView(myOledViewWorkingHEAT);
            sprintf(bufferCompteur, "%d", compteur);
          }
          
      }
      
      
      //ici c'est pour afficher la temperature :test
      temperature = myTemp->getTemperature();
      sprintf(bufferTemperature, "%4.1f", temperature);
      
      
      
      
    }
