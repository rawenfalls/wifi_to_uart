#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "System Access"
#define APPSK  "Password"
#endif

//char str[32] = "";
char data[50];
const char *ssid = APSSID;
const char *password = APPSK;
int machine=1;
IPAddress apip;
ESP8266WebServer server(80);
const String HtmlHtml = "<html><head><meta charset=\"utf-8\">"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlTitle = "<h1>Включаем пид регулятор</h1><br/>\n";
const String HtmlLedStateLow = "<big>Пид регулятор <b>Включен</b></big><br/><br/>\n";
const String HtmlLedStateHigh = "<big>Пид регулятор <b>Выключен</b></big><br/><br/>\n";
const String HtmlButtons = 
"<a href=\"LEDOff\"><button style=\"background-color:red;color:white;width:20%;height:5%;\">Выключить </button></a>\n"
"<a href=\"LEDOn\"><button style=\"background-color:green;color:white;width:20%;height:5%;\">Включить </button></a><br/>";
const String HtmlHtmlClose = "</html>";
int statPId = 0;
int pid_on;
//int sec= 0 ;
//int minuts=0;
//int hours=0;
void handlePidOn() {
  //statusLED = HIGH; // conditions to make the PID turn on
  pid_on = 0;
  statPId = 1;
  //Serial.println("PID off");
  response();
}

void handlePidOff() {
 //statusLED = LOW; // conditions to make the PID turn off
  pid_on = 1;
  statPId = 0;
  machine=1;
  //Serial.println("PID on");
  response();
}

void response(){
  String htmlRes = HtmlHtml + HtmlTitle+ data + data;
  if(statPId == 0){
    htmlRes += HtmlLedStateLow;
  }else{
    htmlRes += HtmlLedStateHigh;
  }
 
  htmlRes += HtmlButtons;
  htmlRes += HtmlHtmlClose; 
  server.send(200, "text/html", htmlRes);
}

void setup() {
  pid_on = 0;
  Serial.begin(9600);
  Serial.setTimeout(1000);
  WiFi.softAP(ssid, password);
  Serial.print("Soft-AP IP address = ");
  apip = WiFi.softAPIP();
  server.on("/", response); 
  server.on("/LEDOff", handlePidOn);
  server.on("/LEDOn", handlePidOff); 
  server.begin();   
  Serial.println(apip);
}

void loop() {
  if (Serial.available()){

    int amount = Serial.readBytesUntil('\n',data ,50);
    data[amount] = NULL;
    amount+=1;
    //Serial.println(data);
    }
    if (pid_on == 1 && machine==1){
      machine=0;
      char data1[3];
      data1[0] = 'p';
      data1[1] = '\n';
      data1[2] = NULL;
      Serial.print(data1);
      pid_on = 0;
      }
      
    //Serial.println('p');
  //Serial.println(apip);
  //delay(500);
  server.handleClient();
}

String webPage()
{
  String web; 
  web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/> <meta charset=\"utf-8\"><title>ESP 8266</title><style>button{color:red;padding: 10px 27px;}</style></head>";
  web += "<h1 style=\"text-align: center;font-family: Open sans;font-weight: 100;font-size: 20px;\">ESP8266 Web Server</h1><div>";
  //++++++++++ LED-1  +++++++++++++
  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">----LED 1----</p>";
  if (digitalRead(D0_pin) == 1)
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #43a209;margin: 0 auto;\">ON</div>";
  }
  else 
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #ec1212;margin: 0 auto;\">OFF</div>";
  }
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></div>";
  // ++++++++ LED-1 +++++++++++++
  
  //++++++++++ LED-2  +++++++++++++
  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">----LED 2----</p>";
  if (digitalRead(D2_pin) == 1)
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #43a209;margin: 0 auto;\">ON</div>";
  }
  else 
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #ec1212;margin: 0 auto;\">OFF</div>";
  }
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></div>";
  // ++++++++ LED-2 +++++++++++++
  
  //++++++++++ LED-3  +++++++++++++
  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">----LED 3----</p>";
  if (digitalRead(D1_pin) == 1)
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #43a209;margin: 0 auto;\">ON</div>";
  }
  else 
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #ec1212;margin: 0 auto;\">OFF</div>";
  }
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socket3On\"><button>ON</button></a>&nbsp;<a href=\"socket3Off\"><button>OFF</button></a></div>";
  // ++++++++ LED-2 +++++++++++++
  
  // ========REFRESH=============
  web += "<div style=\"text-align:center;margin-top: 20px;\"><a href=\"/\"><button style=\"width:158px;\">REFRESH</button></a></div>";
  // ========REFRESH=============
  
  
  web += "</div>";
  return(web);
}
