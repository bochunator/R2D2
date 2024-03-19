#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ModbusRTU.h>

#define REGN 0               //Start Address +1
#define SLAVE_ID 1            //Slave Addr
ModbusRTU mb;

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="refresh" content="10">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .sensors-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 sensors Server</h2>
  <p>
    <span class="sensors-labels">Temperature</span> 
    <span id="TEMPERATURE:">%TEMPERATURE%</span>
  </p>
  
)rawliteral";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600, SERIAL_8N1);
  #if defined(ESP32) || defined(ESP8266)
  mb.begin(&Serial);
#else
  mb.begin(&Serial);
  //mb.begin(&Serial, RXTX_PIN);  //or use RX/TX direction control pin (if required)
  mb.setBaudrate(9600);
#endif
  mb.slave(SLAVE_ID);
  for(int i = 0; i < 10; i++){
    mb.addHreg(REGN + i);
  }
  WiFi.softAP("KoniowifiESP32");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<10; i++){
    mb.Hreg(REGN + i, random(12000));
  }
  mb.task();
  yield();
}

String processor(const String& var){
  String html = "";
  return html;
}