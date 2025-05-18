#include <Servo.h>
#include <Motor_Shield.h> // Include motor shield library for motor control functions

// Define xultrasonic sensor pins
#define TRIG A5
#define ECHO A3

int data = A0;           // Sensor input pin

DCMotor Amotor(1);      // Creating instances for Motor 1
DCMotor Bmotor(2);      // Creating instances for Motor 2

long previousMillis;   // Variables to scan for time alterations
long currentMillis;

// Target distance (cm)
const int TARGET_DISTANCE = 30;

// PID Controller Gains
const float Kp = 20; // Proportional Gain
const float Ki = 0.05; // Integral Gain
const float Kd = 1; // Derivative Gain

float previousError = 0;
float integral = 0;

void setup() {
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    Serial.begin(9600);
}

float getDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    
    long duration = pulseIn(ECHO, HIGH);
    float distance = duration * 0.034 / 2;
    return distance;
}

void moveRobot() {
    float distance = getDistance();
    float error = distance - TARGET_DISTANCE;
    integral += error; // Compute integral term
    float derivative = error - previousError; // Compute derivative term
    
    int speed = constrain(Kp * abs(error) + Ki * abs(integral) + Kd * abs(derivative), 0, 255);
    
    if (error > 0) {
        Serial.print("Moving Forward | Distance: ");
        Serial.print(distance);
        Serial.print(" cm | Speed: ");
        Serial.println(speed);
        Amotor.run(FORWARD);
        Bmotor.run(FORWARD);
        Amotor.setSpeed(speed);
        Bmotor.setSpeed(speed);
    } else if (error < -5) { // If the robot overshoots, move backward
        Serial.print("Moving Backward | Distance: ");
        Serial.print(distance);
        Serial.print(" cm | Speed: ");
        Serial.println(speed);
        Amotor.run(BACKWARD);
        Bmotor.run(BACKWARD);
        Amotor.setSpeed(speed);
        Bmotor.setSpeed(speed);
    } else {
        Serial.print("Stopping | Distance: ");
        Serial.println(distance);
        Amotor.setSpeed(0);
        Bmotor.setSpeed(0);
    }
    
    previousError = error; // Update previous error for next iteration
}

void loop() {
    moveRobot();
    delay(100);
}
