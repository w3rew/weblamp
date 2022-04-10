#ifndef EVENT_HANDLER_HP_INCLUDED
#define EVENT_HANDLER_HP_INCLUDED

#include "shared_lamp.hpp"
#include <OneButton.h>

#ifndef LONG_PRESS_COOLDOWN
#define LONG_PRESS_COOLDOWN 1000 //in ms
#endif

template <int num_leds, int led_pin>
class EventHandler
{
    private:
        SharedLamp<num_leds, pin>* lamp;
        OneButton* btn;
        int long_press_prev_ms;

    public:
        void key_holds();
        void key_clicked();
        void key_double_click();
        void key_long_press_start();

        EventHandler(SharedLamp<num_leds, pin>* lamp, OneButton* btn);
        void tick();
};


template <int num_leds, int led_pin>
EventHandler<num_leds, led_pin>::EventHandler(SharedLamp* lamp, OneButton* btn)
    : long_press_prev_ms(0)
{
    this->lamp = lamp;
    this->btn = btn;
    this->btn.attachClick(this->key_clicked);
    this->btn.attachDoubleClick(this->key_double_clicked);
    this->btn.attachDuringLongPress(this->key_holds);
}

template <int num_leds, int led_pin>
EventHandler<num_leds, led_pin>::tick()
{
    btn->tick();
    lamp->tick();
}

template <int num_leds, int led_pin>
EventHandler<num_leds, led_pin>::key_clicked()
{
    this->lamp.cycle_colors();
}

template <int num_leds, int led_pin>
EventHandler<num_leds, led_pin>::key_holds()
{
    int ms = btn->getPressedTicks();
    if (ms - this->long_press_prev_ms > LONG_PRESS_COOLDOWN) {
        this->long_press_prev_ms = ms;
        this->lamp.cycle_brightness();
    }
}

template <int num_leds, int led_pin>
EventHandler<num_leds, led_pin>::key_long_press_start()
{
    this->long_press_prev_ms = 0;
}

template <int num_leds, int led_pin>
EventHandler<num_leds, led_pin>::key_double_click()
{
    this->lamp.toggle_power();
}
#endif
