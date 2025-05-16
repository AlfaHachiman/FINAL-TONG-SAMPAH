#include <Servo.h>  

#define TRIG_PIN 9      
#define ECHO_PIN 10     
#define LED_PIN 13      
#define BUZZER_PIN 8    
#define SERVO_PIN 7     

bool objectDetected = false; 

Servo myServo;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);

  myServo.attach(SERVO_PIN);
  myServo.write(0);  
}

void loop() {
  int distance = 0;

  // Cek apakah ada input jarak dari Serial Monitor
  if (Serial.available()) {
    distance = Serial.parseInt();
    Serial.read(); // buang karakter akhir
  } else {
    // Baca jarak dari sensor ultrasonik
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;
  }

  Serial.print("Jarak terdeteksi: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 15) {
    digitalWrite(LED_PIN, HIGH);

    // Jika objek baru saja terdeteksi
    if (!objectDetected) {
      myServo.write(180);  // putar servo

      // Bunyikan buzzer 2x
      for (int i = 0; i < 2; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        delay(100);
      }

      objectDetected = true;  // tandai bahwa objek sedang terdeteksi
    }

  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    // Jika objek sebelumnya terdeteksi dan sekarang sudah tidak ada
    if (objectDetected) {
      myServo.write(0);  // kembalikan servo ke posisi awal
      objectDetected = false;
    }
  }

  delay(100);
}
