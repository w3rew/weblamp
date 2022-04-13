#ifndef SHARED_LAMP_HPP_INCLUDED
#define SHARED_LAMP_HPP_INCLUDED
#include <stdint.h>
#include <ESP8266WiFi.h>
#include "lamp.h"
#define TIMEOUT_TICK 3000 //millis
#define TIMEOUT_INET 3000 //millis
#define wait(x) while(!(x)) {if (millis() - transfer_start > TIMEOUT_INET) return false;}

struct LampState
{
    uint8_t power; //bool actually
    uint8_t color;
};

union host_t
{
    uint32_t num;
    uint8_t arr[4];
};

template <int num_leds, int pin>
class SharedLamp : public Lamp<num_leds, pin>
{
    private:
        WiFiClient server;
        unsigned long transfer_start;
        unsigned long previous_tick;
        host_t host;
        uint16_t port;


    public:
        void tick();
        SharedLamp() {};
        SharedLamp(const char* wifi_ssid,
                const char* password,
                host_t host,
                uint16_t port);

        void dump(LampState* state);
        bool load(const LampState* state);
        bool send_state(const LampState* state);
        bool receive_state(LampState* state);
};

template <int num_leds, int pin>
SharedLamp<num_leds, pin>::SharedLamp(const char* wifi_ssid, const char* password, host_t host, uint16_t port)
    : host(host), port(port), previous_tick(0), Lamp<num_leds, pin>()
{
    this->poweron(); //First color is red
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
#ifdef DEBUG
        Serial.println("Connecting");
#endif
        delay(1000);
    }
    Serial.println("Connected!");
    this->cycle_colors(); //Second color is green
}


template <int num_leds, int pin>
void SharedLamp<num_leds, pin>::dump(LampState* state)
{
    state->power = this->power;
    state->color = this->current_color;
}

template <int num_leds, int pin>
bool SharedLamp<num_leds, pin>::load(const LampState* state)
{
    if (state->color < USE_COLORS_LEN) {
        Serial.print("Load: ");
        Serial.print(state->power);
        Serial.print(" ");
        Serial.println(state->color);
        this->power = state->power;
        this->current_color = state->color;
        this->redraw();
        return true;
    }
    return false;
}

template <int num_leds, int pin>
bool SharedLamp<num_leds, pin>::send_state(const LampState* state)
{
    transfer_start = millis();
    this->server.flush();

    Serial.println("Send state");
    wait(this->server.connect(host.arr, port));
    //wait(this->server.connect({100, 111, 251, 167}, 1348));

    this->server.write(state->power);
    this->server.write(state->color);


    Serial.println("Sent state");
    return true;
}

template <int num_leds, int pin>
bool SharedLamp<num_leds, pin>::receive_state(LampState* state)
{
    transfer_start = millis();

    wait(this->server.available());
    state->power = this->server.read();

    wait(this->server.available());
    state->color = this->server.read();

    Serial.println("Receive state");
    return true;

}


template <int num_leds, int pin>
void SharedLamp<num_leds, pin>::tick()
{
    return;
    if (millis() - previous_tick < TIMEOUT_TICK)
        return;

    Serial.println("Tick!");
    LampState state;

    this->dump(&state);

    if (!this->send_state(&state)) {
        Serial.println("Failed to send state!");
        goto FIN_CONN;
    }

    if (!this->receive_state(&state)) {
        Serial.println("Failed to receive state!");
        goto FIN_CONN;
    }

    this->load(&state);
FIN_CONN:
    this->server.stop();

    previous_tick = millis();
}
#endif
