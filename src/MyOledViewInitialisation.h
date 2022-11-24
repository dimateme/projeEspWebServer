/**
    Class MyOledViewInitialisation :  Gestion d'une VUE pour lors de l'initialisation de l'application
    @file MyOledViewInitialisation.h 
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
#ifndef MYOLEDVIEWINITIALISATION_H
#define MYOLEDVIEWINITIALISATION_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"
#include <string>
using namespace std;
class MyOledViewInitialisation: public MyOledView{
private:
     string nomDuSysteme;
     string idDDuSysteme;
     string SensibiliteBoutonAction;
     string SensibiliteBoutonReset;
    /* Methode */
    
    void display(Adafruit_SSD1306 *adafruit);
    void update(Adafruit_SSD1306 *adafruit);

public:
    void setNomDuSysteme(string val);
    void setIdDuSysteme(string val);
    void setSensibiliteBoutonAction(string val);
    void setSensibiliteBoutonReset(string val);
};
#endif


