#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define RST_PIN D3
#define SDA_PIN D4

MFRC522 mfrc522(SDA_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  lcd.begin();      // initialize the lcd 
  lcd.backlight();
  Serial.println("Put your card to the reader...");
  lcd.setCursor(0,0);
  lcd.print("Silahkan");
  lcd.setCursor(0,1);
  lcd.print("Tap Kartumu");

}

void loop() {
  if(!mfrc522.PICC_IsNewCardPresent()){
    return;
  }

  if(!mfrc522.PICC_ReadCardSerial()){
    return;
  }

  Serial.print("UID tag :");
  String content = "";
  byte letter;

  for(byte i = 0; i < mfrc522.uid.size; i++){

    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  lcd.setCursor(0,0);
  lcd.print("UID:" +content);
  lcd.setCursor(0,1);
  lcd.print("Pesan : Berhasil");
  
  Serial.print(content);
  Serial.println();
  Serial.print("Pesan : Berhasil");
  Serial.println();
  Serial.println();

  delay(2000);
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Silahkan");
  lcd.setCursor(0,1);
  lcd.print("Tap Kartumu");



}
