#include "MPU6050Module.h"

extern float axisX;
extern float axisY;
extern float axisZ;

void MPU6050Module::config(CMMC_System *os, AsyncWebServer *server)
{
  static MPU6050Module *that = this;
}

void MPU6050Module::configLoop() {}

void MPU6050Module::setup()
{
  mpu = new CMMC_MPU6050();
  mpu->onData([&](void *d, size_t len) {
    Serial.printf("DATA %lu bytes ARRIVED at %lu\r\n", len, millis());
    memcpy(&data, d, len);
    axisX = data.field1 / 100;
    axisY = data.field2 / 100;
    Serial.printf("onData X=%2f  Y=%2f\r\n", axisX, axisY);
  });
  mpu->every(1000);
  mpu->setup(4, 5); //  I2C pin SDA:4, SCL:5
}

void MPU6050Module::loop()
{
  mpu->read(); 
}
