#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "backendapi.h"

#define BAUD_RATE 115200


// WiFi Configuration
const char *WIFI_SSID = "WIFI-SSID";
const char *WIFI_PASSWORD = "PASSWORD-1234";


// Pin Configuration
const int ID_SW_1 = 5;
const int ID_SW_2 = 14;
const int ID_SW_4 = 4;
const int ID_SW_8 = 12;
const int PUMP = 13;


//API Related Variables
BackendApi api;
const char *HOST = "192.168.178.91";
const unsigned short PORT = 8000;
const char *AUTH_TOKEN = "abc1234";
const char *URL_PREFIX = "";
//const char* CERT_FINGERPRINT = "DE:AD:BE:EF";

unsigned short getBoardID() {
    unsigned short id = 0;
    if (digitalRead(ID_SW_1) == LOW)
        id += 1;
    if (digitalRead(ID_SW_2) == LOW)
        id += 2;
    if (digitalRead(ID_SW_4) == LOW)
        id += 4;
    if (digitalRead(ID_SW_8) == LOW)
        id += 8;

    Serial.print("Board ID: ");
    Serial.println(id);
    return id;
}


bool initializeWiFi() {
    Serial.print("[*] Try to connect to WiFi with SSID ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        if (WiFi.status() == WL_CONNECT_FAILED) {
            Serial.println("[!] Incorrect SSID or password");
            return false;
        }
    }
    Serial.println("");
    Serial.println("[+] WiFi connected");
    Serial.print("[*] IP address: ");
    Serial.println(WiFi.localIP());

    return true;
}


void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("[+] Start ESP firmware initialization\n");

    pinMode(ID_SW_1, INPUT);
    pinMode(ID_SW_2, INPUT);
    pinMode(ID_SW_4, INPUT);
    pinMode(ID_SW_8, INPUT);
    pinMode(PUMP, OUTPUT);
    digitalWrite(PUMP, 0);

    api.setHost(HOST);
    api.setPort(PORT);
    api.setUrlPrefix(URL_PREFIX);
    api.setToken(AUTH_TOKEN);
    api.setCertificateFingerPrint("aaa");

    Serial.println("[+]Finished ESP firmware initialization\n");
}


void loop() {
    unsigned short id = getBoardID();
    unsigned long sleepPeriod = 1;

    if (initializeWiFi()) {
        struct timings *t = api.fetchTimings(id);
        if (t) {
            sleepPeriod = t->sleep;
            if (t->active > 0) {
                Serial.print("Activate pump for ");
                Serial.print(t->active);
                Serial.println(" s\n");
                digitalWrite(PUMP, 1);
                delay(t->active * 1000);
                digitalWrite(PUMP, 0);
            }
        } else {
            Serial.println("[!] Failed to get pump timings\n");
        }
    }
    if (sleepPeriod > 0) {
        Serial.print("Sleep for ");
        Serial.print(sleepPeriod);
        Serial.println(" s\n");
        ESP.deepSleep(sleepPeriod * 1000000, WAKE_RF_DEFAULT);
    }
}

