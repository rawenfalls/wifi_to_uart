// Ссылка на библиотеку https://github.com/miguelbalboa/rfid
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <SPI.h>                      //Подключение библиотеки SPI
//#include <MFRC522.h>                  //Подключение библиотеки RFID
#ifndef APSSID
#define APSSID "System"
#define APPSK  "Pass"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

#define SS_PIN D4                     //На D4 подключен контакт SDA
#define RST_PIN D3                    //На D3 подключен контакт RST

//MFRC522 mfrc522(SS_PIN, RST_PIN);     // Создаём экземпляр класса MFRC522

#define ON_Board_LED 2                // Встроенный светодиод на пине 2

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

void handleRoot() {
String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Rfid КАРТА</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 15px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Вэб страница для показа информации о RFID карте</h1>\n";  
  ptr +="<h2 style=\"color:#2e86c1;\">Вывод информации о карте: <span style=\"color:#17a589;\" id=\"UIDResult\">Ждём'с</span></h2>";  
  ptr +="</div>\n";
  ptr +="<script>";
  ptr +="setInterval(function() {";
  ptr +="getTemperatureData();";
  ptr +="}, 100);";
  ptr +="function getTemperatureData() {";
  ptr +="var xhttp = new XMLHttpRequest();";
  ptr +="xhttp.onreadystatechange = function() {";
  ptr +="if (this.readyState == 4 && this.status == 200) {";
  ptr +="document.getElementById(\"UIDResult\").innerHTML = this.responseText;";
  ptr +="}";
  ptr +="};";
  ptr +="xhttp.open(\"GET\", \"UIDResult\", true);";
  ptr +="xhttp.send();";
  ptr +="}";
  ptr +="</script>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  server.send(200, "text/html", ptr);       //Отправляем страницу на сервер
}

void handleUIDResult() {
  readsuccess = getid();
 
  if(readsuccess) {
    digitalWrite(ON_Board_LED, LOW);          // Включить при связи с роутером
    server.send(200, "text/plane", StrUID);   
    digitalWrite(ON_Board_LED, HIGH);         // Погасить светодиод
  }
}

int getid() {  
  if(!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }  
  for(int i=0;i<4;i++){
    readcard[i]=mfrc522.uid.uidByte[i];         //хранение идентификатора карты
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}

void array_to_string(byte array[], unsigned int len, char buffer[]) {
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}



void setup() {
//  Serial.begin(9600);               //Инициализируем серийный порт на скорости 115200
//  pinMode(ON_Board_LED,OUTPUT); 
//  digitalWrite(ON_Board_LED, HIGH);   // Выключить светодиод
//  SPI.begin();                        //Инициализируем шину SPI
//  mfrc522.PCD_Init();                 //Инициализируем MFRC522
//  delay(500);
  /* Вы можете удалить параметр password, если хотите, чтобы точка доступа была открыта. */
  WiFi.softAP(ssid, password);
  IPAddress apip = WiFi.softAPIP();   // Получить IP

  server.on("/", handleRoot);         // Корневой каталог для отображения web СТРАНИЦЫ
  server.on("/UIDResult", handleUIDResult);  

  server.begin();                     // Стартуем сервер
}

void loop() {
  server.handleClient();
}
