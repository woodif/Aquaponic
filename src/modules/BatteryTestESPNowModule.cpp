#include "BatteryTestESPNowModule.h"

void BatteryTestESPNowModule::config(CMMC_System *os, AsyncWebServer* server) {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  uint8_t* slave_addr = CMMC::getESPNowSlaveMacAddress();
  memcpy(self_mac, slave_addr, 6);
  this->led = ((CMMC_Legend*) os)->getBlinker();;

  strcpy(this->path, "/api/espnow");

  static BatteryTestESPNowModule *that = this;
  this->os = os;
  this->_serverPtr = server;
  this->_managerPtr = new CMMC_ConfigManager("/espnow.json");
  this->_managerPtr->init();

  this->_managerPtr->load_config([](JsonObject * root, const char* content) {
    if (root == NULL) return;
    Serial.println("[user] json loaded..");
    if (root->containsKey("mac")) {
      String macStr = String((*root)["mac"].as<const char*>()); 
      const char* device = (*root)["deviceName"].as<const char*>();
      String deviceName;
      if (device != NULL) {
         deviceName  = String(device);
      } 
      Serial.printf("Loaded mac %s, name=%s\r\n", macStr.c_str(), deviceName.c_str());
      uint8_t mac[6];
      CMMC::convertMacStringToUint8(macStr.c_str(), mac);
      memcpy(that->master_mac, mac, 6);
    }
    else {
      Serial.println("no mac field.");
    }
  });
  this->configWebServer();
} 

void BatteryTestESPNowModule::loop() {
  if(digitalRead(0) == LOW) {
    // dirty = 1;
    isCrashed = 1;
    Serial.println("set crash state ...");
    delay(50);
  } 
  if (millis() % 1000 == 0) {
    Serial.printf("[%lu] send isCrashed = %u\r\n", millis(), isCrashed);
    espNow.send(master_mac, (u8*) &isCrashed, 1, []() {
      Serial.println("espnow sending timeout."); 
    }, 500); 
  }
}

void BatteryTestESPNowModule::configLoop() {
  if (digitalRead(BUTTON_PIN) == 0) {
    _init_simple_pair();
    delay(1000);
  }
}

void BatteryTestESPNowModule::setup() { 
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(15, OUTPUT);
  _init_espnow(); 
} 

void BatteryTestESPNowModule::_init_espnow() {
  espNow.init(NOW_MODE_SLAVE); 
  espNow.enable_retries(true);
  static CMMC_LED *led;
  led = ((CMMC_Legend*) os)->getBlinker();
  led->detach();
  espNow.on_message_sent([](uint8_t *macaddr, u8 status) { led->toggle(); }); 

  static BatteryTestESPNowModule* module; 
  module = this;

  espNow.on_message_recv([](uint8_t * macaddr, uint8_t * data, uint8_t len) {
    Serial.printf("RECV: len = %u byte, isCrashed = %lu at(%lu ms)\r\n", len, data[0], millis());
    module->isCrashed = data[0];
    Serial.printf("[class] isCrashed = %u\r\n", module->isCrashed);;
    if (module->isCrashed) {
      led->blink(50);
    }
    else { 
      led->detach(); 
      led->high(); // led off
    }
  });
}

void BatteryTestESPNowModule::_init_simple_pair() {
  Serial.println("calling simple pair.");
  this->led->blink(250);
  // simplePair.debug([](const char* msg) { Serial.println(msg); });
  static BatteryTestESPNowModule *module = this;
  static bool *flag = &sp_flag_done;
  simplePair.begin(SLAVE_MODE, [](u8 status, u8 * sa, const u8 * data) {
    Serial.println("evt_callback.");
    if (status == 0) {
      char buf[13];
      char self_buf[13];
      Serial.printf("[CSP_EVENT_SUCCESS] STATUS: %d\r\n", status);
      Serial.printf("WITH KEY: ");
      CMMC::dump(data, 16);
      Serial.printf("WITH MAC: ");
      CMMC::dump(sa, 6);
      CMMC::macByteToString(data, buf);
      CMMC::macByteToString(module->self_mac, self_buf);
      CMMC::printMacAddress((uint8_t*)buf);
      CMMC::printMacAddress((uint8_t*)self_buf);
      module->_managerPtr->add_field("mac", buf);
      module->_managerPtr->add_field("self_mac", self_buf);
      module->_managerPtr->commit();
      Serial.println("DONE...");
      *flag = true;
    }
    else {
      Serial.printf("[CSP_EVENT_ERROR] %d: %s\r\n", status, (const char*)data);
    }
  });

  simplePair.start();
  uint32_t startMs = millis();
  while (!sp_flag_done && (millis() - startMs < 10000)) {
    Serial.println("waiting sp_flag_done ..");
    delay(1000L + (250 * sp_flag_done));
  }
  if (sp_flag_done) {
    module->led->blink(1000); 
    delay(5000);
    ESP.restart();
    Serial.println("pair done.");
  }
  else {
    Serial.println("do simple pair device not found.");
    module->led->blink(50);
  }
}

void BatteryTestESPNowModule::_go_sleep(uint32_t deepSleepM) {
  // deepSleepM = 1;
  Serial.printf("\r\nGo sleep for %lu min.\r\n", deepSleepM);
  Serial.println("bye!");
  
  ESP.deepSleep(deepSleepM * 60e6);
  Serial.println("not be reached here.");
} 
