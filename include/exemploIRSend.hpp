/*
 * CustomSender.cpp
 *
 *  Demonstrates sending custom IR codes using sendPulseDistanceWidth.
 *
 *  This example is adapted to send a specific signal every second.
 *
 *  Copyright (C) 2024  OpenAI
 *
 *  MIT License
 */
#include <Arduino.h>
#include <IRremote.hpp> // include the IRremote library

const int IR_SEND_PIN = 13; // Define the pin where the IR LED is connected

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    while (!Serial)
        ; // Wait for Serial to become available. Is optimized away for some cores.

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);

    /*
     * The IR library setup. That's all!
     */
    IrSender.begin(IR_SEND_PIN); // Start with the defined IR_SEND_PIN as send pin
    disableLEDFeedback(); // Disable feedback LED at default feedback LED pin
}

void loop() {
    // Send the custom IR signal
    Serial.println(F("Sending custom IR signal..."));

    /*
    IrSender.sendPulseDistanceWidth(
      38,              // Frequency in kHz
      8350,            // Lead Mark duration in microseconds
      4100,            // Lead Space duration in microseconds
      600,             // '1' Mark duration in microseconds
      1550,            // '1' Space duration in microseconds
      600,             // '0' Mark duration in microseconds
      450,             // '0' Space duration in microseconds
      0x27D088,        // Raw data
      24,              // Number of bits
      PROTOCOL_IS_LSB_FIRST, // Protocol endianness
      0,               // No repeat
      0                // No number of repeats
    );
    */

   IrSender.sendSamsung(0x7, 0x7, 0);

    Serial.println(F("IR signal sent!"));
    
    // Wait for 1 second
    delay(1000);
}
