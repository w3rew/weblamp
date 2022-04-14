#include <Arduino.h>
#include "shared_lamp.h"
#include "event_handler.h"
#include <OneButton.h>

#define BUTTON_PIN D1
#define LED_PIN D5
#define LED_NUM 20
#define KEY 0x1337fda380dff23bULL

host_t host = {.arr = {192, 168, 5, 241}};
uint16_t port = 1348;

SharedLamp<LED_NUM, LED_PIN> lamp("nova29", "tyghfedrdx", host, port, KEY);
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
