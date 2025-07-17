// I2C, huskylens, proximity sensor
#include <Wire.h>
#include "HUSKYLENS.h"
#include "MPU6050_getdata.h"

// Global objects
HUSKYLENS huskylens;
MPU6050_getdata AppMPU6050getdata;

// Motors pins
#define PIN_Motor_STBY 3
#define PIN_Motor_PWMA 5
#define PIN_Motor_AIN_1 7
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8

// Ultrasonic
#define TRIG_PIN 13
#define ECHO_PIN 12
#define DISTANCE_THRESHOLD 20

// Object tracking
int targetID = 1;

// Line tracking sensor
const int leftSensorPin = A0;
const int centerSensorPin = A1;
const int rightSensorPin = A2;

// Thresholds for black line detection
const int Lthreshold = 900;
const int Cthreshold = 900;
const int Rthreshold = 950;


void pin_setup() {
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_STBY, OUTPUT);
  digitalWrite(PIN_Motor_STBY, HIGH);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// Motion functions
void moveForward(uint8_t speed_A, uint8_t speed_B, unsigned long duration_ms) {
  float startYaw = getYawAngle();
  readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, speed_A);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, speed_B);
  delay(duration_ms);
  StopMotors();
  checkAndCorrectYaw(startYaw);
}

void moveBackward(uint8_t speed_A, uint8_t speed_B, unsigned long duration_ms) {
  float startYaw = getYawAngle();
  //readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, speed_A);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, speed_B);
  delay(duration_ms);
  StopMotors();
  checkAndCorrectYaw(startYaw);
}

void turnLeft(uint8_t speed, int duration_ms) {
  //readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, speed);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, 0);
  delay(duration_ms);
  StopMotors();
}

void turnRight(uint8_t speed, int duration_ms) {
  //readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, 0);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, speed);
  delay(duration_ms);
  StopMotors();
}

void StopMotors() {
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 0);
}

// Ultrasonic Distance
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

bool ultraSonicDance() {
  long distance = readDistanceCM();
  Serial.print("Distance: ");
  Serial.println(distance);
  return (distance > 0 && distance <= DISTANCE_THRESHOLD);
}
int trailsBeforeHalt=0;
// Movement based on object detected
void checkAndMoveIfObjectFound() {
  if (!huskylens.request()) {
    Serial.println(F("Fail to request data from HUSKYLENS!"));
  } else if (!huskylens.isLearned()) {
    Serial.println(F("HUSKYLENS not trained!"));
  } else if (!huskylens.available()) {
    Serial.println(F("No block detected!"));
  } else {
    Serial.println(F("###########"));
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      printResult(result);

      if (result.command == COMMAND_RETURN_BLOCK && result.ID == targetID) {
        Serial.println("Target found! Stopping.");
        StopMotors();
        delay(2000);
        break;
      } else if (ultraSonicDance()) {
        Serial.println("Obstacle detected! Stopping.");
        StopMotors();
        break;
      } else {
        if(trailsBeforeHalt <= 4){
        moveForward(50, 50, 1000);
        trailsBeforeHalt = trailsBeforeHalt + 1;
        }else{
          break;
        }
      }
    }
  }
}


// Serial print result from HUSKYLENS
void printResult(HUSKYLENSResult result) {
  if (result.command == COMMAND_RETURN_BLOCK) {
    Serial.println(String() + F("Block: x=") + result.xCenter + F(", y=") + result.yCenter + F(", w=") + result.width + F(", h=") + result.height + F(", ID=") + result.ID);
  }  else {
    Serial.println("Unknown object detected.");
  }
}

// Line tracking sensor
void readLineSensorsAndAct() {
  delay(1000);
  int left = analogRead(leftSensorPin);
  int center = analogRead(centerSensorPin);
  int right = analogRead(rightSensorPin);

  Serial.print("L: ");
  Serial.print(left);
  Serial.print(" C: ");
  Serial.print(center);
  Serial.print(" R: ");
  Serial.println(right);

  if (center > Cthreshold || left > Lthreshold || right > Rthreshold) {
    Serial.println("Line detected! Halting.");
    StopMotors();
    while (1); // Freeze for safety
  }
}

// MPU Test
int testMPU() {
  static float Yaw;
  AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
  Serial.print("Yaw (deg): ");
  Serial.println(Yaw);
  return Yaw;
}

// Get yaw angle using AppMPU6050getdata
float getYawAngle() {
  float yaw;
  AppMPU6050getdata.MPU6050_dveGetEulerAngles(&yaw);
  return yaw;
}

// Yaw correction logic
void checkAndCorrectYaw(float startYaw) {
  float endYaw = getYawAngle();
  float yawDiff = startYaw - endYaw;

  if (abs(yawDiff) > 2.0) {
    Serial.print("Yaw drift: ");
    Serial.println(yawDiff);
    correctOrientation(yawDiff);
  }
}

