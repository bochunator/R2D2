#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

WebServer server(80);

void setup() {
    Serial.begin(115200);
    WiFi.softAP("KoniowifiESP32");
    MDNS.begin("esp32"); // http://esp32.local/
    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.on("/login", HTTP_GET, handleLogin);
    server.on("/connect", HTTP_POST, handleConnect);
    server.begin();
    previousTime = millis();
}

void loop() {
    server.handleClient();
}

void handleRoot() {
    String html = R"(
        <style>
            body {
                font-size: 5rem;
            }
            button {
                padding: 55px;
                border-radius: 50px; 
                border-style: none;
                background-color: hsl(99, 97%, 61%);
                color: #FFF;
                font-weight: bold;
                cursor: pointer;
                font-size: 0.7em;
            }
            button:hover {
                background-color: hsl(99, 97%, 71%);
            }
            button:active {
                background-color: hsl(99, 97%, 81%);
            }
        </style>
        <h3 style="text-align: center; color: hsl(99, 97%, 61%); margin: 100px;">Hello from esp32!</h3>
        <div style="text-align: center;">
            <a href="/login"><button style="">Login to router</button></a>
        </div>
    )";
    server.send(200, "text/html", html);
}

void handleNotFound() {
    server.send(404, "text/plain", "Have you lost?");
}

void handleLogin() {
    String html = R"(
        <style>
            body {
                font-size: 5rem;
            }
            button {
                padding: 55px;
                border-radius: 50px; 
                border-style: none;
                background-color: hsl(99, 97%, 61%);
                color: #FFF;
                font-weight: bold;
                cursor: pointer;
                font-size: 0.7em;
            }
            button:hover {
                background-color: hsl(99, 97%, 71%);
            }
            button:active {
                background-color: hsl(99, 97%, 81%);
            }
            div {
                margin: 50px;
            }
            input {
                font-size: 0.5em;
                width: 300px;
                height: 100px;
            }
        </style>
        <form action='/connect' method='POST' style="text-align: center;">
            <div>
                <label for="ssid">ssid:</label>
                <input type="text" id="ssid" name="ssid">
            </div>
            <div>
                <label for="password">password:</label>
                <input type="text" id="password" name="password">
            </div>
            <div>
                <a href="/"><button type='button' style="background-color: hsl(19, 100%, 50%);">Cancel</button></a>
                <button type='submit'>Connect</button>
            </div>
        </form>
    )";
    server.send(200, "text/html", html);
}

void handleConnect() {
    ssid = server.arg("ssid");
    password = server.arg("password");
    reconnect = true;
    WiFi.begin(ssid, password);
    server.sendHeader("Location", "/");
    server.send(303);
}
