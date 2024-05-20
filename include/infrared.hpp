#include <IRremote.hpp>
#include <pinagem.hpp>

// Define os códigos NEC específicos a serem verificados
#define NEC_CODE_FASTFORWARD 0x40
#define NEC_CODE_REWIND 0x44

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
            }
        }
        IrReceiver.resume(); // Enable receiving of the next value
        return decodedCmd;
    }
}