// Rotate to correct yaw
void correctOrientation(float yawError) {
  if (yawError > 0) {
    digitalWrite(PIN_Motor_AIN_1, HIGH);
    analogWrite(PIN_Motor_PWMA, 20);
    digitalWrite(PIN_Motor_BIN_1, LOW);
    analogWrite(PIN_Motor_PWMB, 20);
  } else {
    digitalWrite(PIN_Motor_AIN_1, LOW);
    analogWrite(PIN_Motor_PWMA, 20);
    digitalWrite(PIN_Motor_BIN_1, HIGH);
    analogWrite(PIN_Motor_PWMB, 20);
  }

  delay(1000);
  StopMotors();
}
void initializeHuskyOrWaitWithUltrasonic() {
  unsigned long startTime = millis();
  bool huskyConnected = false;

  // Try to connect to HuskyLens for 5 seconds
  while (millis() - startTime < 5000) {
    if (huskylens.begin(Wire)) {
      Serial.println("HUSKYLENS connected.");
      huskyConnected = true;
      break;
    }
    Serial.println(F("HUSKYLENS init failed. Retrying..."));
    delay(500);
  }

  // If HuskyLens still not connected, wait until object is detected via ultrasonic
  if (!huskyConnected) {
    Serial.println(F("HUSKYLENS timeout. Waiting for object with ultrasonic..."));
    while (true) {
      long distance = readDistanceCM(); // Corrected
      if (distance > 0 && distance <= DISTANCE_THRESHOLD) { // Corrected
        Serial.println(F("Object detected. Continuing setup."));
        break;
      }
      delay(200);
    }
  }

}
void WaitWithUltrasonic() {
  unsigned long startTime = millis()
    while (millis() - startTime < 5000) {
      Serial.println("start time");
      delay(500);

    }
    Serial.println(F("Waiting for object with ultrasonic..."));
    while (true) {
      long distance = readDistanceCM(); 
      if (distance > 0 && distance <= DISTANCE_THRESHOLD) { 
        Serial.println(F("Object detected. Continuing setup."));
        break;
      }
      delay(200);
    }
  }



// Setup
void setup() {
  Serial.begin(115200);
  pin_setup();

  // MPU init
  bool res_error = AppMPU6050getdata.MPU6050_dveInit();
  AppMPU6050getdata.MPU6050_calibration();
  initializeHuskyOrWaitWithUltrasonic();

  Serial.println("Setup complete.");
}

// Main loop
void loop() {
  delay(2000);
  for (int i = 0 ; i <=3; i++){
      moveForward(60,60,1000);

  }
  turnRight(65,1500);
   for (int i = 0 ; i <=5; i++){
      moveForward(60,60,1000);

  }
  turnRight(65,1500);
  for (int i = 0 ; i <=3; i++){
      moveForward(70,70,1000);
  }
  turnLeft(65,1500);
  moveForward(60,60,2000);
  turnRight(78,2500);
  for (int i = 0 ; i <=4; i++){
      moveForward(70,70,1000);
  }  
  turnRight(100,1550);
  for (int i = 0 ; i <=7; i++){
      moveForward(70,70,1000);
  }   turnRight(80,1700);
  //moveForward(70,70,1000);
  //checkAndMoveIfObjectFound();
  turnRight(100,1200);
  //moveForward(50,50,1000);
  //talk delay
  delay(20000);
  
  moveForward(70,70,1000);
  moveForward(70,70,1000);
  // turnRight(120,1200);
 for (int i = 0 ; i <=4; i++){
      moveForward(70,70,1000);
  }   turnRight(100,1200);
   for (int i = 0 ; i <=4; i++){
      moveForward(70,70,1000);
  }  
 turnRight(100,1200);
 
  for (int i = 0 ; i <=5; i++){
       moveForward(70,70,1000);
   }  
   //return rotation for UAE
   turnRight(80,4500);
  //change
  delay(20000);
  for (int i = 0 ; i <=4; i++){
      moveForward(70,70,1000);
  }   turnRight(80,1000);

for (int i = 0 ; i <=4; i++){
      moveForward(70,70,1000);
  }   turnLeft(80,1000);

for (int i = 0 ; i <=4; i++){
      moveForward(70,70,1000);
  }  

     turnLeft(80,4500);

for (int i = 0 ; i <=4; i++){
      moveForward(70,70,1000);
  }   turnLeft(80,2000);
  for (int i = 0 ; i <=4; i++){
      moveForward(70,70,1000);
  } 
  for(int i = 0; i<4; i++){
    for (int j = 0; j<4; j++){
      moveForward(50,50,1000);
      turnRight(100,1500);
    }

  }

  while(1);
 
}
