#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <MFRC522.h>
#include <string>

//WI-FI Config
const char* ssid = "inan";          //Nama Hotspot Wifi
const char* password = "123456780";  // Password Hotspot Wifi
WiFiClient client;

//Database Config
IPAddress server_addr(54, 255, 24, 46);   // IP address dari server MySQL
char db_user[] = "e_parking";             // nama pengguna MySQL
char db_password[] = "cXzswpk5yLyy2t5S";  // kata sandi MySQL
char db_name[] = "e_parking";             // nama database MySQL
MySQL_Connection conn((Client*)&client);

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
  Serial.println("Connecting to MySQL server...");
  if (conn.connect(server_addr, 3306, db_user, db_password, db_name)) {
    Serial.println("Connected to MySQL server");
  } else {
    Serial.println("Connection to MySQL server failed");
  }

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  delay(1000);

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

    // CreateData(char_array);
    readData();

    digitalWrite(buzzerPin, LOW);
  }
}

void CreateData(String uid) {
  String query = "INSERT INTO `temp_rfid` (`rfid`) VALUES ('" + uid + "')";
  MySQL_Cursor* cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(query.c_str());
  delete cur_mem;
}

void readData() {
  long head_count = 0;

  String query = "SELECT * FROM admin";
  MySQL_Cursor* cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(query.c_str());


  column_names* cols = cur_mem->get_columns();
  // for (int f = 0; f < cols->num_fields; f++) {
  //   // Serial.print(cols->fields[f]->name);
  //   if (f < cols->num_fields-1) {
  //     // Serial.print(',');
  //   }
  // }

  row_values* row = NULL;
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      for (int f = 0; f < cols->num_fields; f++) {
        Serial.print(row->values[f]);
        if (f < cols->num_fields - 1) {
          Serial.print(',');
        }
      }
      // free_row_buffer();
      Serial.println();
    }
  } while (row != NULL);
  // free_columns_buffer();
  // Deleting the cursor also frees up memory used
  delete cur_mem;

  // Show the result
  Serial.print("  NYC pop = ");
  Serial.println(head_count);
}
