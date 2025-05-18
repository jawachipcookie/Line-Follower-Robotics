#include <Motor_Shield.h>

DCMotor Amotor(1);
DCMotor Bmotor(2);

// Sensor pins
const int leftIR = 2;
const int rightIR = 5;
const int trigPin = A5;
const int echoPin = A3;

// Distance thresholds (cm)
const int obstacleThreshold = 20;
const int minThresholdDist = 10;

void setup() {
  Serial.begin(9600);
  pinMode(leftIR, INPUT);
  pinMode(rightIR, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int leftStatus = digitalRead(leftIR);
  int rightStatus = digitalRead(rightIR);
  float distance = getDistance();

  Serial.print("Left IR: "); Serial.print(leftStatus);
  Serial.print(" | Right IR: "); Serial.print(rightStatus);
  Serial.print(" | Distance: "); Serial.println(distance);

  // Obstacle detected
  if (leftStatus == 1 && rightStatus == 1 && distance > minThresholdDist && distance < obstacleThreshold) {
    Serial.println("Obstacle ahead! Executing maneuver...");
    Stop();
    //delay(50);
    maneuver();
    return;
  }

  // Line following logic
  if (leftStatus == 1 && rightStatus == 1) {
    Forward(100, 100 );
    Serial.println("Moving Forward");
  } else if (leftStatus == 0 && rightStatus == 1) {
    // needs right turn
    Right(130,130); // Tweak if needed
    Serial.println("Right Turn");
    
  } else if (leftStatus == 1 && rightStatus == 0) {
    // needs left turn
    Left(130, 130); // Tweak if neede
    Serial.println("Left Turn");
    
  } else {
    Serial.println("Lost both lines! Stopping.");
    Stop();
  }

}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float dist = (duration * 0.0343) / 2;
  return dist;
}

void maneuver() {
  Left(130, 70);   // soft left
  delay(100);
  Forward(100, 100);
  delay(400);
  Right(90, 130);
  delay(400);
  Forward(100, 100);
  delay(400);
  Right(90, 130);
  delay(250);

  while (digitalRead(leftIR) != 1 && digitalRead(rightIR) != 1) {
    Forward(100, 100);
    Serial.println("Searching for line...");
  }

  Left(80, 80);
  delay(150);
}

// Motor controls
void Forward(unsigned char SpeedLeft, unsigned char SpeedRight) {
  Amotor.run(FORWARD);
  Bmotor.run(FORWARD);
  Amotor.setSpeed(SpeedLeft);
  Bmotor.setSpeed(SpeedRight);
}

void Backward(unsigned char Speed) {
  Amotor.run(BACKWARD);
  Bmotor.run(BACKWARD);
  Amotor.setSpeed(Speed);
  Bmotor.setSpeed(Speed);
}

void Left(unsigned char left, unsigned char right) {
  Amotor.run(BACKWARD);
  Bmotor.run(FORWARD);
  Amotor.setSpeed(right);
  Bmotor.setSpeed(left);
}

void Right(unsigned char left, unsigned char right) {
  Amotor.run(FORWARD);
  Bmotor.run(BACKWARD);
  Amotor.setSpeed(right);
  Bmotor.setSpeed(left);
}

void Stop() {
  Amotor.setSpeed(0);
  Bmotor.setSpeed(0);
  delay(50);
}
