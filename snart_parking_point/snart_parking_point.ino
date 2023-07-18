#include <ArduinoJson.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>


#define HTTP_REST_PORT 80
ESP8266WebServer server(HTTP_REST_PORT);

Servo servo1;  // for servo
Servo servo2;  // for servo

const int servo1Pin = 16;  //D0
const int servo2Pin = 5;   //D1

// dibawah untuk Configurasi ke WIFI
const char* ssid = "inan";
const char* password = "123456780";

String URLServer = "http://192.168.213.24:3000/api/scan/a1";  //IP SERVER
WiFiClient client;
HTTPClient httpClient;


void setup() {
  Serial.begin(115200);
  restServerRouting();
  server.begin();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(800);
  }
  Serial.println("Terhubung");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  server.handleClient();
  servo1.write(0);
  servo2.write(0);
}


void restServerRouting() {
  server.on("/", HTTP_GET, []() {
    server.send(200, F("text/html"),
                F("Welcome to the REST Web Server"));
  });

  server.on(F("/a1"), HTTP_POST, scanA1);
  server.on(F("/a2"), HTTP_POST, scanA2);
}

void scanA1() {

  Serial.println("{\"uid\": \"" + server.arg(0) + "\" }");

  String data = "uid=" + server.arg(0);
  httpClient.begin(client, URLServer);
  httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
  httpClient.POST(data);
  String content = httpClient.getString();
  httpClient.end();

  servo1.write(90);
  server.send(200, "text/json", content);
  delay(1000);
}

void scanA2() {

  server.send(200, "text/json", "{\"status\": \"ok\", \"uid\": " + server.arg(0) + " }");


  Serial.println("{\"uid\": \"" + server.arg(0) + "\" }");
  servo2.write(90);
  delay(1000);
}
