//
// Created by henry on 03.06.18.
//

#include <cstdio>
#include <cstring>
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "backendapi.h"
#include <ArduinoJson.h>


void BackendApi::setPort(const unsigned short port) {
    this->port = port;
}

void BackendApi::setHost(const char *host) {
    this->host = host;
}

struct timings *BackendApi::fetchTimings(unsigned short id) {
    String responseData = this->sendRequest(id);
    if (responseData.length() < 1)
        return nullptr;

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &data = jsonBuffer.parseObject(responseData);
    if (!data.success()) {
        Serial.println("[!] Parsing failed");
        return nullptr;
    }

    static struct timings *receivedTimings = nullptr;
    if (receivedTimings == nullptr) {
        receivedTimings = (struct timings *) (malloc(sizeof(struct timings)));
    } else {
        memset(receivedTimings, 0, sizeof(struct timings));
    }

    receivedTimings->active = data["active"].as<unsigned long>();
    receivedTimings->sleep = data["sleep"].as<unsigned long>();
    return receivedTimings;
}

String BackendApi::sendRequest(unsigned short id) {
    char url[250];
    memset(url, 0, sizeof(url));
    sprintf(url, "%s/api/pumps/%d/timings", this->urlPrefix, id);

    //decide if we use HTTP or HTTPS for the data exchange
    HTTPClient http;
    if(this->certFingerPrint != nullptr)
        http.begin(this->host, this->port, url, this->certFingerPrint);
    else
        http.begin(this->host, this->port, url);

    char token [200];
    sprintf(token,"TOKEN %s",this->authToken);
    http.addHeader("Authorization",token);
    http.addHeader("Content-Type","application/json");

    int responseCode = http.GET();
    if (responseCode != HTTP_CODE_OK) {
        Serial.print("[!] Received response with status code ");
        Serial.println(responseCode);
        return "";
    }

    return http.getString();
}

void BackendApi::setUrlPrefix(const char *prefix) {
    this->urlPrefix = prefix;
}

void BackendApi::setToken(const char *token) {
    this->authToken = token;
}

void BackendApi::setCertificateFingerPrint(const char *fp) {
    this->certFingerPrint = fp;
}
