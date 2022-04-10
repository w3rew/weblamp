#ifndef LAMP_HPP_INCLUDED
#define LAMP_HPP_INCLUDED
#include <FastLED.h>
#include <stdint.h>
template <int num_leds, int pin>
class Lamp {
    private:
        CRGB leds[num_leds];
        CRGB raw_leds[num_leds];
        void redraw();
        bool power;
        uint8_t brightness;

    public:
        Lamp();

        void set_brightness(uint8_t);
        void set_color();
        void poweroff();
        void poweron();
        void toggle_power();
        void cycle_brightness();

};

template <int num_leds, int pin>
Lamp<num_leds, pin>::redraw()
{
    FastLED.setBrightness(brightness);
    for (uint8_t i = 0; i < num_leds; ++i) {
        raw_leds[i] = leds[i];
    }
    FastLED.show();
}

template <int num_leds, int pin>
Lamp<num_leds, pin>::Lamp()
    : power(false), brightness(0)
{
    FastLED.addLeds<WS2812B, pin, GRB>(leds, num_leds);
}

template <int num_leds, int pin>
Lamp<num_leds, pin>::set_brightness(uint8_t brightness)
{
    this->brightness = brithness;
    redraw();
}

template <int num_leds, int pin>
Lamp<num_leds, pin>::set_color(CRGB color)
{
    fill_solid(leds, num_leds, color);
    this->redraw();
}

template <int num_leds, int pin>
Lamp<num_leds, pin>::poweroff()
{
    FastLED.clear();
    FastLED.show();
    power = false;
}

template <int num_leds, int pin>
Lamp<num_leds, pin>::poweron()
{
    redraw();
    power = true;
}

template <int num_leds, int pin>
Lamp<num_leds, pin>::toggle_power()
{
    if (power)
        this->poweroff();
    else
        this->poweroff();

    power = !power;
}
        
template <int num_leds, int pin>
Lamp<num_leds, pin>::cycle_brightness()
{
    ++this->brightness; //it is unsigned
    redraw();
}

#endif
