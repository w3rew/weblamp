#include <Arduino.h>
#include "shared_lamp.h"
#include "event_handler.h"
#include <OneButton.h>

#define BUTTON_PIN D1
#define LED_PIN D5
#define LED_NUM 20

host_t host = {.arr = {100, 111, 251, 167}};
uint16_t port = 1348;

SharedLamp<LED_NUM, LED_PIN> lamp;
OneButton btn(BUTTON_PIN, false, false);
EventHandler handler(&lamp, &btn);
void setup()
{
    Serial.begin(9600);
    //lamp = SharedLamp<LED_NUM, LED_PIN>("MGTS_GPON_2686", "9166757418%9150115290", host, port);
}

void loop()
{
    handler.tick();
}
