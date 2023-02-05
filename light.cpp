#include "light.h"
#include <Arduino.h>
#include "sbus_lights.globals.h"

LightConfig::LightConfig(int val) {
    valTo = val;
    next = NULL;
    step = NULL;
}

void LightConfig::AddStep(int state, int dur) {
    LightStep *s = new LightStep();
    s->duration = dur;
    s->lightState = state;
    s->next = step;

    if (step == NULL) {
        step = s;
        step->next = step; // next je null, tak proto
    } else {
        // zaradit na konec, alias pred step
        LightStep *last = step;
        while(last->next != step) {
            last = last->next;
        }
        last->next = s;
    }
}

// TODO zatridit
//
LightConfig * LightConfig::AddConfig(int valTo) {
  LightConfig *retval = new LightConfig(valTo);
  LightConfig *ptr;

    ptr = this;
    while (ptr->next != NULL) {
      ptr = ptr->next;
    }
    ptr->next = retval;
    
  return retval;
}

Light::Light(int pin, LightConfig *conf)
{
    pinNo = pin;
    pinMode(pin, OUTPUT);
    config = conf;
    stepCnt = 0;
    actualConfig = conf;
    actualStep = actualConfig->step;
    inverted = false;
}

void Light::setInverted(bool inv)
{
    inverted = inv;
}

void Light::setValue(int val)
{
    LightConfig *ptr = config;
    while (ptr->next != NULL && ptr->valTo < val)
    {
        ptr = ptr->next;
    }
    if (actualConfig != ptr)
    {
        actualConfig = ptr;
        stepCnt = 0;
        actualStep = actualConfig->step;
        if (actualStep != NULL) {
            output(actualStep->lightState);
        } else {
            output(0);
        }
    }
}

void Light::output(int v) {
    digitalWrite(pinNo, v == 1 ? !inverted : inverted);
}
// budu pocitat tiky pro jednotlive stavy
void Light::tick()
{

    if (actualStep != NULL) {
        stepCnt++;
        if (actualStep->duration < stepCnt) {
            stepCnt = 0;
            actualStep = actualStep->next;
            output(actualStep->lightState);
        }
    } else {
        output(0);
    }
}
