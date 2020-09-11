#include "NexWiFi.h"

extern "C" {
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
}

NexWiFiClass::NexWiFiClass(){
    this->ssid = "nexsolar";
    this->password = "nexsolar";
    //fs::FS& store;

    this->checkConnection = CHECK_INTERNET;
    this->persistence = PERSISTENT;
    this->attempt2Reconnection = FAILED_ATTEMPT;
    this->attempt2Find = 10;
    this->attempt2Connection = 10;
    this->microseconds = 5000000;

    //this->sucess = &(this->sucessDefault);
    //this->failure = &failureDefault;
    //this->notFound = &notFoundDefault;
    //this->failureInternet = &failureInternetDefault;
}

void testA(WiFiEvent_t event, WiFiEventInfo_t info) {
    
}

AsyncWebServer serverDefault(80);

void testB(Print& p) {
    p.println("Eita que deu ruim msmo");
}

void NexWiFiClass::sucessDefault(){}
void NexWiFiClass::failureDefault(){
    this->nexReconnect();
}
void NexWiFiClass::notFoundDefault(){
    this->upServerDefault();
}
void NexWiFiClass::failureInternetDefault() {
    int countReset = 0;
    while (true) {
        if (!this->isConnected())
        {
            this->disconnect(false);
            this->mode(WIFI_STA);
            this->begin(this->ssid.c_str(), this->password.c_str());
            this->nexDelay(); //tempo maximo para att a variavel
            if (this->status() != WL_CONNECTED)
            {
                if (countReset >= this->attempt2Reconnection) {
                    this->upServerDefault();
                }
                countReset = countReset + 1;
            }
        }
        return;
    }
}


void NexWiFiClass::setSsid(std::string ssid) { this->ssid = ssid; }
void NexWiFiClass::setPassword(std::string password) { this->password = password; }
void NexWiFiClass::setCheckConnection(bool checkConnection) { this->checkConnection = checkConnection; }
void NexWiFiClass::setPersistence(bool persistence) { this->persistence = persistence; }
void NexWiFiClass::setAttempt2Find(int attempt2Find) { this->attempt2Find = attempt2Find; }
void NexWiFiClass::setAttempt2Connection(int attempt2Connection) { this->attempt2Connection = attempt2Connection; }
void NexWiFiClass::setMicroseconds(unsigned int microseconds) { this->microseconds = microseconds; }


void NexWiFiClass::printDiag(Print& p)
{
    const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);

    uint8_t primaryChan;
    wifi_second_chan_t secondChan;
    esp_wifi_get_channel(&primaryChan, &secondChan);

    p.println("*----------------------------------------*");

    p.print("Mode: ");
    p.println(modes[mode]);

    p.print("Channel: ");
    p.println(primaryChan);

    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);

    const char* ssid = reinterpret_cast<const char*>(conf.sta.ssid);
    p.print("SSID (");
    p.print(strlen(ssid));
    p.print("): ");
    p.println(ssid);

    const char* passphrase = reinterpret_cast<const char*>(conf.sta.password);
    p.print("Passphrase (");
    p.print(strlen(passphrase));
    p.print("): ");
    p.println(passphrase);

    p.print("BSSID set: ");
    p.println(conf.sta.bssid_set);

    p.println("*----------------------------------------*");
}

void NexWiFiClass::startApDefault() {
    std::string newSsid = PREFIX_AP_SSID;
    String MAC = this->macAddress();
    MAC.replace(":", "-");
    newSsid.append(MAC.c_str());

    IPAddress local_IP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 9);
    IPAddress subnet(255, 255, 255, 0);

    this->softAP(newSsid.c_str(), this->password.c_str());
    this->softAPConfig(local_IP, gateway, subnet);
    this->mode(WIFI_MODE_AP);
}

int NexWiFiClass::checkInternetConnectionDefault() {
    HTTPClient http;
    http.begin(PING_1);

    if (200 == http.GET()) return SUCCESSFUL_INTERNET_CONNECTION;
    else FAILURE_IN_INTERNET_CONNECTION;
}

void NexWiFiClass::checkInternetConnectionCallBack(void (*sucess)()) {
    HTTPClient http;
    http.begin(PING_1);

    if (200 == http.GET()) return sucess();
    else this->failureInternetDefault();
}

void NexWiFiClass::checkInternetConnectionCallBack(void (*sucess)(), void(*failureInternet)()) {
    HTTPClient http;
    http.begin(PING_1);

    if (200 == http.GET()) return sucess();
    else failureInternet();
}

int NexWiFiClass::startStaDefault(fs::FS& store) {
    this->mode(WIFI_STA);
    this->disconnect();
    int numSsid = this->scanNetworks();

    for(int miss = 0; miss < this->attempt2Find; miss++, numSsid = this->scanNetworks()){
       
        for (int thisNet = 0; thisNet < numSsid; thisNet++) {
            if (strcmp(this->ssid.c_str(),(this->SSID(thisNet).c_str())) == 0) {

                for (int failure = 0; failure < this->attempt2Connection; failure++) {
                    if (this->status() == WL_CONNECTED) {

                        if (this->persistence) this->onEvent(testA, SYSTEM_EVENT_STA_DISCONNECTED);

                        File file = store.open(STORE_PATH, FILE_WRITE);
                        if (!file) {
                            //erro ao abrir o arquivo
                        }
                        char result[100];
                        strcpy(result, this->ssid.c_str());
                        strcat(result, this->password.c_str());
                        if (file.print(result)) {
                            //salvo corretamente
                        }
                        else {
                            //erro ao escrever no arquivo
                        }
                        return checkInternetConnectionDefault();
                    }
                    this->begin(this->ssid.c_str(), this->password.c_str());
                    usleep(this->microseconds);
                }
                return CONNECTION_FAILURE;
            }
        }
    }
    if (numSsid >= 0) return SSID_NOT_FOUND;
    else return numSsid;
}

