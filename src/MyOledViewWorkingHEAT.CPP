/**
    Gestion d'une VUE pour la temperature lorsqu'il fait froid 
    @file MyOledViewWorkingCOLD.cpp
    @author Jean paul Bai
    @version 1.0 2022/11/11
*/

#include <Arduino.h> 
#include "MyOledViewWorkingHEAT.h"
using namespace std;

void MyOledViewWorkingHEAT::display(Adafruit_SSD1306 *adafruit) {
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
    adafruit->setCursor(70, 20);
    adafruit->print(getTag("statusDuSysteme").c_str());

    adafruit->setTextSize(2);
    adafruit->setCursor(10, 30);
    adafruit->print(getTag("temperature").c_str());
    //displayGifFire(adafruit, 20, 50);
    adafruit->setTextSize(1);
    adafruit->setCursor(30, 50);
    adafruit->print(getTag("ipDuSysteme").c_str());
    adafruit->display();
}
void MyOledViewWorkingHEAT::update(Adafruit_SSD1306 *adafruit) {
    adafruit->clearDisplay();
    adafruit->setTextSize(1);
    adafruit->setTextColor(WHITE);
    adafruit->setCursor(0, 0);
    adafruit->display();
}


