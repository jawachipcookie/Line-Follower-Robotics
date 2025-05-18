#include <Servo.h>

#include <Motor_Shield.h> 
#define trigPin A5
#define echoPin A3
// had to use define here because 
//can't store a char as int LMao 


int irLeft = 2;    //IR SENS
int irRight = 5;
//int trig = A5;
//int echo = A3;  
int x = 25;


DCMotor Amotor(1);  // Motor A - right ? 
DCMotor Bmotor(2);  // Motor B - left  ? 

void setup() 
{
  Serial.begin(9600);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
}

void loop()
{     
  int irLeftStatus = digitalRead(irLeft);
  int irRightStatus = digitalRead(irRight);
  //implementing direct port manipulation here 
  // to make this happen faster 

  Serial.print("Left Sensor = ");
  Serial.println(irLeftStatus);
  Serial.print("Right Sensor = ");
  Serial.println(irRightStatus);

 //this logic works in the Vishesh Jawa robot 
 // 1 || 0 - Left (the robot should at this )
   
 // 1 || 1 - Forward
 // 0 || 1 - Right (no delay because should be instant)
 // 0 || 0 - stop with a delay 

// so the sensors are placed really far apart 
// we depend on only one sensor to follow the line 
// read it as fast as possible and implement the turn 
//sharpnesss of turn can be changed basically 
//
  if(irLeftStatus == 1 && irRightStatus == 0) {
    Stop();
    Left();
    Serial.println("Turning Left");
  }
  
  else if (irRightStatus == 1 && irLeftStatus == 0) {
    Stop();
    Right();
    Serial.println("Turning Right");
  }
  // If both sensors = 1, move forward
  else if (irLeftStatus == 1 && irRightStatus == 1) {
    Forward(135, 135);
    Serial.println("Moving Forward");
  }
  // If both sensors = 0, stop
  else {
    Stop();
    Serial.println("Stopping");
  }

  //delay(10); // Short delay for stability
}

// Motor control functions:
void Forward(unsigned char SpeedLeft, unsigned char SpeedRight) 
{
  Amotor.run(FORWARD);
  Bmotor.run(FORWARD);
  Amotor.setSpeed(SpeedLeft);
  Bmotor.setSpeed(SpeedRight);
 
}

void Backward(unsigned char Speed)
{
  Amotor.run(BACKWARD);
  Bmotor.run(BACKWARD);
  Amotor.setSpeed(Speed);
  Bmotor.setSpeed(Speed);
}

void Left(void)                                           //Turn Left Function   
{
  
           Amotor.run(FORWARD);
           Bmotor.run(BACKWARD);
           Amotor.setSpeed(200);                          
           Bmotor.setSpeed(255);
          delay(x);
}
void Right(void)                                          //Turn Right Function  
{
          
           Amotor.run(BACKWARD);
           Bmotor.run(FORWARD); 
           Amotor.setSpeed(255);                          
           Bmotor.setSpeed(200);
           delay(x);
           
}
  void Stop(void)                                          //All motors stop to stop the robot
        {
            Amotor.setSpeed(0);
            Bmotor.setSpeed(0);
            delay(50);
        }
