#ifndef LAMP_HPP_INCLUDED
#define LAMP_HPP_INCLUDED

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <stdint.h>

#define SATURATION 255


template <int num_leds, int pin>
class Lamp {
    protected:
        CRGB leds[num_leds];
        CRGB raw_leds[num_leds];
        void redraw();
        bool power;
        uint8_t brightness;
        uint8_t current_color;

    public:
        Lamp();

        void set_brightness(uint8_t);
        void set_color(uint8_t);
        void poweroff();
        void poweron();
        void toggle_power();
        void cycle_brightness();
        void cycle_colors();

};

template <int num_leds, int pin>
void Lamp<num_leds, pin>::redraw()
{
    if (!power) {
        this->poweroff();
        return;
    }

#ifdef DEBUG
    Serial.print("Redraw: ");
    Serial.println(current_color);
#endif
    for (uint8_t i = 0; i < num_leds; ++i) {
        raw_leds[i] = CHSV(current_color, SATURATION, brightness);
#ifdef DEBUG
        Serial.print(raw_leds[i].r);
        Serial.print(" ");
        Serial.print(raw_leds[i].g);
        Serial.print(" ");
        Serial.println(raw_leds[i].b);
#endif
    }
    FastLED.show();
}

template <int num_leds, int pin>
Lamp<num_leds, pin>::Lamp()
    : power(false), brightness(255), current_color(0)
{
    FastLED.addLeds<WS2812B, pin, GRB>(raw_leds, num_leds);
}

template <int num_leds, int pin>
void Lamp<num_leds, pin>::set_brightness(uint8_t brightness)
{
    brightness = brightness;
    redraw();
}

template <int num_leds, int pin>
void Lamp<num_leds, pin>::set_color(uint8_t color)
{
    current_color = color;
    redraw();
}

template <int num_leds, int pin>
void Lamp<num_leds, pin>::poweroff()
{
    FastLED.clear();
    FastLED.show();
    power = false;
}

template <int num_leds, int pin>
void Lamp<num_leds, pin>::poweron()
{
    power = true;
    redraw();
}

template <int num_leds, int pin>
void Lamp<num_leds, pin>::toggle_power()
{
    if (power)
        poweroff();
    else
        poweron();

}
        
template <int num_leds, int pin>
void Lamp<num_leds, pin>::cycle_brightness()
{
    ++brightness; //it is unsigned
    redraw();
}

template <int num_leds, int pin>
void Lamp<num_leds, pin>::cycle_colors()
{
#ifdef DEBUG
    Serial.println("Cycle colors!");
#endif
    current_color = random8();
    redraw();
}

#endif
