#ifndef SHARED_LAMP_HPP_INCLUDED
#define SHARED_LAMP_HPP_INCLUDED
#include <stdint.h>
#include <ESP8266WiFi.h>
#include "lamp.h"

template <int num_leds, int pin>
class SharedLamp : public Lamp<num_leds, pin>
{
    public:
        void tick() {};

};
#endif
