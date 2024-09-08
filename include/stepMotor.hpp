#include <Arduino.h>
#include <pinagem.hpp>
#include <persistencia.hpp>
#include <FastAccelStepper.h>

namespace stepMotor {

    // Número de steps necessários para virar completamente a persiana
    int fullTurnSteps = -26600;

    FastAccelStepperEngine engine = FastAccelStepperEngine();
    FastAccelStepper *stepper = NULL;

    void initDriver() {
        engine.init();
        stepper = engine.stepperConnectToPin(STEP_PIN);

        if (stepper) {
            stepper->setDirectionPin(DIR_PIN);
            stepper->setEnablePin(ENABLE_PIN);
            stepper->setAutoEnable(true);

            // If auto enable/disable need delays, just add (one or both):
            // stepper->setDelayToEnable(50);
            // stepper->setDelayToDisable(1000);

            stepper->setSpeedInUs(750);  // the parameter is us/step !!!
            stepper->setAcceleration(12800);
        }
    }

    // Move para um numero de steps absoluto
    void moveToPosition(int step){
        // Move the stepper motor to the target step
        stepper->moveTo(step);
    }

    // Incrementa ou dcrementa o numero target de steps para o numero informado
    void moveSteps(int steps) {
        stepper->moveTo(stepper->getCurrentPosition() + steps);
    }

    // Reseta o valor do step
    void resetStep() {
        stepper->forceStop();
        while (stepper->isRunning()) {
            // Pass
        }
        stepper->setCurrentPosition(0);
    }

    // Define o valor da current position de steps
    void setPosicaoSteps(int position) {
        stepper->setCurrentPosition(position);
    }

    // Obtém o valor da current position de steps
    int getPosicaoSteps() {
        return stepper->getCurrentPosition();
    }

    // Returna o número de steps equivalente a porcentagem do fullTurnSteps
    int blindPercentage(float percentage) {
        return fullTurnSteps*percentage/100;
    }
}