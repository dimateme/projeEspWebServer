/**
    Class MyOledViewWorkingOFF :  Gestion d'une VUE pour le Wifi lorsqu'il n'y a pas de connexion
    @file MyOledViewWorkingOFF.h 
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
#ifndef MYOLEDVIEWWORKINGOFF_H
#define MYOLEDVIEWWORKINGOFF_H

#include <Adafruit_SSD1306.h>
#include "MyOledViewWorking.h"
#include <string>
using namespace std;
class MyOledViewWorkingOFF: public MyOledViewWorking{
private:
    /* Methode */
    void display(Adafruit_SSD1306 *adafruit);
    
public:
  
};
#endif


