#include <NexWiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "SouzaLesmo";
const char* password =  "Fusca1300";

//AsyncWebServer serverDefault(80);

/*
* WiFi Events
0  SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
1  SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
2  SYSTEM_EVENT_STA_START                < ESP32 station start
3  SYSTEM_EVENT_STA_STOP                 < ESP32 station stop
-->4  SYSTEM_EVENT_STA_CONNECTED            < ESP32 station connected to AP
-->5  SYSTEM_EVENT_STA_DISCONNECTED         < ESP32 station disconnected from AP
6  SYSTEM_EVENT_STA_AUTHMODE_CHANGE      < the auth mode of AP connected by ESP32 station changed
7  SYSTEM_EVENT_STA_GOT_IP               < ESP32 station got IP from connected AP
8  SYSTEM_EVENT_STA_LOST_IP              < ESP32 station lost IP and the IP is reset to 0
9  SYSTEM_EVENT_STA_WPS_ER_SUCCESS       < ESP32 station wps succeeds in enrollee mode
10 SYSTEM_EVENT_STA_WPS_ER_FAILED        < ESP32 station wps fails in enrollee mode
11 SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       < ESP32 station wps timeout in enrollee mode
12 SYSTEM_EVENT_STA_WPS_ER_PIN           < ESP32 station wps pin code in enrollee mode
13 SYSTEM_EVENT_AP_START                 < ESP32 soft-AP start
14 SYSTEM_EVENT_AP_STOP                  < ESP32 soft-AP stop
15 SYSTEM_EVENT_AP_STACONNECTED          < a station connected to ESP32 soft-AP
16 SYSTEM_EVENT_AP_STADISCONNECTED       < a station disconnected from ESP32 soft-AP
17 SYSTEM_EVENT_AP_STAIPASSIGNED         < ESP32 soft-AP assign an IP to a connected station
18 SYSTEM_EVENT_AP_PROBEREQRECVED        < Receive probe request packet in soft-AP interface
19 SYSTEM_EVENT_GOT_IP6                  < ESP32 station or ap or ethernet interface v6IP addr is preferred
20 SYSTEM_EVENT_ETH_START                < ESP32 ethernet start
21 SYSTEM_EVENT_ETH_STOP                 < ESP32 ethernet stop
22 SYSTEM_EVENT_ETH_CONNECTED            < ESP32 ethernet phy link up
23 SYSTEM_EVENT_ETH_DISCONNECTED         < ESP32 ethernet phy link down
24 SYSTEM_EVENT_ETH_GOT_IP               < ESP32 ethernet got IP from connected AP
25 SYSTEM_EVENT_MAX
*/
 
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Connected to AP!");
 
    Serial.print("SSID Length: ");
    Serial.println(info.connected.ssid_len);
 
    Serial.print("SSID: ");
    for(int i=0; i<info.connected.ssid_len; i++){
      Serial.print((char) info.connected.ssid[i]);
    }
 
    Serial.print("\nBSSID: ");
    for(int i=0; i<6; i++){
      Serial.printf("%02X", info.connected.bssid[i]);
 
      if(i<5){
        Serial.print(":");
      }
    }
     
    Serial.print("\nChannel: ");
    Serial.println(info.connected.channel);
 
    Serial.print("Auth mode: ");
    Serial.println(info.connected.authmode);  
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    NexWiFi.nexReconnect();
}

void sucess(){
  NexWiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
  //NexWiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  Serial.println(NexWiFi.localIP());
}

void failure(){
  Serial.println("Erro");
}

void setup()
{
    Serial.begin(115200);
    SPIFFS.begin(true);
    //NexWiFi.setSsid(ssid);
    //NexWiFi.setPassword(password);

    Serial.println("--------------------");
    NexWiFi.startStaCallback(SPIFFS, sucess);
    Serial.println("--------------------");
    
    
    delay(5000); 
}
 
void loop(){
  Serial.println("main loop..");
  delay(5500);  
}
