/**
    Class MyOledViewWorkingHEAT :  Gestion d'une VUE pour la temperature lorsqu'il fait chaud
    @file MyOledViewWorkingHEAT.h 
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
#ifndef MYOLEDVIEWWORKINGHEAT_H
#define MYOLEDVIEWWORKINGHEAT_H

#include <Adafruit_SSD1306.h>
#include "MyOledViewWorking.h"
#include <string>
class MyOledViewWorkingHEAT: public MyOledViewWorking{
private:
    /* data */
    void display(Adafruit_SSD1306 *adafruit);
    void update(Adafruit_SSD1306 *adafruit);
public:
    void aniamtion(Adafruit_SSD1306 *adafruit);
};
#endif
