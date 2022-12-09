/**
    Gestion d'une VUE pour le OLed : Base pour les écrans de travail
    @file MyOledViewWorking.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyOledViewInitialisation.h"

using namespace std;
void MyOledViewInitialisation::setNomDuSysteme(string val) {
    nomDuSysteme = val;
    }
void MyOledViewInitialisation::setIdDuSysteme(string val) {
    
    idDDuSysteme = val;
    }
void MyOledViewInitialisation::setSensibiliteBoutonAction(string val) {
    SensibiliteBoutonAction = val;
    }
void MyOledViewInitialisation::setSensibiliteBoutonReset(string val) {
    SensibiliteBoutonReset = val;
    }
void MyOledViewInitialisation::display(Adafruit_SSD1306 *adafruit) {
    
    adafruit->setTextColor(WHITE);
    
    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(nomDuSysteme.c_str());
   
    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(idDDuSysteme.c_str());
    
    adafruit->setCursor(0, 30);
    adafruit->print("INITIALISATION");

    adafruit->setCursor(20, 40);
    adafruit->print("Bouton ACTION: ");
    adafruit->print(SensibiliteBoutonAction.c_str());

    adafruit->setCursor(20, 50);
    adafruit->print("Bouton RESET: ");
    adafruit->print(SensibiliteBoutonReset.c_str());

    adafruit->display();
    }
void MyOledViewInitialisation::update(Adafruit_SSD1306 *adafruit) {
     adafruit->setTextColor(WHITE);
    Serial.println("Update my view MyOledViewInitialisation");
    adafruit->display();
    }