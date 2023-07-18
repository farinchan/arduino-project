#include <ESP8266WiFi.h>

const char* ssid = "ORBIT-Robocar";   // Nama SSID AP/Hotspot
const char* password = "";  // Password Wifi

// Mengatur IP Address ----------------------------------------------------
IPAddress IP(192, 168, 1, 101);
IPAddress GATEWAY(192, 168, 1, 1);
IPAddress NETMASK(255, 255, 255, 0);
IPAddress DNS(192, 168, 1, 1);

/* define port */
WiFiClient client;
WiFiServer server(80);


/* data received from application */
String data = "";

/* define L298N or L293D motor control pins */
#define ENA 14   // Fungsi motor sisi Kiri (D5)
#define ENB 12   // Fungsi motor sisi Kanan (D6)
#define IN_1 15  // Fungsi  L298N in1 motor 1 kiri (D8)
#define IN_2 13  // Fungsi L298N in2 motor 2 kiri (D7)
#define IN_3 2   // Fungsi L298N in3 motor 1 kanan (D4)
#define IN_4 0   // Fungsi L298N in4 motor 2 kanan (D3)
#define LB D0    // Fungsi Lampu belakang (D0)
#define LD D1    // Fungsi Lampu depan (D1)

int kecepatan = 800;  // Range kecepatan 400 - 1023.

void setup() {
  Serial.begin(115200);

  /* initialize motor control pins as output */
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(LD, OUTPUT);

  /* start server communication */
  server.begin();
  ConnectWifi();
}
void loop() {
  /* If the server available, run the "checkClient" function */
  client = server.available();
  if (!client) return;
  data = checkClient();

  /* If the incoming data is "forward", run the "MotorForward" function */
  if (data == "forward") maju();
  else if (data == "backward") mundur();
  else if (data == "right") belokKanan();
  else if (data == "left") belokKiri();

  /* If the incoming data is "stop", run the "MotorStop" function */
  else if (data == "stop") berhenti();
}

//Fungsi untuk mengontrol pergerakan robot
void maju() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, kecepatan);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, kecepatan);
}

void mundur() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, kecepatan);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, kecepatan);
}

void belokKanan() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, kecepatan);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, kecepatan);
}

void belokKiri() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, kecepatan);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, kecepatan);
}

void berhenti() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, kecepatan);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, kecepatan);
  digitalWrite(LB, LOW);
}

String checkClient(void) {
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}

void ConnectWifi() {
  // Connect to WiFi network ------------------------------------------------
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Mengatur WiFi ----------------------------------------------------------
  WiFi.mode(WIFI_STA);         // Mode Station
  WiFi.begin(ssid, password);  // Mencocokan SSID dan Password
  WiFi.config(IP, GATEWAY, NETMASK, DNS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print status Connect ---------------------------------------------------
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}