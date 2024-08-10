
#include <Arduino.h>
#include <pinagem.hpp>
#include <stepMotor.hpp>
#include <infrared.hpp>
#include <lightSensor.hpp>
#include <timeSync.hpp>

// Executa a leitura do sensor em uma task separada
void taskLeituraSensor(void *args) {
    const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
    for(;;) {
        lightSensor::readSensor();
        Serial.println("Sensor value: " + String(lightSensor::sensorValue));
        vTaskDelay(xDelay);
        // Serial.println("Sensor, Core: " + String(xPortGetCoreID()));
    }
}

void taskVerificarTempo(void *args) {
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

    // Sincroniza o relógio
    timeSync::syncTime();
    for(;;) {
        // Sincroniza o horário do ESP32 com o servidor NTP meio-dia
        if (timeSync::isTime(12, 0)) {
            timeSync::syncTime();
        }
        // Verifica se o horário atual é igual ao horários passados como parâmetro
        if (timeSync::isTime(6, 0)) {
            stepMotor::updateTarget(stepMotor::blindPercentage(50)); // Rotate clockwise
        }
         if (timeSync::isTime(18, 0)) {
            stepMotor::updateTarget(stepMotor::blindPercentage(0)); // Rotate clockwise
        }
        // Printa o horário local do ESP32
        timeSync::printLocalTime();
        vTaskDelay(xDelay);
    }
}

void setup() {
    // Inicializa a comunicação serial
    Serial.begin(9600);

    // Inicializa o driver do motor de passo
    stepMotor::initDriver();

    // Inicializa o receptor de infravermelho
    infrared::initIR();

    // Inicializa o sensor de luminosidade
    lightSensor::initSensor();

    // Cria as tasks
    // xTaskCreate(taskLeituraSensor, "Leitura Sensor", 4096, NULL, 4, NULL);
    xTaskCreate(taskVerificarTempo, "Verificar Tempo", 4096, NULL, 4, NULL);
}

void loop() {
    
    // Decodifica o sinal infravermelho
    uint32_t decodedIR = infrared::decodeIR();

    // Se algum sinal tiver sido recebido
    if (decodedIR != 0x00) {
        // Verifica se o código infravermelho recebido é igual ao código passado como parâmetro
        if (decodedIR == NEC_CODE_FASTFORWARD) {
            stepMotor::incrementStep(120, true); // Rotate clockwise
        } else if (decodedIR == NEC_CODE_REWIND) {
            stepMotor::incrementStep(-120, true); // Rotate counterclockwise
        } else if (decodedIR == NEC_CODE_RESET_STEP) {
            stepMotor::resetStep(); // Reseta o target e o current step
        } else if (decodedIR == NEC_CODE_0) {
            stepMotor::updateTarget(stepMotor::blindPercentage(0));
        } else if (decodedIR == NEC_CODE_1) {
            stepMotor::updateTarget(stepMotor::blindPercentage(100/9));
        } else if (decodedIR == NEC_CODE_2) {
            stepMotor::updateTarget(stepMotor::blindPercentage(200/9));
        } else if (decodedIR == NEC_CODE_3) {
            stepMotor::updateTarget(stepMotor::blindPercentage(300/9));
        } else if (decodedIR == NEC_CODE_4) {
            stepMotor::updateTarget(stepMotor::blindPercentage(400/9));
        } else if (decodedIR == NEC_CODE_5) {
            stepMotor::updateTarget(stepMotor::blindPercentage(500/9));
        } else if (decodedIR == NEC_CODE_6) {
            stepMotor::updateTarget(stepMotor::blindPercentage(600/9));
        } else if (decodedIR == NEC_CODE_7) {
            stepMotor::updateTarget(stepMotor::blindPercentage(700/9));
        } else if (decodedIR == NEC_CODE_8) {
            stepMotor::updateTarget(stepMotor::blindPercentage(800/9));
        } else if (decodedIR == NEC_CODE_9) {
            stepMotor::updateTarget(stepMotor::blindPercentage(100));
        }
    }
    
    // Executa um passo do motor de passo
    stepMotor::executeStep();

    // Serial.println(stepMotor::currentStep);
}