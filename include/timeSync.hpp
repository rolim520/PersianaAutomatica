#include <WiFi.h>
#include <time.h>
#include <pinagem.hpp>
#include <ledSignal.hpp>
#include <networkInfo.hpp>

namespace timeSync {
    const char* ntpServer = "pool.ntp.org";
    const long  gmtOffset_sec = -10800; // GMT -3:00
    const int   daylightOffset_sec = 0; // No daylight saving time

    int previousTimeReached = -1; // Guarda a ultima verificação de horário para evitar repetições

    // Converte as horas minutos e segundos em um unico inteiro representando os segundos desde 0h
    int toDaySeconds(int hour, int minute, int second = 0) {
        return hour*3600 + minute*60 + second;
    }

    // Imprime o horário local do ESP32
    void printLocalTime(){
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            return;
        }
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    }

    // Conecta na rede wifi
    void connectWiFi() {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED) {
            LedSignal::blinkLED(1, 200);
            Serial.print(".");
        }

        Serial.println("");
        Serial.println("WiFi connected.");
        LedSignal::blinkLED(1, 1000);
    }

    // Sincroniza o horário do ESP32 com o servidor NTP
    void syncTime() {
        connectWiFi();
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

        struct tm timeinfo;
        while(!getLocalTime(&timeinfo)) {
            LedSignal::blinkLED(1, 200);
            Serial.println("Failed to obtain time");
        }
        Serial.println("Time synchronized:");
        LedSignal::blinkLED(1, 1000);
        printLocalTime();

        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);

        previousTimeReached = toDaySeconds(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    }

    // Verifica se o horário atual é igual ao horário passado como parâmetro
    bool isTime(int hour, int minute, int second = 0) {

        struct tm timeinfo;
        getLocalTime(&timeinfo);

        // Converte o horário do esp e o recebido em segundos
        int localSeconds = toDaySeconds(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        int receivedSeconds = toDaySeconds(hour, minute, second);

        // Reseta o marcador de verificação às 0h
        if (previousTimeReached > localSeconds) {
            previousTimeReached = localSeconds;
        }
        
        // Se tiver ultrapassado o horario e se essa verificação já não tiver sido feita.
        if ((localSeconds >= receivedSeconds) and (receivedSeconds > previousTimeReached)){
            previousTimeReached = receivedSeconds;
            return true;
        }
        return false;
    }
}