
#ifndef CMMC_BatteryTestESPNowModule_MODULE_H
#define CMMC_BatteryTestESPNowModule_MODULE_H
#define CMMC_USE_ALIAS 

#include <CMMC_Legend.h>
#include <CMMC_Utils.h>
#include <CMMC_Module.h>
#include <CMMC_ESPNow.h>
#include <CMMC_SimplePair.h>
#include <CMMC_Sensor.h>
#include <CMMC_LED.h>

#define BUTTON_PIN  0
class BatteryTestESPNowModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
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
}; 
#endif