
#ifndef CMMC_BUTTON_MODULE_H
#define CMMC_BUTTON_MODULE_H

#include <CMMC_Module.h>

// #define BUTTON_PIN  13

class ButtonModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup(int button);
    void loop(); 
    void configLoop();
    int buttonPin;
  protected:
  private:
};

#endif