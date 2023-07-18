#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>

WiFiClient wifiClient;

#define RST_PIN D3                  //rfid
#define SDA_PIN D4                  //rfid
MFRC522 mfrc522(SDA_PIN, RST_PIN);  //RFID RC522

LiquidCrystal_I2C lcd(0x27, 16, 2);  //LCD I2C

RTC_DS3231 rtc;  // for RTC
Servo myservo;   // for servo

const int buzzerPin = 16;

// dibawah untuk Configurasi ke WIFI
const char* ssid = "inan";
const char* password = "12345678";

void setup() {
  Wire.begin(4, 5);  //Setting wire untuk I2C (4 untuk SDA dan 5 untuk SCL)

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  lcd.begin();  // initialize the lcd
  lcd.backlight();

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

  SPI.begin();
  mfrc522.PCD_Init();

  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));  //Setting Time

  pinMode(buzzerPin, OUTPUT);
  myservo.attach(15);
  myservo.write(0);
}

void loop() {
  // Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Absensi RFID");

  DateTime now = rtc.now();  //Menampilkan RTC pada variable now

  lcd.setCursor(0, 1);
  lcd.print(now.day(), DEC);
  lcd.print('-');
  lcd.print(now.month(), DEC);
  lcd.print(" ");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);

  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {

      digitalWrite(buzzerPin, HIGH);
      myservo.write(180);

      String content = "";

      for (byte i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      lcd.setCursor(0, 0);
      lcd.print("Pesan : Berhasil");
      Serial.println("Pesan : Berhasil");
      lcd.setCursor(0, 1);
      lcd.print("ID:");
      lcd.print(content);
      Serial.println(content);
      Serial.println();

      delay(200);
      digitalWrite(buzzerPin, LOW);
      myservo.write(0);
      delay(100);


      HTTPClient http;
      http.begin(wifiClient, "http://rfid.gariskode.com/absensi/scan");
      // http.begin(wifiClient, "http://192.168.100.198:3000/api/rfid");
      http.addHeader("Content-Type", "application/json");
      String waktu = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());  // simpan informasi waktu dalam satu variabel
      String data = "{\"tag\":\"" + content + "\", \"time\" : \"" + waktu + "\"}";

      Serial.println(data);

      int httpCode = http.POST(data);
      Serial.println(httpCode);

      if (httpCode > 0) {
        String payload = http.getString();
        Serial.println(payload);
      }
      http.end();
      myservo.write(90);

      delay(2000);
    }
  }

  delay(1000);
}
