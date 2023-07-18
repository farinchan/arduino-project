#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <MFRC522.h>

WiFiClient wifiClient;

#define RST_PIN D3                  //rfid
#define SDA_PIN D4                  //rfid
MFRC522 mfrc522(SDA_PIN, RST_PIN);  //RFID RC522


const int buzzerPin = 16;

// dibawah untuk Configurasi ke WIFI
const char* ssid = "inan";
const char* password = "12345678";

void setup() {

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
      Serial.print(".");
  }

  SPI.begin();
  mfrc522.PCD_Init(); 

  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  digitalWrite(buzzerPin, LOW);


  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {

      digitalWrite(buzzerPin, HIGH);

      String content = "";

      for (byte i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }

      Serial.println("Pesan : Berhasil");
      Serial.println(content);
      Serial.println();

      delay(200);
      digitalWrite(buzzerPin, LOW);
    }
  }
}
