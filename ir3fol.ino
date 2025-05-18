#include <Motor_Shield.h>  // Make sure you have the right Motor_Shield library installed

// IR sensor pins (Digital)
#define LEFT_IR    2
#define RIGHT_IR   5
#define CENTER_IR  6

// Create motor shield instance
Motor_Shield shield;

// Define motors: 1 = Left motor, 2 = Right motor
Motor motor1 = shield.getMotor(1);
Motor motor2 = shield.getMotor(2);

void setup() {
  // IR sensor pins
  pinMode(LEFT_IR, INPUT);
  pinMode(CENTER_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);

  // Initialize motor shield
  shield.begin();

  // Stop motors at startup
  stopMotors();
}

void loop() {
  // Read IR sensors (LOW = black line)
  int leftValue   = digitalRead(LEFT_IR);
  int centerValue = digitalRead(CENTER_IR);
  int rightValue  = digitalRead(RIGHT_IR);

  if (centerValue == LOW && leftValue == HIGH && rightValue == HIGH) {
    moveForward();
  } 
  else if (leftValue == LOW) {
    turnLeft();
  } 
  else if (rightValue == LOW) {
    turnRight();
  } 
  else {
    stopMotors();
  }
}

void moveForward() {
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void turnLeft() {
  motor1.setSpeed(100);
  motor2.setSpeed(150);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}

void turnRight() {
  motor1.setSpeed(150);
  motor2.setSpeed(100);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}
