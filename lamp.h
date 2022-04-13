#ifndef LAMP_HPP_INCLUDED
#define LAMP_HPP_INCLUDED
#include <FastLED.h>
#include <stdint.h>

const CRGB USE_COLORS[] = {
    CRGB::Red,
    CRGB::Green,
    CRGB::Yellow,
    CRGB::Pink,
    CRGB::Turquoise
};

#define USE_COLORS_LEN sizeof(USE_COLORS) / sizeof((USE_COLORS)[0])

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
        void set_color(CRGB);
        void poweroff();
        void poweron();
        void toggle_power();
        void cycle_brightness();
        void cycle_colors();

};

template <int num_leds, int pin>
void Lamp<num_leds, pin>::redraw()
{
    if (!power)
        return;

    Serial.print("Redraw: ");
    Serial.println(current_color);
    FastLED.setBrightness(brightness);
    for (uint8_t i = 0; i < num_leds; ++i) {
        leds[i] = USE_COLORS[current_color];
        raw_leds[i] = leds[i];
        Serial.print(raw_leds[i].r);
        Serial.print(" ");
        Serial.print(raw_leds[i].g);
        Serial.print(" ");
        Serial.println(raw_leds[i].b);
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
void Lamp<num_leds, pin>::set_color(CRGB color)
{
    fill_solid(leds, num_leds, color);
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
    redraw();
    power = true;
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
    ++current_color;
    current_color %= USE_COLORS_LEN;
    redraw();
}

#endif
