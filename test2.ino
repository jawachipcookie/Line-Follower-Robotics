#include <Motor_Shield.h>

// Pin definitions
#define trigPin A5
#define echoPin A3
#define irLeft 2
#define irRight 5

// Configurable constants
const int SPEED = 120;    // Main forward speed - easy to change
const int obstacleThreshold = 1;  // Only react if obstacle < threshold cm
const int turnDelay = 250;         // Time spent during turning (ms)
const int postDetectionDelay = 15; // After detection, go straight for this time (ms)

// Timing constants
unsigned long lastSpeedPrint = 0;
const unsigned long speedPrintInterval = 5000; // Print motor speeds every 5 seconds
unsigned long lastDistancePrint = 0;
const unsigned long distancePrintInterval = 2000; // Print distance every 2 seconds

// State tracking
int lastLeftStatus = 0;
int lastRightStatus = 0;
String currentState = ""; // "FORWARD", "LEFT", "RIGHT", "STOP"

// Current speed tracking
int currentSpeedA = 0;
int currentSpeedB = 0;

// Motor instances
DCMotor Amotor(1);
DCMotor Bmotor(2);

// Function declarations
bool checkLineDetected();

void setup()
{
  Serial.begin(9600);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Start moving immediately
  Forward(SPEED);
  currentState = "FORWARD";
  Serial.println("STARTING: FORWARD");
}

void loop()
{
  // Direct port manipulation for faster IR sensor reading
  int irLeftStatus = (PIND & (1 << irLeft)) ? 1 : 0;  // Reading pin 2 directly
  int irRightStatus = (PIND & (1 << irRight)) ? 1 : 0; // Reading pin 5 directly
  
  // Get distance from ultrasonic sensor
  long distance = getDistance();
  
  unsigned long currentMillis = millis();
  
  // Print distance every 2 seconds
  if (currentMillis - lastDistancePrint >= distancePrintInterval) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    lastDistancePrint = currentMillis;
  }

  // Check for sensor changes
  bool sensorChanged = (irLeftStatus != lastLeftStatus) || (irRightStatus != lastRightStatus);
  
  if (sensorChanged) {
    // Update stored status
    lastLeftStatus = irLeftStatus;
    lastRightStatus = irRightStatus;
    
    Serial.print("IR Change -> Left: ");
    Serial.print(irLeftStatus);
    Serial.print(" | Right: ");
    Serial.println(irRightStatus);

    // Decision-making based on sensor readings
    if (irLeftStatus == 1 && irRightStatus == 0) {
      Left();
      currentState = "LEFT";
    }
    else if (irRightStatus == 1 && irLeftStatus == 0) {
      Right();
      currentState = "RIGHT";
    }
    else if (irLeftStatus == 1 && irRightStatus == 1) {
      if (distance < obstacleThreshold && distance > 0) {
        Serial.println("Diverting...");
        Stop();
        take_diversion();
        currentState = "DIVERTING";
      } 
      else {
        Forward(SPEED);
        currentState = "FORWARD";
      }
    }
    else {
      Stop();
      currentState = "STOP";
    }
  } else {
    // Check for obstacles even if no sensor change
    if (irLeftStatus == 1 && irRightStatus == 1 && 
        distance < obstacleThreshold && distance > 0) {
      Serial.println("Obstacle detected without sensor change!");
      Stop();
      take_diversion();
      currentState = "DIVERTING";
    }
  }

  // Print status every 5 seconds
  if (currentMillis - lastSpeedPrint >= speedPrintInterval) {
    printStatus();
    lastSpeedPrint = currentMillis;
  }
}

// --- Motor Control Functions ---

void Forward(unsigned char Speed)
{
  Amotor.run(FORWARD);
  Bmotor.run(FORWARD);
  Amotor.setSpeed(Speed);
  Bmotor.setSpeed(Speed);
  currentSpeedA = Speed;
  currentSpeedB = Speed;
}

void Backward(unsigned char Speed)
{
  Amotor.run(BACKWARD);
  Bmotor.run(BACKWARD);
  Amotor.setSpeed(Speed);
  Bmotor.setSpeed(Speed);
  currentSpeedA = Speed;
  currentSpeedB = Speed;
}

void Left(void)
{
  Amotor.run(FORWARD);
  Bmotor.run(BACKWARD);
  Amotor.setSpeed(180);
  Bmotor.setSpeed(200);
  currentSpeedA = 180;
  currentSpeedB = 200;
  delay(turnDelay);
  
}

void Right(void)
{
  Amotor.run(BACKWARD);
  Bmotor.run(FORWARD);
  Amotor.setSpeed(200);
  Bmotor.setSpeed(180);
  currentSpeedA = 200;
  currentSpeedB = 180;
  delay(turnDelay);
 
}

void Stop(void)
{
  Amotor.setSpeed(0);
  Bmotor.setSpeed(0);
  currentSpeedA = 0;
  currentSpeedB = 0;
  delay(50);
}

// Check if line is detected
bool checkLineDetected() {
  // Read IR sensors directly
  int leftStatus = (PIND & (1 << irLeft)) ? 1 : 0;
  int rightStatus = (PIND & (1 << irRight)) ? 1 : 0;
  
  // Return true if either sensor detects a line
  return (leftStatus == 1 || rightStatus == 1);
}

void take_diversion()
{
  // Backup first
  Backward(SPEED);
  delay(100);
  
  // First turn
  Right();
  delay(100);
  
  // Move and check for line
  for (int i = 0; i < 10; i++) {
    Forward(SPEED);
    delay(50);
    if (checkLineDetected()) {
      Serial.println("Line detected during diversion!");
      return; // Exit diversion if line found
    }
  }
  
  // Second turn
  Left();
  delay(100);
  
  // Move and check for line again
  for (int i = 0; i < 10; i++) {
    Forward(SPEED);
    delay(50);
    if (checkLineDetected()) {
      Serial.println("Line detected during diversion!");
      return; // Exit diversion if line found
    }
  }
  
  // Final adjustment to find line
  Left();
  delay(100);
  Forward(SPEED);
}

long getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  long distance = (duration * 0.034) / 2;

  if (duration == 0) {
    return -1; // No reading
  }

  return distance;
}

// Renamed function as requested in comment
void printStatus()
{
  Serial.print("STATE: ");
  Serial.print(currentState);
  Serial.print(" | A Speed: ");
  Serial.print(currentSpeedA);
  Serial.print(" | B Speed: ");
  Serial.println(currentSpeedB);
}