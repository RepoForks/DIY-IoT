
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const int PIN_RELAY = 14;

void setup() {
  Serial.begin(115200);
    
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);

  boolean result = WiFi.softAP("Jarvis_wifi", "1234567890");
  if(result == true) {
    Serial.println("AP mode success!");
  } else {
    Serial.println("AP mode Failed!");
  }

  server.on("/", []() {
    server.send(200, "text/html", setHtemlBrowser());
  });

  server.on("/relay", []() {
    handleGPIO();
    server.send(200, "text/html", setHtemlBrowser());
    delay(1000);
  });
  
  server.begin();
}

void loop(void){
  server.handleClient();
} 

String setHtemlBrowser() {
  return 
  "<!DOCTYPE html>"
  "<html>"
    "<head>"
      "<style>"
        "button {"
            "background-color: #4CAF50;"
            "border: none;"
            "color: white;"
            "padding: 10px 24px;"
            "text-align: center;"
            "text-decoration: none;"
            "display: inline-block;"
            "font-size: 16px;"
          "}"
      "</style>"
    "</head>"
    "<body>"
      "<h1>Welcome to Jarvis Web Server!</h1>"
      "<h3>Desk MuliTap</h3>"
      "<a href=\"relay\"><button style=\"background-color:" + getButtonColor() + ";\">" + getButtonLabel() + "</button></a>"
    "</body>"
  "</html>";
}

void handleGPIO() {
  if(digitalRead(PIN_RELAY) == LOW) {
    digitalWrite(PIN_RELAY, HIGH);
  } else {
    digitalWrite(PIN_RELAY, LOW);
  }
}

String getButtonColor() {
  if(digitalRead(PIN_RELAY) == LOW) {
    return getGreenColorCode();
  } else {
    return geteRedColorCode();
  }
}

String getButtonLabel() {
  if(digitalRead(PIN_RELAY) == LOW) {
    return "Switch ON";
  } else {
    return "Switch OFF";
  }
}

String getGreenColorCode() {
  return "#4CAF50";
}

String geteRedColorCode() {
  return "#f44336";
}
