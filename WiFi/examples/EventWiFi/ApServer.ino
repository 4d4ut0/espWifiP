/*void startServer(AsyncWebServer *server){
  server->on("/", HTTP_GET, handle_OnConnect);
  server->on("/form", HTTP_POST, handle_RecvForm);
  server->onNotFound(handle_NotFound);
  server->begin();
  Serial.println("HTTP server started");
}

void handle_NotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// http://192.168.4.1/
void handle_OnConnect(AsyncWebServerRequest *request) {
  request->send(200, "text/html", HTMLConfig);
}
// http://192.168.4.1/form
void handle_RecvForm(AsyncWebServerRequest *request) {
  String message = "No message sent";
  if (request->hasParam(PARAM_MESSAGE_SSID, true)) {
    message = request->getParam(PARAM_MESSAGE_SSID, true)->value();
    ssid = message.c_str();
    Serial.println(message);
  }
  if (request->hasParam(PARAM_MESSAGE_PASSWORD, true)) {
    message = request->getParam(PARAM_MESSAGE_PASSWORD, true)->value();
    password = message.c_str();
    Serial.println(message);
  }
  request->send(200, "text/plain", "salvo");
}*/
