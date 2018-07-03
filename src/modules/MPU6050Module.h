#include <CMMC_Module.h>
#include <CMMC_MPU6050.hpp>

#ifndef CMMC_MPU6050_MODULE_H
#define CMMC_MPU6050_MODULE_H

class MPU6050Module : public CMMC_Module
{
public:
  void config(CMMC_System *os, AsyncWebServer *server);
  void setup();
  void loop();
  void configLoop();

protected:
private:
  CMMC_SENSOR_DATA_T data;
  CMMC_MPU6050 *mpu;
};

#endif