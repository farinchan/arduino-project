int whiteLamp = 3;
int redLamp = 4;
int buzzer = 2;

void setup() {
  Serial.begin(9600);
  pinMode(whiteLamp, OUTPUT);
  pinMode(redLamp, OUTPUT);
  pinMode(buzzer, OUTPUT);
  

}

void loop() {

  if (Serial.available() > 0) {
     char data = Serial.read(); // Baca data yang diterima dari koneksi serial
    if (data == 'F') {
      digitalWrite(whiteLamp, LOW);
      digitalWrite(redLamp, HIGH);
      digitalWrite(buzzer, HIGH);
    } else if (data == 'H')  {
      digitalWrite(redLamp, LOW);
      digitalWrite(buzzer, LOW);
      digitalWrite(whiteLamp, HIGH);
    }

  
  }
 

}
