/**
    Class MyOledViewErrorWifiConnexion :  Gestion d'une VUE pour le Wifi
    @file MyOledViewErrorWifiConnexion.h
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
#ifndef MYOLEDVIEWERRORWIFICONNEXION_H
#define MYOLEDVIEWERRORWIFICONNEXION_H

#include <Adafruit_SSD1306.h>
#include "MyOledViewErrorWifiConnexion.h"
#include "MyOledView.h"
#include <string>

using namespace std;

class MyOledViewErrorWifiConnexion: public MyOledView{
private:
    /*Les attributs privés*/
    string nomDuSysteme;
    /*Les methodes privées*/
   void display(Adafruit_SSD1306 *adafruit);
    void update(Adafruit_SSD1306 *adafruit);
public:
     //string nomDuSysteme;
    void setNomDuSysteme(string val);
};

#endif