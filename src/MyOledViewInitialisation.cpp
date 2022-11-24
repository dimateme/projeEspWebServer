/**
    Gestion d'une VUE pour le OLed : Base pour les écrans de travail
    @file MyOledViewWorking.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyOledViewInitialisation.h"

using namespace std;
void MyOledViewInitialisation::setIdDuSysteme(string val) {
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
    Serial.println("MyOledViewInitialisation");
    adafruit->display();
    }
void MyOledViewInitialisation::update(Adafruit_SSD1306 *adafruit) {
     adafruit->setTextColor(WHITE);
    Serial.println("Update my view MyOledViewInitialisation");
    adafruit->display();
    }