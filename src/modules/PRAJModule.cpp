#include "PRAJModule.h"
// extern uint32_t user_espnow_sent_at;


void PRAJModule::config(CMMC_System *os, AsyncWebServer* server) {
  static PRAJModule *that = this;
}

void PRAJModule::configLoop() { }

void PRAJModule::setup() {
}

void PRAJModule::loop() { 
  if (millis() % 1000 == 0) {
  }
}
