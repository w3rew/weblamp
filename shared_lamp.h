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
    uint64_t key;
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
        bool connected;
        uint64_t key;


    public:
        void tick();
        SharedLamp() {};
        SharedLamp(const char* wifi_ssid,
                const char* wifi_password,
                host_t host,
                uint16_t port,
                uint64_t key);

        void dump(LampState* state);
        bool load(const LampState* state);
        bool send_state(const LampState* state);
        bool receive_state(LampState* state);
};

template <int num_leds, int pin>
SharedLamp<num_leds, pin>::SharedLamp(const char* wifi_ssid,
        const char* wifi_password,
        host_t host,
        uint16_t port,
        uint64_t key)
    : host(host),
    port(port),
    previous_tick(0),
    connected(false),
    key(key),
    Lamp<num_leds, pin>()
{
    this->set_color(CRGB::Red);
    this->poweron(); //First color is red
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
}


template <int num_leds, int pin>
void SharedLamp<num_leds, pin>::dump(LampState* state)
{
    Serial.print("uint64_t size: ");
    Serial.println(sizeof(this->key));

    state->key = this->key;
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

    Serial.println("Trying to send state");
    if (!this->server.connected()) {
        Serial.println("Not connected, will connect and retry again");
        this->server.connect(host.arr, port);
        return false;
    }


    char* key_data = (char*)&state->key;
    for (int i = 0; i < 8; ++i)
        this->server.write(key_data[i]);
    this->server.write(state->power);
    this->server.write(state->color);


    Serial.println("Sent state");
    return true;
}

template <int num_leds, int pin>
bool SharedLamp<num_leds, pin>::receive_state(LampState* state)
{
    transfer_start = millis();

    Serial.println("Trying to receive state");
    wait(this->server.available()); //TODO: maybe rewrite this
    state->power = this->server.read();

    wait(this->server.available());
    state->color = this->server.read();

    Serial.println("Received state");
    return true;

}


template <int num_leds, int pin>
void SharedLamp<num_leds, pin>::tick()
{
    if (millis() - previous_tick < TIMEOUT_TICK)
        return;

    previous_tick = millis();
    Serial.println("Tick!");
    Serial.print("Wifi status: ");
    Serial.println(WiFi.status());


    if (WiFi.status() == WL_CONNECTED) {
        if (!connected) {
            this->set_color(CRGB::Green);
            connected = true;
        }
    } else
        return;
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
    return;
}
#endif
