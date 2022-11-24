/**
    Gestion d'une VUE pour la temperature lorsqu'il fait froid 
    @file MyOledViewWorkingCOLD.cpp
    @author Jean paul Bai
    @version 1.0 2022/11/11
*/

#include <Arduino.h> 
#include "MyOledViewWorkingCOLD.h"
using namespace std;

void MyOledViewWorkingCOLD::display(Adafruit_SSD1306 *adafruit) {
    adafruit->setTextColor(WHITE);
    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print("SAC System");
   

    adafruit->setTextSize(2);
    adafruit->setCursor(0, 10);
    adafruit->print("nomDuSysteme");

    
    adafruit->display();
}
void MyOledViewWorkingCOLD::update(Adafruit_SSD1306 *adafruit) {
    adafruit->clearDisplay();
    adafruit->setTextSize(1);
    adafruit->setTextColor(WHITE);
    adafruit->setCursor(0, 0);
    adafruit->display();
}
