
#include <CMMC_Legend.h>
#include <CMMC_Sensor.h>

#include <modules/WiFiModule.h>
#include <modules/MqttModule.h>


CMMC_Legend os;
int temp;
// float axisX = 0.0;
// float axisY = 0.0;
// float axisZ = 0.0;

void setup()
{
   os.addModule(new WiFiModule()); 
  os.addModule(new MqttModule());
  os.setup();
  Serial.printf("APP VERSION: %s\r\n", LEGEND_APP_VERSION);
}

void loop()
{
  os.run();
}