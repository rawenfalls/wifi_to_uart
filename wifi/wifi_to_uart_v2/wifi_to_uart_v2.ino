#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "Reostat"
#define APPSK  "Password"
#endif

int nagrew = 0;
int led = 0;
int culer = 0;
int pid = 0;

//char str[32] = "";
char data[50];
const char *ssid = APSSID;
const char *password = APPSK;
int machine=1;
IPAddress apip;
ESP8266WebServer server(80);

void setup() {
  //pid_on = 0;
  Serial.begin(9600);
  Serial.setTimeout(1000);
  WiFi.softAP(ssid, password);
  Serial.print("Soft-AP IP address = ");
  apip = WiFi.softAPIP();

  //+++++++++++++++++++++++ START  nagrew ++++++++++++++++++++
  server.on("/", [](){
    server.send(200, "text/html", webPage());
  });
  server.on("/socket1On", [](){
    nagrew = 1;
    server.send(200, "text/html", webPage());
    delay(100);
    
  });
  server.on("/socket1Off", [](){
    nagrew = 0;
    server.send(200, "text/html", webPage());
    delay(100);
   //+++++++++++++++++++++++ END  nagrew ++++++++++++++++++++ 
    
   //+++++++++++++++++++++++ START  led  ++++++++++++++++++++ 
  });
  server.on("/socket2On", [](){
    led = 1;
    server.send(200, "text/html", webPage());
    delay(100);    
  });
  server.on("/socket2Off", [](){
    led = 0;
    server.send(200, "text/html", webPage());
    delay(100);
    
   // +++++++++++++++++++++++ END  led ++++++++++++++++++++
   
   //+++++++++++++++++++++++ START  culer  ++++++++++++++++++++ 
  });
  server.on("/socket3On", [](){
    culer = 1;
    server.send(200, "text/html", webPage());
    delay(100);    
  });
  server.on("/socket3Off", [](){
    culer = 0;
    server.send(200, "text/html", webPage());
    delay(100);
    
   // +++++++++++++++++++++++ END  culer ++++++++++++++++++++
    
  //+++++++++++++++++++++++ START  pid  ++++++++++++++++++++
  });
   server.on("/socket4On", [](){
    pid = 1;
    server.send(200, "text/html", webPage());
    delay(100);    
  });
  server.on("/socket4Off", [](){
    pid = 0;
    server.send(200, "text/html", webPage());
    delay(100);
    
   // +++++++++++++++++++++++ END  pid ++++++++++++++++++++
    
    
  });

  server.begin();
  Serial.println("HTTP server started");
  Serial.println(apip);
}
unsigned long last_time = 0;
int change1 = 0;
int change2 = 0;
int change3 = 0;
int change4 = 0;
void loop() {
  
  if (millis() - last_time > 1000)
  {
    if ((change1 != nagrew) || (change2 != led) || (change3 != culer) || (change4 != pid)){
    change1 = nagrew;
    change2 = led;
    change3 = culer;
    change4 = pid;
    Serial.print(nagrew);
    Serial.print(";");
    Serial.print(led);
    Serial.print(";");
    Serial.print(culer);
    Serial.print(";");
    Serial.print(pid);
    Serial.println(";");
    }
    last_time = millis();
  }
  // if (Serial.available()){

  //  int amount = Serial.readBytesUntil('\n',data ,50);
  //  data[amount] = NULL;
  //  amount+=1;
  //  //Serial.println(data);
  //  }
    // if (pid_on == 1 && machine==1){
    //  machine=0;
    //  char data1[3];
    //  data1[0] = 'p';
    //  data1[1] = '\n';
    //  data1[2] = NULL;
    //  Serial.print(data1);
    //  pid_on = 0;
    //  }
    //Serial.println('p');
  //Serial.println(apip);
  //delay(500);
  server.handleClient();
}


String webPage()
{
  String web; 
  web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/> <meta charset=\"utf-8\"><title>ESP 8266</title><style>button{color:red;padding: 10px 27px;}</style></head>";
  web += "<h1 style=\"text-align: center;font-family: Open sans;font-weight: 100;font-size: 20px;\">Reostat Server</h1><div>";
  //++++++++++ нагрев  +++++++++++++
  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">----nagrew----</p>";
  if (nagrew == 1)
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #43a209;margin: 0 auto;\">ON</div>";
  }
  else 
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #ec1212;margin: 0 auto;\">OFF</div>";
  }
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></div>";
  // ++++++++ нагрев +++++++++++++
  
  //++++++++++ подсветка  +++++++++++++
  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">----led----</p>";
  if (led == 1)
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #43a209;margin: 0 auto;\">ON</div>";
  }
  else 
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #ec1212;margin: 0 auto;\">OFF</div>";
  }
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></div>";
  // ++++++++ подсветка +++++++++++++
  
  //++++++++++ кулер  +++++++++++++
  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">----culer----</p>";
  if (culer == 1)
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #43a209;margin: 0 auto;\">ON</div>";
  }
  else 
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #ec1212;margin: 0 auto;\">OFF</div>";
  }
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socket3On\"><button>ON</button></a>&nbsp;<a href=\"socket3Off\"><button>OFF</button></a></div>";
  // ++++++++ кулер +++++++++++++
  
    //++++++++++ pid  +++++++++++++
  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">----pid----</p>";
  if (pid == 1)
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #43a209;margin: 0 auto;\">ON</div>";
  }
  else 
  {
    web += "<div style=\"text-align: center;width: 98px;color:white ;padding: 10px 30px;background-color: #ec1212;margin: 0 auto;\">OFF</div>";
  }
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socket4On\"><button>ON</button></a>&nbsp;<a href=\"socket4Off\"><button>OFF</button></a></div>";
  // ++++++++ pid +++++++++++++

  // ========REFRESH=============
  web += "<div style=\"text-align:center;margin-top: 20px;\"><a href=\"/\"><button style=\"width:158px;\">REFRESH</button></a></div>";
  // ========REFRESH=============
  
  
  web += "</div>";
  return(web);
}
