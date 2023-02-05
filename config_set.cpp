#include "light.h"
#include "sbus_lights.globals.h"


LightConfig * Light::createSetupConfig() {
  // 200 - 1800 na 
  // 172, 499, 827, 1154, 1482, 1809 - hodnoty
  LightConfig * cfg;
  LightConfig * auxConfig;
  cfg = new LightConfig(300); //
  auxConfig = cfg;

  auxConfig = auxConfig->AddConfig(650); // blinkr
  auxConfig->AddStep(1, 500);
  auxConfig->AddStep(0, 500); 

  auxConfig = auxConfig->AddConfig(950); // rychly blinkr
  auxConfig->AddStep(1, 200);
  auxConfig->AddStep(0, 200);

  auxConfig = auxConfig->AddConfig(1300); // flash
  auxConfig->AddStep(1, 100);
  auxConfig->AddStep(0, 900);

  auxConfig = auxConfig->AddConfig(1650); // double flash
  auxConfig->AddStep(1, 50);
  auxConfig->AddStep(0, 50);
  auxConfig->AddStep(1, 50);
  auxConfig->AddStep(0, 850);
  
  auxConfig = auxConfig->AddConfig(2000); // rozsviceno
  auxConfig->AddStep(1,1);

  return cfg;
}
