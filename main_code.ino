#include <Motor_Shield.h>
//this code does not do turning properly 
// Pin definitions
#define trigPin A5
#define echoPin A3
#define irLeft 2
#define irRight 5

// Configuration variables (easily modifiable)
const int FORWARD_SPEED = 180;  // Default forward speed (can be changed)
const int OBSTACLE_THRESHOLD = 3;  // Obstacle distance in cm
const int TURN_DELAY = 10;  // Delay for turns (ms)

// Timing variables
unsigned long lastSpeedPrintTime = 0;
const unsigned long SPEED_PRINT_INTERVAL = 3000;  // 3 seconds
unsigned long lastDistancePrintTime = 0;
const unsigned long DISTANCE_PRINT_INTERVAL = 1500;  // 1.5 seconds

// Motor instances
DCMotor Amotor(1);
DCMotor Bmotor(2);

// Function declarations
long getDistance();
void Forward(unsigned char SpeedLeft, unsigned char SpeedRight);
void Backward(unsigned char Speed);
void Left();
void Right();
void Stop();
void take_diversion();

void setup() {
  Serial.begin(9600);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Direct port manipulation for faster IR sensor reading
  bool irLeftStatus = PIND & (1 << irLeft);
  bool irRightStatus = PIND & (1 << irRight);
  
  // Get distance from ultrasonic sensor
  long distance = getDistance();
  
  // Print distance every 1.5 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - lastDistancePrintTime >= DISTANCE_PRINT_INTERVAL) {
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");
    lastDistancePrintTime = currentMillis;
  }
  
  // Control logic based on sensor readings
  if (!irLeftStatus && irRightStatus) {
    Stop();
    Left();
  } 
  else if (!irRightStatus && irLeftStatus) {
    Stop();
    Right();
  }
  else if (irLeftStatus && irRightStatus) {
    if (distance < OBSTACLE_THRESHOLD) {
      Serial.println("Obstacle Detected! Taking Diversion...");
      Stop();
      delay(200);
      take_diversion();
    }
    else {
      Forward(FORWARD_SPEED, FORWARD_SPEED);
    }
  }
  else {
    Stop();
  }
  
  // Small delay for stability
  //delay(10);
}

// Motor control functions:
void Forward(unsigned char SpeedLeft, unsigned char SpeedRight) {
  Amotor.run(FORWARD);
  Bmotor.run(FORWARD);
  Amotor.setSpeed(SpeedLeft);
  Bmotor.setSpeed(SpeedRight);
  
  // Print speed status every 3 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - lastSpeedPrintTime >= SPEED_PRINT_INTERVAL) {
    Serial.print("Motor Speeds -> Left: ");
    Serial.print(SpeedLeft);
    Serial.print(" | Right: ");
    Serial.println(SpeedRight);
    lastSpeedPrintTime = currentMillis;
  }
}

void Backward(unsigned char Speed) {
  Amotor.run(BACKWARD);
  Bmotor.run(BACKWARD);
  Amotor.setSpeed(Speed);
  Bmotor.setSpeed(Speed);
}

void Left() {
  Amotor.run(FORWARD);
  Bmotor.run(BACKWARD);
  Amotor.setSpeed(150);
  Bmotor.setSpeed(200);
  delay(TURN_DELAY);
}

void Right() {
  Amotor.run(BACKWARD);
  Bmotor.run(FORWARD);
  Amotor.setSpeed(200);
  Bmotor.setSpeed(150);
  delay(TURN_DELAY);
}

void Stop() {
  Amotor.setSpeed(0);
  Bmotor.setSpeed(0);
  delay(50);
}

void take_diversion() {
  Right();
  delay(250);
  Forward(180, 180);
  delay(800);
  Left();
  delay(450);
  Forward(180, 180);
  delay(1120);
  Left();
  delay(400);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  long distance = (duration * 0.034) / 2;
  return distance;
}