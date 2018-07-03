
#ifndef CMMC_WC_ESPNow_MODULE_H
#define CMMC_WC_ESPNow_MODULE_H
#define CMMC_USE_ALIAS

#include <CMMC_Legend.h>
#include <CMMC_Utils.h>
#include <CMMC_Module.h>
#include <CMMC_ESPNow.h>
#include <CMMC_SimplePair.h>
#include <CMMC_Sensor.h>
#include <CMMC_LED.h>
#include "HX711.h"

#define BUTTON_PIN 0
class WC_ESPNowModule : public CMMC_Module
{
public:
  void config(CMMC_System *os, AsyncWebServer *server);
  void configLoop();
  void setup();
  void loop();

private:
  CMMC_SENSOR_DATA_T _userPacket;
  uint8_t isCrashed = 0;
  uint8_t _defaultDeepSleep_m = 30;
  CMMC_System *os;
  CMMC_ESPNow espNow;
  CMMC_SimplePair simplePair;
  CMMC_LED *led;
  uint8_t self_mac[6];
  uint8_t master_mac[6];
  bool sp_flag_done = false;
  void _init_simple_pair();
  void _go_sleep(uint32_t deepSleepM);
  void _init_espnow();

  // HX711 variable
  float get_units_kg1();
  float get_units_kg2();
  void get_average();

  float calibration_factor1 = 11636.00;
  uint32_t zero_factor1 = 8123913;
  float calibration_factor2 = 12120.00;
  uint32_t zero_factor2 = 7842768;

  int numReadings;
  float readings1[10];
  float readings2[10];
  int readIndex = 0;
  double average = 0;
  uint32_t arrayScale[1000];
  unsigned long standCount = 0;
  boolean standState = false;

  // int DOUT1 = 4;
  // int CLK1 = 5;
  // int DOUT2 = 12;
  // int CLK2 = 14;
  HX711 *scale1;
  HX711 *scale2;
  // HX711 scale1(4, 5);
  // HX711 scale2(12, 14);
};
#endif