void NexWiFiClass::startStaCallback(fs::FS& store, void (*sucess)()) {
    this->mode(WIFI_STA);
    this->disconnect();
    int numSsid = this->scanNetworks();

    for (int miss = 0; miss < this->attempt2Find; miss++, numSsid = this->scanNetworks()) {

        for (int thisNet = 0; thisNet < numSsid; thisNet++) {
            if (strcmp(this->ssid.c_str(), (this->SSID(thisNet).c_str())) == 0) {

                for (int failure = 0; failure < this->attempt2Connection; failure++) {
                    if (this->status() == WL_CONNECTED) {

                        //if (this->persistence) this->onEvent(this->nexReconnect, SYSTEM_EVENT_STA_DISCONNECTED);

                        File file = store.open(STORE_PATH, FILE_WRITE);
                        if (!file) {
                            //erro ao abrir o arquivo
                        }
                        char result[100];
                        strcpy(result, this->ssid.c_str());
                        strcat(result, this->password.c_str());
                        if (file.print(result)) {
                            //salvo corretamente
                        }
                        else {
                            //erro ao escrever no arquivo
                        }
                        return checkInternetConnectionCallBack(sucess);
                    }
                    this->begin(this->ssid.c_str(), this->password.c_str());
                    usleep(this->microseconds);
                }
                this->failureDefault();
                return;
            }
        }
    }
    this->notFoundDefault();
    return;
}

void NexWiFiClass::startStaCallback(fs::FS& store, void (*sucess)(), void(*failure)(), void(*notFound)(), void(*failureInternet)()) {
    this->mode(WIFI_STA);
    this->disconnect();
    int numSsid = this->scanNetworks();

    for (int miss = 0; miss < this->attempt2Find; miss++, numSsid = this->scanNetworks()) {

        for (int thisNet = 0; thisNet < numSsid; thisNet++) {
            if (strcmp(this->ssid.c_str(), (this->SSID(thisNet).c_str())) == 0) {

                for (int failure = 0; failure < this->attempt2Connection; failure++) {
                    if (this->status() == WL_CONNECTED) {

                        //if (this->persistence) this->onEvent(this->nexReconnect, SYSTEM_EVENT_STA_DISCONNECTED);

                        File file = store.open(STORE_PATH, FILE_WRITE);
                        if (!file) {
                            //erro ao abrir o arquivo
                        }
                        char result[100];
                        strcpy(result, this->ssid.c_str());
                        strcat(result, this->password.c_str());
                        if (file.print(result)) {
                            //salvo corretamente
                        }
                        else {
                            //erro ao escrever no arquivo
                        }
                        return checkInternetConnectionCallBack(sucess, failureInternet);
                    }
                    this->begin(this->ssid.c_str(), this->password.c_str());
                    usleep(this->microseconds);
                }
                failure();
                return;
            }
        }
    }
    notFound();
    return;
}

void NexWiFiClass::nexDelay()
{
    long d;
    long milliseconds = microseconds / 1000;
    d = millis();


    while (millis() - d < milliseconds) {
        yield(); 
    }
}

void NexWiFiClass::nexReconnect()
{
    int countReset = 0;
    while (true) {
        if (!this->isConnected())
        {
            this->disconnect(false);
            this->mode(WIFI_STA);
            this->begin(this->ssid.c_str(), this->password.c_str());
            this->nexDelay(); //tempo maximo para att a variavel
            if (this->status() != WL_CONNECTED)
            {
                if (countReset >= this->attempt2Reconnection) {
                    ESP.restart();
                }
                countReset = countReset + 1;
            }
        }
        return;
    }
}

void NexWiFiClass::startServer(AsyncWebServer* server) {
    server->on("/", HTTP_GET, handle_OnConnect);
    server->on("/form", HTTP_POST, handle_RecvForm);
    server->onNotFound(handle_NotFound);
    server->begin();
    Serial.println("HTTP server started");
}

void handle_NotFound(AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "Not found");
}

/* http://192.168.4.1/ */
void handle_OnConnect(AsyncWebServerRequest* request) {
    request->send(200, "text/html", HTMLConfig);
}

/* http://192.168.4.1/form */
void handle_RecvForm(AsyncWebServerRequest* request) {
    String message = "No message sent";
    if (request->hasParam(PARAM_MESSAGE_SSID, true)) {
        message = request->getParam(PARAM_MESSAGE_SSID, true)->value();
        NexWiFi.setSsid(message.c_str());
    }
    if (request->hasParam(PARAM_MESSAGE_PASSWORD, true)) {
        message = request->getParam(PARAM_MESSAGE_PASSWORD, true)->value();
        NexWiFi.setPassword(message.c_str());
    }
    request->send(200, "text/plain", "salvo");
}

void NexWiFiClass::upServerDefault() {
    this->startApDefault();
    this->startServer(&serverDefault);
}

NexWiFiClass NexWiFi;
