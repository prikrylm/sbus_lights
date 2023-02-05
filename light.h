#ifndef LIHGT_h
#define LIGHT_h

#include <Arduino.h>
#include "sbus_lights.globals.h"

struct LightStep {
    int lightState; // on / off zatim 0 1 nekdy mozna jako analog write hodnota
    int duration; // ms
    LightStep *next; 
};


// polozka pole konfigu, valTo je hranicni hodnota, kdy polozka plati
// konfig bude seznam konfigu, kde bude step jako cyklicky seznam

class LightConfig {
    public:
        LightConfig(int val);
        int valTo;
        LightStep *step; // kruhovy seznam kroku
        LightConfig *next; // nasledujici konfig
        void AddStep(int state, int dur);

        LightConfig * AddConfig(int valTo);
};

class Light {
    public:
        Light(int pin, LightConfig *conf);
        void setValue(int val); // sbus value
        void tick();  
        void setInverted(bool inv);
        static LightConfig * createSetupConfig(); 

    private:
        LightConfig *config; // list of LightConfig  sequence
        LightConfig *actualConfig; // config bude seznam, ukazuje na aktualni polozku podle value
        LightStep *actualStep; // index do config[] nastaví se v setValue()
        int stepCnt;
        int pinNo;
        bool inverted; // pro LED spínané nulou

        void output(int v);

};

#endif
