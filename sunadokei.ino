#include <WiFi.h>

const char SSID[] = "htwireless";
const char PASSWORD[] = "";

WiFiServer server(80);

int tiltPin = 15;
int digitalVal;

//時間変数
float nowTime;
float elapsedTime;
float trigerTime = 10000;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  WiFi.begin(SSID, PASSWORD);
  Serial.print("WiFi connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println(" connected");

  server.begin();

  Serial.print("HTTP Server: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  pinMode(tiltPin, INPUT_PULLUP);
  digitalVal = digitalRead(tiltPin);
  nowTime = millis();
  elapsedTime = 0;
  
}

void loop() {
  WiFiClient client = server.available();
//  if (client) {
//    client.println("Hello World!");
//    client.stop();
//  }


  if(digitalVal != digitalRead(tiltPin)){
    Serial.println("Changed");
    digitalVal = digitalRead(tiltPin);
    nowTime = millis();
    elapsedTime = 0;
  }else{
    elapsedTime = millis() - nowTime;
    if(elapsedTime >= trigerTime){
      client.println(prepareHtmlPage("FINISH!"));
    }else{
      client.println(prepareHtmlPage(String(elapsedTime)));
    }
  }
//  if(digitalVal == HIGH){
//    Serial.println("HIGH");
//    client.println(prepareHtmlPage(digitalVal));
////    client.stop();
//  }else{
//    Serial.println("LOW");
//    client.println(prepareHtmlPage(digitalVal));
////    client.stop();
//  }
}

String prepareHtmlPage(String moji)
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "Refresh: 0.2\r\n" +  // refresh the page automatically every 5 sec
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html style=\"text-align:center\">" +
             "<h2>" + moji + "</h2>" +
            "</html>" +
            "\r\n";
  return htmlPage;
}
