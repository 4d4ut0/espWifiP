#ifndef NexWiFi_h
#define NexWiFi_h

#define PASSWORD_AP "nexsolar"
#define PREFIX_AP_SSID "NexIoT_"

#define SSID_NOT_FOUND -3
#define CONNECTION_FAILURE -4
#define SUCCESSFUL_INTERNET_CONNECTION 1
#define FAILURE_IN_INTERNET_CONNECTION 0

#define FAILED_ATTEMPT 30 

#define CHECK_INTERNET true
#define NOT_CHECK_INTERNET false
#define PERSISTENT true
#define NOT_PERSISTENT false

#define PING_1 "https://www.google.com/"
#define STORE_PATH "/wifi.txt"

#define HTMLConfig "<!DOCTYPE HTML><html lang=\"pt-BR\"><head>    <meta charset=\"UTF-8\">    <title>Configuração</title>    <style>body {    }h1 {    text-align: center;}form {    /* Apenas para centralizar o form na página */    margin: 0 auto;    width: 400px;    /* Para ver as bordas do formulário */    padding: 1em;    border: 1px solid #CCC;    border-radius: 1em;}form div + div {    margin-top: 1em;}label {    /*Para ter certeza que todas as labels tem o mesmo tamanho e estão propriamente alinhadas */    display: inline-block;    width: 90px;    text-align: right;}input, textarea {    /* Para certificar-se que todos os campos de texto têm as mesmas configurações de fonte. Por padrão, textareas ter uma fonte monospace*/    font: 1em sans-serif;    /* Para dar o mesmo tamanho a todos os campo de texto */    width: 300px;    -moz-box-sizing: border-box;    box-sizing: border-box;    /* Para harmonizar o look & feel das bordas nos campos de texto*/    border: 1px solid #999;}input:focus, textarea:focus {    /* Dar um pouco de destaque nos elementos ativos */    border-color: #000;}textarea {    /* Para alinhar corretamente os campos de texto de várias linhas com sua label*/    vertical-align: top;    /* Para dar espaço suficiente para digitar algum texto */    height: 5em;    /* Para permitir aos usuários redimensionarem qualquer textarea verticalmente. Ele não funciona em todos os browsers */    resize: vertical;}.button {    /* Para posicionar os botões para a mesma posição dos campos de texto */    text-align: center;}button {    /* Esta margem extra representa aproximadamente o mesmo espaço que o espaço entre as labels e os seus campos de texto*/    margin-left: .5em;    text-align: center;}</style></head><body><form action=\"/form\" method=\"post\">    <div>        <h1>Configuração de Rede</h1>    </div>    <div>        <label for=\"ssid\">Nome:</label>        <input type=\"text\" id=\"ssid\" name=\"ssid\" />    </div>    <div>        <label for=\"password\">Senha:</label>        <input type=\"password\" id=\"password\" name=\"password\" />    </div>      <div class=\"button\">        <button type=\"submit\">Salvar</button>    </div></form></body></html>"
#define PARAM_MESSAGE_SSID "ssid"
#define PARAM_MESSAGE_PASSWORD "password"

#include <stdint.h>
#include <string>

#include "Print.h"
#include "IPAddress.h"
#include "IPv6Address.h"

#include "WiFiType.h"
#include "WiFiSTA.h"
#include "WiFiAP.h"
#include "WiFiScan.h"
#include "WiFiGeneric.h"

#include "WiFiClient.h"
#include "WiFiServer.h"
#include "WiFiUdp.h"

#include "HTTPClient.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

void testA(WiFiEvent_t event, WiFiEventInfo_t info);

void testB(Print& p);

void handle_NotFound(AsyncWebServerRequest* request);
void handle_OnConnect(AsyncWebServerRequest* request);
void handle_RecvForm(AsyncWebServerRequest* request);

class NexWiFiClass : public WiFiGenericClass, public WiFiSTAClass, public WiFiScanClass, public WiFiAPClass
{
private:
    std::string ssid;
    std::string password; 
    //fs::FS& store;

    bool checkConnection;
    bool persistence;
    int attempt2Find;
    int attempt2Connection;
    int attempt2Reconnection;
    unsigned int microseconds;

public:
    using WiFiGenericClass::channel;

    using WiFiSTAClass::SSID;
    using WiFiSTAClass::RSSI;
    using WiFiSTAClass::BSSID;
    using WiFiSTAClass::BSSIDstr;

    using WiFiScanClass::SSID;
    using WiFiScanClass::encryptionType;
    using WiFiScanClass::RSSI;
    using WiFiScanClass::BSSID;
    using WiFiScanClass::BSSIDstr;
    using WiFiScanClass::channel;


public:
    NexWiFiClass();

    void setSsid(std::string ssid);
    void setPassword(std::string password);
    void setCheckConnection(bool checkConnection);
    void setPersistence(bool persistence);
    void setAttempt2Find(int attempt2Find);
    void setAttempt2Connection(int attempt2Connection);
    void setMicroseconds(unsigned int microseconds);

    void sucessDefault();
    void failureDefault();
    void notFoundDefault();
    void failureInternetDefault();

    void nexDelay();
    void printDiag(Print& dest);
    void checkInternetConnectionCallBack(void (*sucess)());
    void checkInternetConnectionCallBack(void (*sucess)(), void(*failureInternet)());
    int checkInternetConnectionDefault();
    int startStaDefault(fs::FS& store);
    void startApDefault();
    void startStaCallback(fs::FS& store, void (*sucess)());
    void startStaCallback(fs::FS& store, void (*sucess)(), void(*failure)(), void(*notFound)(), void(*failureInternet)());
    void nexReconnect();

    void startServer(AsyncWebServer* server);
    void upServerDefault();
    //void upServer(AsyncWebServer* server);

    friend class WiFiClient;
    friend class WiFiServer;
    friend class WiFiUDP;
};

extern NexWiFiClass NexWiFi;

#endif
