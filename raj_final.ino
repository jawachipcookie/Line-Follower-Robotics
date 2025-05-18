#include <Motor_Shield.h>

DCMotor Amotor(1);
DCMotor Bmotor(2);

  const int leftIR = 5;      // Left IR sensor
  const int rightIR = 2;     // Right IR sensor
  const int trigPin = A5;    // Ultrasonic Trig pin
  const int echoPin = A3;    // Ultrasonic Echo pin

// Threshold for obstacle detection
const int obstacleThreshold = 13; // in centimeters



void setup() {
  Serial.begin(9600);

  pinMode(2, INPUT);
  pinMode(5, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int rightIR = digitalRead(5);
  int leftIR = digitalRead(2);
  
  long distance = getDistance();
  
  if(leftIR==1 && rightIR==1) {
    if (distance < obstacleThreshold) {
    // Stop the motors
    Stop();
    delay(200); // Pause before deciding next move
    take_diversion(); // Move back and turn
    //delay(200);
    //Stop();
    //delay(500);
   }
   else{
    Forward();
    } 
  }
  else if(leftIR==0 && rightIR==1) {
    turnLeft();
  } 
  else if(leftIR==1 && rightIR==0) {
    turnRight();
  } 
  else {
    Stop();
  }
}


void Forward() {
  Amotor.run(FORWARD);
  Bmotor.run(FORWARD);
  Amotor.setSpeed(180);
  Bmotor.setSpeed(180);
}

void turnRight(void){
  Amotor.run(BACKWARD); 
  Bmotor.run(FORWARD);
  Amotor.setSpeed(95);
  Bmotor.setSpeed(95);
  //Turn Left Function
}

void turnLeft(void){
  Amotor.run(FORWARD);
  Bmotor.run (BACKWARD);
  Amotor.setSpeed(70);
  Bmotor.setSpeed(70);
//Turn Right Function
}

void Stop(void){
  Amotor.setSpeed(0);
  Bmotor.setSpeed(0);
  delay(10);
}

void take_diversion() {
  turnRight();
  delay(250);
  Forward();
  delay(800);
  turnLeft();
  delay(450);
  Forward();
  delay(1120);
  turnLeft();
  delay(400);

 

}

long getDistance() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Set the trigPin HIGH
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  // Set the trigPin LOW
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin
  long duration = pulseIn(echoPin, HIGH);
  // Calculate distance in cm
  long distance = (duration * 0.034) / 2;
  return distance;
}