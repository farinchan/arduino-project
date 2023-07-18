#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <MFRC522.h>


//WI-FI Config
const char* ssid = "inan";           //Nama Hotspot Wifi
const char* password = "123456780";  // Password Hotspot Wifi
WiFiClient client;

//RFID RC522
#define RST_PIN D3  //rfid
#define SDA_PIN D4  //rfid
MFRC522 mfrc522(SDA_PIN, RST_PIN);

//buzzer di pin 16 (D0)
const int buzzerPin = 16;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(700);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(buzzerPin, OUTPUT);
}

void loop() {

  digitalWrite(buzzerPin, LOW);
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    digitalWrite(buzzerPin, HIGH);

    String card_uid = "";

    for (byte i = 0; i < mfrc522.uid.size; i++) {
      card_uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    card_uid.toUpperCase();
    Serial.println("Card UID: " + card_uid);


    delay(200);
    digitalWrite(buzzerPin, LOW);
  }
}
