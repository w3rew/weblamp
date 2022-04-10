#include <Arduino.h>
#include "shared_lamp.h"
#include "event_handler.h"
#include <OneButton.h>

#define BUTTON_PIN D1
#define LED_PIN D5
#define LED_NUM 20

SharedLamp<LED_NUM, LED_PIN> lamp;
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
