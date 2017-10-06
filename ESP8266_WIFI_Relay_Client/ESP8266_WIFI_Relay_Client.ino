/*
 * Copyright (C) 2017 skydoves
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

const String deviceName = "ROOMLIGHTS";

const char* address = "address";
const int port = 21;

const char* ssid = "ssid";
const char* password = "password";

const int PIN_RELAY = 14;

ESP8266WiFiMulti WiFiMulti;

void setup() {
    Serial.begin(115200);
    delay(10);

    pinMode(PIN_RELAY, OUTPUT);
    digitalWrite(PIN_RELAY, LOW);
}

void connectAP(){
    // We start by connecting to a WiFi network
    WiFiMulti.addAP(ssid, password);

    Serial.println();
    Serial.print("Wait for WiFi");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
}

void loop() {
  // check wifi status
  if(WiFiMulti.run() != WL_CONNECTED)
    connectAP();
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  // try connect to server
  if (!client.connect(address, port)) {
    Serial.println("connection failed");
    return;
  }
  
  // check timeOut
  int timeout = millis() + 5000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println("Client Timeout !");
      client.stop();
      return;
    }
  }

  // This will send the set device name request to the server
  Serial.println("connected!");
  client.print("setName : " + deviceName);
  
  // Read all the lines of the reply from server and print them to Serial
   while (client.connected()) {
    if (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        handleMessage(line);
    } else if(!client.connected()){
      Serial.print("Server is closed!");
      break;
    }
  }

  // disconnected
  Serial.println("disconnected!");

  // waiting..
  delay(5000);
}

void handleMessage(String data){
    if(data.equals("on")) 
      digitalWrite(PIN_RELAY, LOW);
    else if(data.equals("off")) 
      digitalWrite(PIN_RELAY, HIGH);
}

