/**
    Class MyOledViewWifiAp :  Gestion d'une VUE pour le Wifi
    @file MyOledViewWifiAp.h 
    @author Jean paul Bai
    @version 1.0 2022/11/11
    


    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.0      2022/11/11   Jean paul Bai         Première version de la classe
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
         Aucune
    Autres librairies (à copier dans le répertoire lib)
        Aucune

**/
#ifndef MYOLEDVIEWWIFIAP_H
#define MYOLEDVIEWWIFIAP_H

#include <Adafruit_SSD1306.h>
//#include "MyOled.h"
#include "MyOledView.h"
using namespace std;

class MyOledViewWifiAp: public MyOledView{
private:
    /*Les attributs privés*/
    string nomDuSysteme;
    string ssIDDuSysteme;
    string passeDuSysteme;
    /*Les methodes privées*/
    void display(Adafruit_SSD1306 *adafruit);
    void update(Adafruit_SSD1306 *adafruit);
public:
    
    void setNomDuSysteme(string val);
    void setSsIDDuSysteme(string val);
    void setPasseDuSysteme(string val);
    
};

#endif