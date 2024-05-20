#include <Arduino.h>
#include <pinagem.hpp>

namespace lightSensor {

    // Variavel para guardar a leitura do sensor
    int sensorValue = 0;

    // Inicializa o sensor de luminosidade
    void initSensor() {
        pinMode(LIGHT_SENSOR_PIN, INPUT);
    }

    // Realiza a leitura do sensor de luminosidade
    void readSensor() {
        sensorValue = analogRead(LIGHT_SENSOR_PIN);
    }
}
