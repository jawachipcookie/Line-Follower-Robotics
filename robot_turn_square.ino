#include <Motor_Shield.h> //Include motor shield lib into arduino lib for motor related functions.

#define Duration 200    //Duration for exiting the loop

int data = A0;           // Sensor Input pin 

DCMotor Amotor(1);      // Creating instances for Motor 1
DCMotor Bmotor(2);      // Creating instances for Motor 2

const int forwardTime = 1000; // Time to move forward in milliseconds
const int turnTime = 650;     // Time to make a 90-degree turn in milliseconds


void setup() 
{
  Serial.begin(9600);
  digitalWrite(data,HIGH);
  pinMode(data, INPUT);   //Configure sensor pin as input
 
}

void loop() {
  // Repeat the following steps 4 times to make a square
  for (int i = 0; i < 4; i++) {
    // Step 1: Move Forward
    moveForward(150);
    delay(forwardTime);
    
    // Step 2: Stop
    stopMotors();
    delay(500); // Small delay to ensure the robot stops

    // Step 3: Turn Right
    turnRight();
    delay(turnTime);

    // Step 4: Stop
    stopMotors();
    delay(500); // Small delay to ensure the robot stops after turning
  }

  // Stop after completing the square
  stopMotors();
  //while (true); // Stop the program here
}


void moveForward(unsigned char Speed)                       //Forward Function
{
            Amotor.run(FORWARD);
            Bmotor.run(FORWARD);
            Amotor.setSpeed(Speed);                            
            Bmotor.setSpeed(Speed);
           
}


void turnRight(void)                                          //Turn Right Function  
{
           Amotor.run(FORWARD);
           Bmotor.run(BACKWARD); 
           Amotor.setSpeed(200);                          
           Bmotor.setSpeed(50);
           
}

 void stopMotors(void)                                          //All motors stop to stop the robot
        {
            Amotor.setSpeed(0);
            Bmotor.setSpeed(0);
            delay(10);
        }

