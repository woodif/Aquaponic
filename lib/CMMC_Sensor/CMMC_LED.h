#ifndef CMMC_LED_H
#define CMMC_LED_H
#include <Arduino.h>
#include <Ticker.h>

typedef enum
{
  BLINK_TYPE_TICKER = 1,
  BLINK_TYPE_INTERVAL,
} blink_t;


static const blink_t TYPE_TICKER = BLINK_TYPE_TICKER;
static const blink_t TYPE_INTERVAL = BLINK_TYPE_INTERVAL;


class CMMC_LED
{
public:
  CMMC_LED(blink_t type = BLINK_TYPE_TICKER);
  CMMC_LED(Ticker *ticker);
  CMMC_LED init(blink_t type = BLINK_TYPE_TICKER);

  void setPin(uint8_t pin);
  void toggle();
  void blink(uint32_t ms, uint8_t pin);
  void detach();
  void low();
  void high();
  void blink(uint32_t ms);

private:
  unsigned int _ledPin = 254;
  Ticker *_ticker;
  Ticker *_ticker2;
  blink_t _type;
  uint8_t state = LOW;
  uint32_t prev_active;
  bool _initialized = false;
};

#endif