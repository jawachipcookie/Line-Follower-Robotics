#include <Motor_Shield.h> // Include motor shield library for motor-related functions

#define IR_LEFT 2        // Left IR sensor input pin
#define IR_RIGHT 5       // Right IR sensor input pin
#define TRIG_PIN A3     // Trigger pin for ultrasonic sensor
#define ECHO_PIN A2      // Echo pin for ultrasonic sensor


DCMotor Rmotor(1);    // Creating instance for Motor 1 (RIGHT MOTOR)
DCMotor Lmotor(2);    // Creating instance for Motor 2 (LEFT MOTOR)

void setup() {
  pinMode(IR_LEFT, INPUT);   // Configure left IR sensor pin as input
  pinMode(IR_RIGHT, INPUT);  // Configure right IR sensor pin as input
  pinMode(TRIG_PIN, OUTPUT); // Set the trigger pin for ultrasonic sensor as output
  pinMode(ECHO_PIN, INPUT);  // Set the echo pin for ultrasonic sensor as input
}


long obstacledistance() {
  // Send a pulse to trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the pulse duration on the echo pin
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance based on pulse duration (in centimeters)
  return (duration * 0.034) / 2;
}

void turnLeft() {
  Rmotor.run(FORWARD);
  Lmotor.run(BACKWARD);
  Rmotor.setSpeed(80);                          
  Lmotor.setSpeed(150);

}

void turnRight() {
  Rmotor.run(BACKWARD);
  Lmotor.run(FORWARD);
  Rmotor.setSpeed(150);                          
  Lmotor.setSpeed(80);

}

void moveForward() {
  Rmotor.run(FORWARD);
  Lmotor.run(FORWARD);
  Rmotor.setSpeed(75);                            
  Lmotor.setSpeed(75);
}

void stopMotors() {
    Rmotor.run(RELEASE);
    Lmotor.run(RELEASE);
}

void avoidObstacle() {
    Serial.println(" Obstacle Detected → Avoiding...");

    stopMotors();
    delay(200);

    // 1. Turn right 
    turnRight();
    delay(1000);
    stopMotors();
    delay(100);

    // 2. Move forward to clear side
    moveForward();
    delay(2200);
    stopMotors();
    delay(100);

    // 3. Turn left ~30°
    turnLeft();
    delay(1000);
    stopMotors();
    delay(100);

    // 4. Move forward alongside obstacle
    moveForward();
    delay(2000);  // Critical forward movement next to obstacle
    stopMotors();
    delay(100);

    // 5. Turn left to return to line
    turnLeft();
    delay(800);
    stopMotors();
    delay(100);
}

//MAIN CODE

void loop() {
 long distance = obstacledistance();

  int LIR = digitalRead(IR_LEFT);
  int RIR = digitalRead(IR_RIGHT);

  //LOGIC

  if(LIR == 0 && RIR == 0){
    if(distance < 20){
      avoidObstacle();
    }
    else{
      moveForward();
    }
  }
  else if(LIR==1 && RIR == 0){
    turnRight();
  }
  else if (LIR == 0 && RIR == 1){
    turnLeft();
  }
  else if ( LIR == 1 && RIR == 1){
    stopMotors();
  }

}