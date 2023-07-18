

/* define L298N or L293D motor control pins */
#define ENA A1  // Fungsi motor sisi Kiri (D5)
#define ENB A2  // Fungsi motor sisi Kanan (D6)
#define IN_1 7  // Fungsi  L298N in1 motor 1 kiri (D8)
#define IN_2 6  // Fungsi L298N in2 motor 2 kiri (D7)
#define IN_3 5  // Fungsi L298N in3 motor 1 kanan (D4)
#define IN_4 4  // Fungsi L298N in4 motor 2 kanan (D3)

#define LB D0  // Fungsi Lampu belakang (D0)
#define LD D1  // Fungsi Lampu depan (D1)

char value;

int kecepatan = 800;  // Range kecepatan 400 - 1023.

void setup() {
  /* initialize motor control pins as output */
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //  maju();
  //  delay(1000);
  //  mundur();
  //  delay(1000);
  //  belokKanan();
  //  delay(1000);
  //  belokKiri();
  //  delay(1000);
  Bluetoothcontrol();
}

void Bluetoothcontrol() {
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
  }
  if (value == 'F') {
    maju();
  } else if (value == 'B') {
    mundur();
  } else if (value == 'L') {
    belokKanan();
  } else if (value == 'R') {
    belokKiri();
  } else if (value == 'S') {
    berhenti();
  }
}

//Fungsi untuk mengontrol pergerakan robot
void belokKanan() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, kecepatan);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, kecepatan);
}

void belokKiri() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, kecepatan);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, kecepatan);
}

void maju() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, kecepatan);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, kecepatan);
}

void mundur() {

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
}
