#ifndef EVENT_HANDLER_HP_INCLUDED
#define EVENT_HANDLER_HP_INCLUDED

#include "shared_lamp.h"
#include <Arduino.h>
#include <OneButton.h>

#ifndef LONG_PRESS_COOLDOWN
#define LONG_PRESS_COOLDOWN 10 //in ms
#endif

template <int num_leds, int led_pin>
class EventHandler
{
    private:
        SharedLamp<num_leds, led_pin>* lamp;
        OneButton* btn;
        unsigned long long_press_prev_ms;

    public:
        void key_holds();
        void key_clicked();
        void key_double_click();
        void key_long_press_start();

        EventHandler(SharedLamp<num_leds, led_pin>* lamp, OneButton* btn);
        void tick();
};


template <int num_leds, int led_pin>
EventHandler<num_leds, led_pin>::EventHandler(SharedLamp<num_leds, led_pin>* lamp, OneButton* btn)
    : long_press_prev_ms(0)
{
    this->lamp = lamp;
    this->btn = btn;
    this->btn->attachClick([this](){this->key_clicked();});
    this->btn->attachDoubleClick([this](){this->key_double_click();});
    this->btn->attachDuringLongPress([this](){this->key_holds();});
    this->btn->attachLongPressStart([this](){this->key_long_press_start();});
}

template <int num_leds, int led_pin>
void EventHandler<num_leds, led_pin>::tick()
{
    btn->tick();
    lamp->tick();
}

template <int num_leds, int led_pin>
void EventHandler<num_leds, led_pin>::key_clicked()
{
    lamp->cycle_colors();
}

template <int num_leds, int led_pin>
void EventHandler<num_leds, led_pin>::key_holds()
{
    unsigned long ms = millis();
    if (ms - long_press_prev_ms > LONG_PRESS_COOLDOWN) {
        Serial.println("Key holds");
        long_press_prev_ms = ms;
        lamp->cycle_brightness();
    }
}

template <int num_leds, int led_pin>
void EventHandler<num_leds, led_pin>::key_long_press_start()
{
    long_press_prev_ms = millis();
}

template <int num_leds, int led_pin>
void EventHandler<num_leds, led_pin>::key_double_click()
{
    lamp->toggle_power();
}

#endif
