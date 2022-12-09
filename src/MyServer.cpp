/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyServer.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
using namespace std;


typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

//Exemple pour appeler une fonction CallBack
//if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend); 
void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;


}

void MyServer::initAllRoutes() { 
    currentTemperature = 3.3f;

    //Initialisation du SPIFF.
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
        }

    //Route initiale (page html)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
        });

    //Route du script JavaScript
    this->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script.js", "text/javascript");
        });
    
    this->on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.css", "text/css");
        });
    this->on("/logo_SAC.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/logo_SAC.png", "image/png");
        });
   this->on("/lireTemp", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTempFour");
        String lireTempDuFour =String(repString.c_str());
        request->send(200, "text/plain", lireTempDuFour );
    });
    this->on("/DemarrerFour", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("actionToDo", true)) {
            String actionToDo = request->getParam("actionToDo", true)->value();
            if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {
                if (ptrToCallBackFunction)(*ptrToCallBackFunction)("startAction");
            }
        }
        request->send(204); 
    });
    //compteure de temps
    this->on("/lireCompteure", HTTP_GET, [](AsyncWebServerRequest *request) {
            std::string repString = "";
            if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askCompteur");
            String getCompteur =String(repString.c_str());
            request->send(200, "text/plain", getCompteur );
    });
    this->on("/getAllWoodOptions", HTTP_GET, [](AsyncWebServerRequest *request) {
        HTTPClient http;
        String apiRestAddress="http://51.79.50.149:3000/typebois/nom";//api renvoie la liste des bois
        http.begin(apiRestAddress);
        http.GET();
        String response = http.getString();
       request->send(200, "text/plain", response);
    });
    this->on("/getCaracteristiqueBois", HTTP_POST, [](AsyncWebServerRequest *request) {
        String id =""; 
        if (request->hasParam("actionBoisId", true)) {
            id = request->getParam("actionBoisId", true)->value();
        }
        if(id!=""){
            HTTPClient http;
            String apiRestAddress="http://51.79.50.149:3000/typebois/"+String(id);//cette api renvoie les caracteristiques du bois
            http.begin(apiRestAddress);
            http.GET();
            String response = http.getString();
            //nouveau
            String response2 = response;
            char json[500];
            response2.replace(" ","");
            response2.replace("\n","");
            response2.trim();
            response2.remove(0,1);
            response2.toCharArray(json, 100);
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, json);
            String temps = doc["temps"];
            String temperature = doc["temperature"];
            string repString = "";
            String stringToSend = "tellCaracteristique ";
            stringToSend +=String("temperature") +String(" ") +temperature + String(" ");
            stringToSend +=String("temps") + String(" ") + temps + String(" ");
                
            if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)(stringToSend.c_str());
            request->send(200, "text/plain", response);
            
        }
        request->send(204);
    });
    this->begin();
    
    
};