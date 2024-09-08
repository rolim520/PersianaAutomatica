#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#define ERROR_VALUE -99999

namespace persistenciaDeDados {

    void initMemoria() {
        // Initialize LittleFS
        if (!LittleFS.begin(true)) {
            Serial.println("Failed to mount file system");
            return;
        }

        // Check if the file 'data.json' exists
        if (!LittleFS.exists("/data.json")) {
            Serial.println("File 'data.json' does not exist, creating it...");

            // Create and open the file for writing
            File file = LittleFS.open("/data.json", FILE_WRITE);
            if (!file) {
                Serial.println("Failed to create file 'data.json'");
                return;
            }

            // Write initial content of the file
            file.println("{\"steps\":0}");
            file.close();
            Serial.println("File 'data.json' created successfully");
        } else {
            Serial.println("File 'data.json' already exists");

            // Open the file for reading
            File file = LittleFS.open("/data.json", FILE_READ);
            if (!file) {
                Serial.println("Failed to open 'data.json' for reading");
                return;
            }

            // Print the content of the file
            while (file.available()) {
                Serial.write(file.read());
            }
            Serial.println();
            file.close();
        }
    }


    void storeSteps(int value) {
        // Open the file for reading
        File file = LittleFS.open("/data.json", FILE_READ);
        if (!file) {
            Serial.println("Failed to open 'data.json' for reading");
            return;
        }

        // Allocate a buffer for the JSON data
        JsonDocument doc;
        
        // Parse the JSON file
        DeserializationError error = deserializeJson(doc, file);
        file.close();  // Close the file after reading
        
        if (error) {
            Serial.println("Failed to parse JSON file");
            return;
        }

        // Update the key-value pair with an integer value
        doc["steps"] = value;

        // Open the file for writing
        file = LittleFS.open("/data.json", FILE_WRITE);
        if (!file) {
            Serial.println("Failed to open 'data.json' for writing");
            return;
        }

        // Write the updated JSON object to the file
        serializeJson(doc, file);
        file.close();

        Serial.println("Successfully wrote key-value to 'data.json'");
    }

    int readSteps() {
        // Open the file for reading
        File file = LittleFS.open("/data.json", FILE_READ);
        if (!file) {
            Serial.println("Failed to open 'data.json' for reading");
            return ERROR_VALUE;
        }

        // Allocate a buffer for the JSON data
        JsonDocument doc;

        // Parse the JSON file
        DeserializationError error = deserializeJson(doc, file);
        file.close();  // Close the file after reading

        if (error) {
            Serial.println("Failed to parse JSON file");
            return ERROR_VALUE;
        }

        // Retrieve the value associated with the key
        if (doc.containsKey("steps")) {
            return doc["steps"].as<int>();
        } else {
            Serial.println("Key not found in JSON file");
            return ERROR_VALUE;
        }
    }
}
