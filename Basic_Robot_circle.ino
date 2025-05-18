#include <Motor_Shield.h> //Include motor shield lib into arduino lib for motor related functions.

#define Duration 200    //Duration for exiting the loop

int data = A0;           // Sensor Input pin 

DCMotor Amotor(1);      // Creating instances for Motor 1
DCMotor Bmotor(2);      // Creating instances for Motor 2

long previousMillis;   // variables to scan for time alterations
long currentMillis;

const float wheelBase = 0.15;  // Distance between wheels in meters (example value)
const float radius = 0.1;     // Desired circle radius in meters
const int baseSpeed = 40;    // Base speed for the inner wheel (adjust as needed)

void setup() 
{
  Serial.begin(9600);
  digitalWrite(data,HIGH);
  pinMode(data, INPUT);   //Configure sensor pin as input

  float speedRatio = (radius + wheelBase / 2) / (radius - wheelBase / 2);
  int outerSpeed = baseSpeed * speedRatio;
  
  // Constrain speed to PWM limits
  outerSpeed = constrain(outerSpeed, 0, 160);

  Amotor.run(FORWARD);
  Bmotor.run(FORWARD);
  Amotor.setSpeed(outerSpeed);                            
  Bmotor.setSpeed(baseSpeed);
           
 
}

void loop()
{
  
}