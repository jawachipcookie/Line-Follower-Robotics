#include <Motor_Shield.h>

// Define IR sensor pins
const int irLeft   = 2;  // Left IR sensor
const int irRight  = 5;  // Right IR sensor
const int irCenter = 6;  // Center IR sensor

// Initialize motors
DCMotor leftMotor(1);   // Left motor
DCMotor rightMotor(2);  // Right motor

void setup() {
  Serial.begin(9600);
  
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  pinMode(irCenter, INPUT);
}

void loop() {
  int leftStatus   = digitalRead(irLeft);
  int rightStatus  = digitalRead(irRight);
  int centerStatus = digitalRead(irCenter);

  Serial.print("L:");
  Serial.print(leftStatus);
  Serial.print(" C:");
  Serial.print(centerStatus);
  Serial.print(" R:");
  Serial.println(rightStatus);

  // Sensor logic: LOW = Black Line Detected
  if (centerStatus == LOW && leftStatus == HIGH && rightStatus == HIGH) {
    Forward(150, 150);
    Serial.println("Straight");
  }
  else if (leftStatus == LOW) {
    TurnLeft();
    Serial.println("Turning Left");
  }
  else if (rightStatus == LOW) {
    TurnRight();
    Serial.println("Turning Right");
  }
  else {
    Stop();
    Serial.println("Stop");
  }

  delay(30);
}

// Movement functions
void Forward(uint8_t leftSpeed, uint8_t rightSpeed) {
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
  leftMotor.setSpeed(leftSpeed);
  rightMotor.setSpeed(rightSpeed);
}

void TurnLeft() {
  leftMotor.run(BACKWARD);
  rightMotor.run(FORWARD);
  leftMotor.setSpeed(100);
  rightMotor.setSpeed(180);
}

void TurnRight() {
  leftMotor.run(FORWARD);
  rightMotor.run(BACKWARD);
  leftMotor.setSpeed(180);
  rightMotor.setSpeed(100);
}

void Stop() {
  leftMotor.setSpeed(0);
  rightMotor.setSpeed(0);
}
