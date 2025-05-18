#include <Servo.h>
#include <Motor_Shield.h>

// IR Sensor Pins
int irLeft = 5;     
int irRight = 2;    

// Motor Setup
DCMotor Amotor(1);  
DCMotor Bmotor(2);  

// PID Constants (TUNE THESE)
float Kp = 10;   // Proportional Gain (TUNE THIS)
float Ki = 1;    // Integral Gain (Start with 0)
float Kd = 20.0;   // Derivative Gain (TUNE THIS)

// PID Variables
float error = 0, previousError = 0, integral = 0;

void setup() {
    Serial.begin(9600);
    pinMode(irLeft, INPUT);
    pinMode(irRight, INPUT);
}

void loop() {    
    int irLeftStatus = digitalRead(irLeft);
    int irRightStatus = digitalRead(irRight);

    // Compute Error
    error = (irLeftStatus == 0 ? -1 : 0) + (irRightStatus == 0 ? 1 : 0);

    // PID Calculations
    integral += error;
    float derivative = error - previousError;
    float correction = Kp * error + Ki * integral + Kd * derivative;
    previousError = error;

    // Base Speed
    int baseSpeed = 100;
    int leftSpeed = baseSpeed - correction;
    int rightSpeed = baseSpeed + correction;

    // Ensure speed values stay within limits
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    // Move the robot accordingly
    if (irLeftStatus == 1 && irRightStatus == 1) {
        Forward(leftSpeed, rightSpeed);
        Serial.println("Moving Forward");
    } 
    else if (irLeftStatus == 0 && irRightStatus == 1) {
        Forward(leftSpeed, rightSpeed);
        Serial.println("Turning Left");
    } 
    else if (irRightStatus == 0 && irLeftStatus == 1) {
        Forward(leftSpeed, rightSpeed);
        Serial.println("Turning Right");
    } 
    else {  
        // If both sensors detect black, prevent getting stuck
        Backward(80);
        delay(200);
        Stop();
        Left();
        delay(300);
        Serial.println("Handling Stuck Condition");
    }

    delay(10); // Small delay for stability
}

// Motor control functions:
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

void Left(void) {
    Amotor.run(FORWARD);
    Bmotor.run(BACKWARD);
    Amotor.setSpeed(150);                          
    Bmotor.setSpeed(200);
}

void Right(void) {
    Amotor.run(BACKWARD);
    Bmotor.run(FORWARD); 
    Amotor.setSpeed(200);                          
    Bmotor.setSpeed(150);
}

void Stop(void) {
    Amotor.setSpeed(0);
    Bmotor.setSpeed(0);
    delay(10);
}
