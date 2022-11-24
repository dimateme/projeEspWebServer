/**
    Gestion d'une VUE pour le wifi 
    @file MyOledViewWifiAp.cpp
    @author Jean paul Bai
    @version 1.0 2022/11/11
*/

#include <Arduino.h> 
#include "MyOledViewErrorWifiConnexion.h"
using namespace std;

void MyOledViewErrorWifiConnexion::setNomDuSysteme(string val) {
    nomDuSysteme = val;
}
void MyOledViewErrorWifiConnexion::display(Adafruit_SSD1306 *adafruit) {
    adafruit->setTextColor(WHITE);
    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print("SAC System");
   

    adafruit->setTextSize(2);
    adafruit->setCursor(0, 10);
    adafruit->print("nomDuSysteme");

    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(nomDuSysteme.c_str());
    
    adafruit->setCursor(0, 30);
    adafruit->print("AP Configuration");
    
    adafruit->display();
}
void MyOledViewErrorWifiConnexion::update(Adafruit_SSD1306 *adafruit) {
    adafruit->clearDisplay();
    adafruit->setTextSize(1);
    adafruit->setTextColor(WHITE);
    adafruit->setCursor(0, 0);
    adafruit->display();
}
