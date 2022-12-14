/**
    Gestion d'une VUE pour le wifi 
    @file MyOledViewWifiAp.cpp
    @author Jean paul Bai
    @version 1.0 2022/11/11
*/

#include <Arduino.h> 
#include "MyOledViewWifiAp.h"
using namespace std;

void MyOledViewWifiAp::setNomDuSysteme(string val) {
    nomDuSysteme = val;
}
void MyOledViewWifiAp::setSsIDDuSysteme(string val) {
    ssIDDuSysteme = val;
}
void MyOledViewWifiAp::setPasseDuSysteme(string val) {
    passeDuSysteme = val;
}
  void MyOledViewWifiAp::display(Adafruit_SSD1306 *adafruit) {
    
    adafruit->setTextColor(WHITE);
    
    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print("SAC System");
   
    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(nomDuSysteme.c_str());
    
    adafruit->setCursor(0, 30);
    adafruit->print("AP Configuration");

    adafruit->setCursor(20, 40);
    adafruit->print("SSID: ");
    adafruit->print(ssIDDuSysteme.c_str());

    adafruit->setCursor(20, 50);
    adafruit->print("PASS: ");
    adafruit->print(passeDuSysteme.c_str());

    adafruit->display();
}
void MyOledViewWifiAp::update(Adafruit_SSD1306 *adafruit) {
    adafruit->clearDisplay();
    adafruit->setTextSize(1);
    adafruit->setTextColor(WHITE);
    adafruit->setCursor(0, 0);
    adafruit->display();
}
