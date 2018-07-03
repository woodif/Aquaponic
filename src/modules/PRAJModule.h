#include <CMMC_Module.h>
#include <Wire.h>

#ifndef CMMC_PRAJ_MODULE_H
#define CMMC_PRAJ_MODULE_H 

class PRAJModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void configLoop();
  protected:
  private:
  
};

#endif