#include <IRremote.hpp>
#include <pinagem.hpp>

// Define os códigos NEC específicos a serem verificados
#define NEC_CODE_FASTFORWARD 0x40
#define NEC_CODE_REWIND 0x44
#define NEC_CODE_RESET_STEP 0x43
#define NEC_CODE_FULLBACK 0xD
#define NEC_CODE_0 0x16
#define NEC_CODE_1 0xC
#define NEC_CODE_2 0x18
#define NEC_CODE_3 0x5E
#define NEC_CODE_4 0x8
#define NEC_CODE_5 0x1C
#define NEC_CODE_6 0x5A
#define NEC_CODE_7 0x42
#define NEC_CODE_8 0x52
#define NEC_CODE_9 0x4A



namespace infrared {

    // Inicializa o receptor de infravermelho
    void initIR() {
        IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); // Start the receiver
    }

    // Função que decodifica o sinal infravermelho
    uint32_t decodeIR() {
        uint32_t decodedCmd = 0x00;

        if (IrReceiver.decode()) {
            // Somente guarda o sinal se for do protocolo NEC ou não tiver erros de paridade
            if ((IrReceiver.decodedIRData.protocol == NEC) and (IrReceiver.decodedIRData.flags != IRDATA_FLAGS_PARITY_FAILED)){
                decodedCmd = IrReceiver.decodedIRData.command;
                Serial.println(decodedCmd, HEX);
            }
        }
        IrReceiver.resume(); // Enable receiving of the next value
        return decodedCmd;
    }
}