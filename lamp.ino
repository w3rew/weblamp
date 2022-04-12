#include <Arduino.h>
#include "shared_lamp.h"
#include "event_handler.h"
#include <OneButton.h>

#define BUTTON_PIN D1
#define LED_PIN D5
#define LED_NUM 20

host_t host = {.arr = {94, 45, 205, 220}};
uint16_t port = 1348;

SharedLamp<LED_NUM, LED_PIN> lamp("nova29", "tyghfedrdx", host, port);
OneButton btn(BUTTON_PIN, false, false);
EventHandler handler(&lamp, &btn);
void setup()
{
    Serial.begin(9600);
}

void loop()
{
    handler.tick();
}
