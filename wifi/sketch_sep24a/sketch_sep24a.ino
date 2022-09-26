#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "SSacess"
#define APPSK "zaqmond"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

#define LED 2
int statusLED = HIGH;  // Начальный статус светодиода ВЫКЛЮЧЕН. Светодиод инверсный

const String HtmlHtml = "<html><head><meta charset=\"utf-8\">"
                        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlTitle = "<h1>Мигаем системным светодиодом из созданной точки доступа</h1><br/>\n";
const String HtmlLedStateLow = "<big>Светодиод <b>Включен</b></big><br/><br/>\n";
const String HtmlLedStateHigh = "<big>Светодиод <b>Выключен</b></big><br/><br/>\n";
const String HtmlButtons =
  "<a href=\"LEDOff\"><button style=\"background-color:red;color:white;width:20%;height:5%;\">Выключить </button></a>\n"
  "<a href=\"LEDOn\"><button style=\"background-color:green;color:white;width:20%;height:5%;\">Включить </button></a><br/>";
const String HtmlHtmlClose = "</html>";

void handleLedOn() {
  statusLED = HIGH;  // conditions to make the LED turn on
  digitalWrite(LED, statusLED);
  Serial.println("Светодиод >Выключен");
  response();
}

void handleLedOff() {
  statusLED = LOW;  // conditions to make the LED turn off
  digitalWrite(LED, statusLED);
  Serial.println("Светодиод Включен");
  response();
}

void response() {
  String htmlRes = HtmlHtml + HtmlTitle;
  if (statusLED == LOW) {
    htmlRes += HtmlLedStateLow;
  } else {
    htmlRes += HtmlLedStateHigh;
  }

  htmlRes += HtmlButtons;
  htmlRes += HtmlHtmlClose;
  server.send(200, "text/html", htmlRes);
}

void setup() {
  delay(1000);
  WiFi.softAP(ssid, password);
  IPAddress apip = WiFi.softAPIP();
  server.on("/", response);
  server.on("/LEDOff", handleLedOn);
  server.on("/LEDOn", handleLedOff);
  server.begin();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, statusLED);
}

void loop() {
  server.handleClient();
}
