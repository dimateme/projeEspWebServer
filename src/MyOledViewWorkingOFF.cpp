/**
    Gestion d'une VUE pour le wifi lorsqu'il n'y a pas de connexion
    @file MyOledViewWorkingOFF.cpp
    @author Jean paul Bai
    @version 1.0 2022/11/11
*/

#include <Arduino.h> 
#include "MyOledViewWorkingOFF.h"
using namespace std;

void MyOledViewWorkingOFF::display(Adafruit_SSD1306 *adafruit) {
    
    
    adafruit->clearDisplay();
    adafruit->setTextColor(WHITE);
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(getTag("nomDuSysteme").c_str());
    
    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(getTag("idDuSysteme").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(90, 20);
    adafruit->print(getTag("statusDuSysteme").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(90, 30);
    adafruit->print(getTag("temperature").c_str());
    //adafruit->print("23.00");

    adafruit->setTextSize(1);
    adafruit->setCursor(30, 50);
    adafruit->print(getTag("ipDuSysteme").c_str());
    adafruit->display();
}

