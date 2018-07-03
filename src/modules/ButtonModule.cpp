#include "ButtonModule.h"


void ButtonModule::config(CMMC_System *os, AsyncWebServer* server) {
  static ButtonModule *that = this;
}

void ButtonModule::configLoop() { }

void ButtonModule::setup(int button = 13) {
  buttonPin = button;
  pinMode(buttonPin, INPUT_PULLUP);
}

void ButtonModule::loop() { 
  if(digitalRead(buttonPin) == 0) {
    delay(200);
    Serial.println("buttonPin press");
  }

}
