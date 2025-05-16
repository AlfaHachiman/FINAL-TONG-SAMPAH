#include <Servo.h>  // Library servo

#define TRIG_PIN 9      // Arduino ke sensor
#define ECHO_PIN 10     // Sensor ke Arduino
#define LED_PIN 13      // Lampu
#define BUZZER_PIN 8    // Buzzer
#define SERVO_PIN 7     // Servo

bool objectDetected = false; // Status objek terdeteksi
bool buzzerDone = false;     // Flag untuk buzzer sudah bunyi 2 kali

Servo myServo;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);

  myServo.attach(SERVO_PIN);
  myServo.write(0);  // Posisi servo awal
}

void loop() {
  int distance = 0;

  if (Serial.available()) {
    distance = Serial.parseInt();
    Serial.read();
  } else {
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

    if (!buzzerDone) {
      myServo.write(90);  // Servo langsung gerak ke 90°

      for (int i = 0; i < 2; i++) {   // Buzzer bunyi 2 kali
        digitalWrite(BUZZER_PIN, HIGH);
        delay(200);
        digitalWrite(BUZZER_PIN, LOW);
        delay(200);
      }

      buzzerDone = true;
    } else {
      myServo.write(90);  // Servo tetap di 90°
    }

    objectDetected = true;

  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    if (objectDetected) {
      myServo.write(0);   // Servo balik ke posisi awal
      objectDetected = false;
      buzzerDone = false; // Reset supaya buzzer bisa bunyi lagi
    }
  }

  delay(100);
}
