#include <CMMC_Module.h>
#include <CMMC_BME680.hpp>
#include <Wire.h>
#include <SSD1306.h>

#ifndef CMMC_OLED_MODULE_H
#define CMMC_OLED_MODULE_H 

class OLEDModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void configLoop();
    void printBuffer();
  protected:
  private:
  
    SSD1306 *oled;
};

#endif