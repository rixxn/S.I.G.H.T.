#include <Servo.h>

#define TRIG_PIN 2
#define ECHO_PIN 3
#define LEFT_VIBRATION_PIN 5
#define RIGHT_VIBRATION_PIN 6
#define SERVO_PIN 9

Servo servo;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LEFT_VIBRATION_PIN, OUTPUT);
  pinMode(RIGHT_VIBRATION_PIN, OUTPUT);
  
  servo.attach(SERVO_PIN);
}

void loop() {
  int distance = getDistance();
  Serial.print("Distance: ");
  Serial.println(distance);
  
  for (int angle = 0; angle <= 180; angle += 2) {
    servo.write(angle); // Move servo to the right
    delay(20); // Delay for servo to reach position
    checkAndActivateVibration(distance);
  }

  for (int angle = 180; angle >= 0; angle -= 2) {
    servo.write(angle); 
    delay(20); 
    checkAndActivateVibration(distance);
  }
}

void checkAndActivateVibration(int distance) {
  if (distance <= 125 && distance > 0) { // Object detected within 2.5 meters
    int angle = servo.read();
    if (angle >= 60 && angle <= 120) {
      // Object detected within center range
      activateVibrations(255, 255); // Both vibration motors on
    } else if (angle >= 60) {
      // Object detected on the right
      activateVibrations(0, 255); // Right vibration motor on
    } else if (angle <= 120) {
      // Object detected on the left
      activateVibrations(255, 0); // Left vibration motor on
    }
  } else {
    // No object detected within range, turn off vibrations
    activateVibrations(LOW, LOW);
  }
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2; // Speed of sound = 0.034 cm/microsecond
  
  return distance;
}

void activateVibrations(int leftState, int rightState) {
  digitalWrite(LEFT_VIBRATION_PIN, leftState);
  digitalWrite(RIGHT_VIBRATION_PIN, rightState);
}

