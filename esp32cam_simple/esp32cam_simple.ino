#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
#include <HTTPClient.h>

const char* WIFI_SSID = "inan";
const char* WIFI_PASS = "123456780";

String serverName = "http://192.168.93.24:4000";  //alamat server
String serverPath = "/classify";


HTTPClient http;

static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);

void serveJpg() {
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  http.begin(serverName + serverPath);  // Ganti dengan URL endpoint server Anda
  http.addHeader("Content-Type", "application/json");
  // Data yang akan dikirim dalam format JSON
  String jsonPayload = "{\"key1\":\"value1\",\"key2\":\"value2\"}";

  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  // server.setContentLength(frame->size());
  // server.send(200, "image/jpeg");
  // WiFiClient client = server.client();
  // frame->writeTo(client);
}

void handleJpgLo() {
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}

void handleJpgHi() {
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}

void handleJpgMid() {
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA); 
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  Masukkan 1 = cam-Hi");
  Serial.println("  Masukkan 2 = cam-Mid");
  Serial.println("  Masukkan 3 = cam-lo");
}

void loop() {
  if (Serial.available()) {
    if (Serial.read() == "1") {
      Serial.println("cam-Hi");
      handleJpgHi();
    }
    if (Serial.read() == "2") {
      Serial.println("cam-Mid");
      handleJpgMid();
    }
    if (Serial.read() == "3") {
      Serial.println("cam-Low");
      handleJpgLo();
    }
  }
}