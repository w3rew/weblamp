#ifndef SHARED_LAMP_HPP_INCLUDED
#define SHARED_LAMP_HPP_INCLUDED
#include <stdint.h>
#include <ESP8266WiFi.h>
#include "lamp.h"
#define TIMEOUT_TICK 3000 //millis
#define TIMEOUT_INET 3000 //millis
#define wait(x) while(!(x)) {if (millis() - transfer_start > TIMEOUT_INET) return false;}

struct LampState
{
    uint8_t power; //bool actually
    uint8_t color;
};

union host_t
{
    uint32_t num;
    uint8_t arr[4];
};

template <int num_leds, int pin>
class SharedLamp : public Lamp<num_leds, pin>
{
    public:
        void tick() {};
};


#endif
