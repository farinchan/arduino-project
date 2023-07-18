#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Wire.h>

#define HTTP_REST_PORT 80
ESP8266WebServer server(HTTP_REST_PORT);

RTC_DS3231 rtc;  // for RTC
Servo myservo;   // for servo

LiquidCrystal_I2C lcd(0x27, 16, 2);  //LCD I2C

// const int buzzerPin = 16;
// const int whiteLedPin = 0;
// const int redLedPin = 2;

const int infrared = 16;  //D0

const int servoPin = 15;

//inisialisasi pin untuk 12c
const int SDAPin = 4;  //D2
const int SCLPin = 5;  //D1

// dibawah untuk Configurasi ke WIFI
// const char* ssid = "@Wifi_Bag_Umum";
// const char* password = "@BagianUmum";
// const char* ssid = "shafa33";
// const char* password = "193025manza";
const char* ssid = "inan";
const char* password = "123456780";


void setup() {

  Serial.begin(9600);
  restServerRouting();
  server.begin();

  Wire.begin(4, 5);  //Setting wire untuk I2C (4 untuk SDA dan 5 untuk SCL)
  WiFi.begin(ssid, password);

  lcd.begin();  // initialize the lcd
  lcd.backlight();

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("WIFI : ");
  lcd.print(ssid);

  lcd.setCursor(0, 1);
  while (WiFi.status() != WL_CONNECTED) {
    lcd.print(".");
    delay(800);
  }

  lcd.setCursor(0, 1);
  lcd.print("Terhubung");
  delay(700);



  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));  //Setting Time

  // pinMode(buzzerPin, OUTPUT);
  // pinMode(whiteLedPin, OUTPUT);
  // pinMode(redLedPin, OUTPUT);

  pinMode(infrared, INPUT);

  myservo.attach(servoPin);
  myservo.write(0);
}

void loop() {

  server.handleClient();
  DateTime now = rtc.now();  //Menampilkan RTC pada variable now

  if (digitalRead(infrared) == HIGH) {
    myservo.write(0);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(WiFi.localIP());

    lcd.setCursor(0, 1);
    lcd.print(now.day(), DEC);
    lcd.print('-');
    lcd.print(now.month(), DEC);
    lcd.print("-");
    lcd.print(now.year(), DEC);
    lcd.print(" ");
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
  }

  delay(100);
}

void restServerRouting() {
  server.on("/", HTTP_GET, []() {
    server.send(200, F("text/html"),
                F("Welcome to the REST Web Server"));
  });

  server.on(F("/scan"), HTTP_POST, scan);
}

void scan() {

  String message = "test";
  // for (uint8_t i = 0; i < server.args(); i++) { message =  server.arg(i) + "\n"; }
  server.send(200, "text/json", "{\"status\": \"ok\", \"uid\": " + server.arg(0) + " }");

  // //parsing json
  // const size_t capacity = JSON_OBJECT_SIZE(2) + 60;
  // DynamicJsonBuffer jsonBuffer(capacity);
  // JsonObject& root = jsonBuffer.parseObject(server.arg(0));

  char buf1[25];
  DateTime now = rtc.now();
  String waktuSekarang = String(sprintf(buf1, "%02d:%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year()));
  if (digitalRead(infrared) == LOW) {
    if (Serial.available()) {
      String data = Serial.readString();
      // Serial.println("Data diterima: " + data);
    }
    Serial.println("{\"uid\": \"" + server.arg(0) + "\" }");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan Success");
    lcd.setCursor(0, 1);
    // lcd.print("UID : " + root["uid"].as<String>());
    lcd.print("UID : " + server.arg(0));
    myservo.write(90);
  }
}
