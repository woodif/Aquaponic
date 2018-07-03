
#include <CMMC_Legend.h>
#include <modules/KadyaiModule.h>
#include <modules/ESPNowModule.h>
#include <modules/OLEDModule.h>
#include <modules/MPU6050Module.h>
#include <modules/PRAJModule.h>
#include <modules/PRAJ_ESPNowModule.h>
#include <modules/WC_ESPNowModule.h>
#include <CMMC_Sensor.h>

CMMC_Legend os;

// float axisX = 0.0;
// float axisY = 0.0;
// float axisZ = 0.0;

void setup()
{
  // os.addModule(new KadyaiModule());
  // os.addModule(new ESPNowModule());
  // os.addModule(new OLEDModule());
  // os.addModule(new KadyaiModule());
  // os.addModule(new PRAJModule());
  // os.addModule(new PRAJ_ESPNowModule()); 
  os.addModule(new WC_ESPNowModule()); 
  os.setup();
  Serial.printf("APP VERSION: %s\r\n", LEGEND_APP_VERSION);
}

void loop()
{
  os.run();
}