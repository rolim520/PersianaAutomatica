#include <Arduino.h>
#include <pinagem.hpp>

namespace LedSignal {
    void blinkLED(int times, int interval) {
        for (int i = 0; i < times; i++) {
            digitalWrite(LED_PIN, HIGH);
            delay(interval);
            digitalWrite(LED_PIN, LOW);
            delay(interval);
        }
    }
}