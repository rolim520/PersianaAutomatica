#include <Arduino.h>
#include <pinagem.hpp>

namespace stepMotor {

    int currentStep = 0;
    int targetStep = 0;

    void initDriver() {
        // Define os pinos como saída ou entrada
        pinMode(STEP_PIN, OUTPUT);
        pinMode(DIR_PIN, OUTPUT);
        pinMode(ENABLE_PIN, OUTPUT);
        pinMode(LED_PIN, OUTPUT);

        // Desativa o driver
        digitalWrite(ENABLE_PIN, HIGH);
    }

    // Acança um passo em direção ao target step
    void executeStep() {
        if (currentStep < targetStep) {
            // Define a direção do motor
            digitalWrite(DIR_PIN, HIGH);
            // Atualiza o passo atual
            currentStep++;
        } else if (currentStep > targetStep) {
            // Define a direção do motor
            digitalWrite(DIR_PIN, LOW);
            // Atualiza o passo atual
            currentStep--;
        }

        if (currentStep != targetStep) {
            // Ativa o driver
            digitalWrite(ENABLE_PIN, LOW);

            // Gira o motor 1 passo
            digitalWrite(STEP_PIN, HIGH);
            delayMicroseconds(800);
            digitalWrite(STEP_PIN, LOW);
            delayMicroseconds(800);

            // Desativa o driver
            digitalWrite(ENABLE_PIN, HIGH);
        }
    }

    // Define o valor do target step
    void updateTarget(int step) {
        targetStep = step;
    }

    // Incrementa o valor do targetstep
    void incrementStep(int steps, bool isLimited) {
        int limit = 70;
        // Se mudar a direção do motor com targets acumulados ele reseta o target
        if ((targetStep-currentStep)*steps >= 0) {
            targetStep = targetStep + steps;
        } else {
            targetStep = currentStep + steps;
        }

        // Limita o valor maximo de incremento de limite for verdadeiro
        if (isLimited) {
            targetStep = max(currentStep-limit, min(targetStep, currentStep+limit));
        }
    }

    // Reseta o valor do step
    void resetStep() {
        currentStep = 0;
        targetStep = 0;
    }
}