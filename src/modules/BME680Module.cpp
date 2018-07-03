#include "BME680Module.h"
extern int temp;

CMMC_SENSOR_DATA_T data;
void BME680Module::config(CMMC_System *os, AsyncWebServer* server) {
  static BME680Module *that = this;
}

void BME680Module::configLoop() { }

void BME680Module::setup() {
  bme = new CMMC_BME680();
  bme->onData([&](void *d, size_t len) {
    Serial.printf("DATA %lu bytes ARRIVED at %lu\r\n", len, millis()); 
    memcpy(&data, d, len);
    temp = data.field1;
    Serial.printf("onData temp = %d\r\n", temp);
  }); 
  bme->every(3000);
  bme->setup(0, 0);
}

void BME680Module::loop() { 
  bme->read(); 
}
