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
    adafruit->setTextColor(WHITE);
    
    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print("SAC System");
   

    adafruit->setTextSize(2);
    adafruit->setCursor(0, 10);
    adafruit->print("My Oled View Working OFF");



    adafruit->display();
}

