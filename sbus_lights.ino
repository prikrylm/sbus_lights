/*
světla na s.bus, myšlenky
 - co kanál to světlo, možná někdy 2 na blinkry
 - podle hodnotry kanálu různé režimy svícení blikání
   - zhasnuto
   - svítí
   - bliká jako blinkr (střída 1:1, perioda ~ 1s)
   - bliká jako rainlight na F1
   - flash ala letadlo (střída 1:10, perioda ~ 1s)
   - double flash ala letadlo
   - flash a double flash bude pro různé kanály jakoby sekvenční - napřed blikne kanál 1 pak 2 ...
   - možná něco dalšího

   google flash pattern
   https://aviation.stackexchange.com/questions/24151/airplane-strobe-light-pattern
*/

// #define dbg

#include <Arduino.h>
#include <IWatchdog.h>
#include "sbus.h"
#include "light.h"
#include "sbus_lights.globals.h"

#define INTERNAL_LED PC13
#define CONFIG_PIN PC14
#define LED_COUNT 8

#if !defined(STM32_CORE_VERSION) || (STM32_CORE_VERSION < 0x01090000)
#error "Due to API change, this sketch is compatible with STM32_CORE_VERSION  >= 0x01090000"
#endif

//HardwareSerial Serial2(PB11, PB10); // pro s.bus C8
HardwareSerial Serial2(PA10, PA9); // pro s.bus C6, C8
SBUS sbus(Serial2);
TIM_TypeDef *sbusTimer = TIM2; // TIM1 se pouziva na analog write

LightConfig *lightConfig;
Light *light[LED_COUNT];

int first_LED = 1;

void tick()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    light[i]->tick();
  }

  if (Serial2.available() > 24)
  {

    if (sbus.process())
    {
      for (int i = 0; i < LED_COUNT; i++)
      {
        light[i]->setValue(sbus.getChannel(i + first_LED));
      }
      // FAIL
      digitalWrite(INTERNAL_LED, !sbus.getFailsafeStatus()); //???
#ifndef dbg      
      IWatchdog.reload();
#endif      
    }
  }
}

void setup()
{
  sbus.begin();
  pinMode(INTERNAL_LED, OUTPUT); // RX failsafe indikator - svítí = failsafe
  digitalWrite(INTERNAL_LED, 0); //sviti, pokud bude sbus ok, zhasne

  pinMode(CONFIG_PIN, INPUT_PULLUP);
  if (digitalRead(CONFIG_PIN) == 0) {
    first_LED = 9;
  }

#ifdef dbg

  Serial.begin(1200);
  Serial.print("init ");
#endif

  lightConfig = Light::createSetupConfig();

  light[0] = new Light(PA0, lightConfig);
  light[1] = new Light(PA1, lightConfig);
  light[2] = new Light(PA2, lightConfig);
  light[3] = new Light(PA3, lightConfig);
  light[4] = new Light(PA4, lightConfig);
  light[5] = new Light(PA5, lightConfig);
  light[6] = new Light(PA6, lightConfig);
  light[7] = new Light(PA7, lightConfig);

  for (int i = 0; i < LED_COUNT; i++)
  {
    light[i]->setInverted(true);
  }

  HardwareTimer *MyTim = new HardwareTimer(sbusTimer);
  // timer
  MyTim->setOverflow(1000, HERTZ_FORMAT);
  MyTim->attachInterrupt(tick);
  MyTim->resume();
#ifndef dbg
  IWatchdog.begin(5000000); // 5s
#endif
}

void loop()
{

#ifdef dbg
  // debug
    Serial.print(sbus.getGoodFrames());
    Serial.print(" - ");
    Serial.print(sbus.getLostFrames());
    Serial.print("\n");
    delay(1000);
#endif
}
