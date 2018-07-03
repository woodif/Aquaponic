#include "WC_ESPNowModule.h"

void WC_ESPNowModule::config(CMMC_System *os, AsyncWebServer *server)
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  uint8_t *slave_addr = CMMC::getESPNowSlaveMacAddress();
  memcpy(self_mac, slave_addr, 6);
  this->led = ((CMMC_Legend *)os)->getBlinker();
  ;

  strcpy(this->path, "/api/espnow");

  static WC_ESPNowModule *that = this;
  this->os = os;
  this->_serverPtr = server;
  this->_managerPtr = new CMMC_ConfigManager("/espnow.json");
  this->_managerPtr->init();

  this->_managerPtr->load_config([](JsonObject *root, const char *content) {
    if (root == NULL)
      return;
    Serial.println("[user] json loaded..");
    if (root->containsKey("mac"))
    {
      String macStr = String((*root)["mac"].as<const char *>());
      const char *device = (*root)["deviceName"].as<const char *>();
      String deviceName;
      if (device != NULL)
      {
        deviceName = String(device);
      }
      Serial.printf("Loaded mac %s, name=%s\r\n", macStr.c_str(), deviceName.c_str());
      uint8_t mac[6];
      CMMC::convertMacStringToUint8(macStr.c_str(), mac);
      memcpy(that->master_mac, mac, 6);
    }
    else
    {
      Serial.println("no mac field.");
    }
  });
  this->configWebServer();
}

void WC_ESPNowModule::loop()
{
  get_average();

  standCount = 0;
  while (average > 50.00)
  {
    get_average();
    arrayScale[standCount] = average;
    // Serial.println(arrayScale[standCount]);
    delay(10);

    standCount += 1;
    standState = true;
  }

  if (standState == true)
  {
    int32_t arraySize = standCount;
    arraySize = arraySize / 5;

    double sumAVE = 0;
    for (int i = arraySize; i < (standCount - arraySize); i++)
    {
      sumAVE += arrayScale[i];
    }

    sumAVE = sumAVE / ((standCount - arraySize) - arraySize);
    Serial.println(sumAVE);

    _userPacket.field1 = sumAVE*1000;
    strcpy(_userPacket.sensorName, "SMART-WC");
    _userPacket.nameLen = strlen(_userPacket.sensorName);
    espNow.send(master_mac, (u8 *)&_userPacket, sizeof(_userPacket), []() {
      Serial.println("espnow sending timeout.");
    },500);

    standCount = 0;
    standState = false;
  }
}

void WC_ESPNowModule::configLoop()
{
  if (digitalRead(BUTTON_PIN) == 0)
  {
    _init_simple_pair();
    delay(1000);
  }
}

void WC_ESPNowModule::setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(15, OUTPUT);

  numReadings = 10;
  // HX711 scale1(4, 5);
  // HX711 scale2(12, 14);

  scale1 = new HX711(4, 5);
  scale2 = new HX711(12, 14);

  scale1->set_scale(calibration_factor1);
  scale1->set_offset(zero_factor1);
  scale2->set_scale(calibration_factor2);
  scale2->set_offset(zero_factor2);

  _init_espnow();
}

void WC_ESPNowModule::_init_espnow()
{
  espNow.init(NOW_MODE_SLAVE);
  espNow.enable_retries(true);
  static CMMC_LED *led;
  led = ((CMMC_Legend *)os)->getBlinker();
  led->detach();
  espNow.on_message_sent([](uint8_t *macaddr, u8 status) { led->toggle(); });

  static WC_ESPNowModule *module;
  module = this;

  // espNow.on_message_recv([](uint8_t *macaddr, uint8_t *data, uint8_t len) {
  //   Serial.printf("RECV: len = %u byte, isCrashed = %lu at(%lu ms)\r\n", len, data[0], millis());
  //   module->isCrashed = data[0];
  //   Serial.printf("[class] isCrashed = %u\r\n", module->isCrashed);
  //   ;
  //   if (module->isCrashed)
  //   {
  //     led->blink(50);
  //   }
  //   else
  //   {
  //     led->detach();
  //     led->high(); // led off
  //   }
  // });
}

void WC_ESPNowModule::_init_simple_pair()
{
  Serial.println("calling simple pair.");
  this->led->blink(250);
  // simplePair.debug([](const char* msg) { Serial.println(msg); });
  static WC_ESPNowModule *module = this;
  static bool *flag = &sp_flag_done;
  simplePair.begin(SLAVE_MODE, [](u8 status, u8 *sa, const u8 *data) {
    Serial.println("evt_callback.");
    if (status == 0)
    {
      char buf[13];
      char self_buf[13];
      Serial.printf("[CSP_EVENT_SUCCESS] STATUS: %d\r\n", status);
      Serial.printf("WITH KEY: ");
      CMMC::dump(data, 16);
      Serial.printf("WITH MAC: ");
      CMMC::dump(sa, 6);
      CMMC::macByteToString(data, buf);
      CMMC::macByteToString(module->self_mac, self_buf);
      CMMC::printMacAddress((uint8_t *)buf);
      CMMC::printMacAddress((uint8_t *)self_buf);
      module->_managerPtr->add_field("mac", buf);
      module->_managerPtr->add_field("self_mac", self_buf);
      module->_managerPtr->commit();
      Serial.println("DONE...");
      *flag = true;
    }
    else
    {
      Serial.printf("[CSP_EVENT_ERROR] %d: %s\r\n", status, (const char *)data);
    }
  });

  simplePair.start();
  uint32_t startMs = millis();
  while (!sp_flag_done && (millis() - startMs < 10000))
  {
    Serial.println("waiting sp_flag_done ..");
    delay(1000L + (250 * sp_flag_done));
  }
  if (sp_flag_done)
  {
    module->led->blink(1000);
    delay(5000);
    ESP.restart();
    Serial.println("pair done.");
  }
  else
  {
    Serial.println("do simple pair device not found.");
    module->led->blink(50);
  }
}

void WC_ESPNowModule::_go_sleep(uint32_t deepSleepM)
{
  // deepSleepM = 1;
  Serial.printf("\r\nGo sleep for %lu min.\r\n", deepSleepM);
  Serial.println("bye!");

  ESP.deepSleep(deepSleepM * 60e6);
  Serial.println("not be reached here.");
}

float WC_ESPNowModule::get_units_kg1()
{
  return (scale1->get_units() * 0.453592);
}

float WC_ESPNowModule::get_units_kg2()
{
  return (scale2->get_units() * 0.453592);
}

void WC_ESPNowModule::get_average()
{
  readings1[readIndex] = get_units_kg1(); // * 1000;
  readings2[readIndex] = get_units_kg2(); // * 1000;

  readIndex++;
  if (readIndex >= numReadings)
  {
    readIndex = 0;
  }

  average = 0;
  for (int x = 0; x < numReadings; x++)
  {
    average += (readings1[x] + readings2[x]);
  }

  average = (average / numReadings) - 2.65;

  // Serial.print("average = ");
  // Serial.println(average);
}