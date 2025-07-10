//I2C, huskylens, proximity sesnor 
#include <Wire.h>
#include <MPU6050.h>
#include "HUSKYLENS.h"

// Motors pins, standby, pulse width modulation, direction 
#define PIN_Motor_STBY 3
#define PIN_Motor_PWMA 5
#define PIN_Motor_AIN_1 7
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8

// Ultrasonic 
#define TRIG_PIN 13
#define ECHO_PIN 12

//Ultrasonic threshold
#define DISTANCE_THRESHOLD 20 

// Objecte tracking 
int targetID = 1; 

//line tracking sensor 
const int leftSensorPin = A0;
const int centerSensorPin = A1;
const int rightSensorPin = A2;

// Thresholds for black line detection 
const int Lthreshold = 900;
const int Cthreshold = 900;
const int Rthreshold = 950;

HUSKYLENS huskylens;

//proximity sensor (for robot redirection)
MPU6050 mpu;
float currentHeading = 0;  
unsigned long lastOrientationCheck = 0;

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

void updateHeadingFromTurn(int angleDelta) {
  currentHeading += angleDelta;
  if (currentHeading >= 360) currentHeading -= 360;
  if (currentHeading < 0) currentHeading += 360;
}

void checkOrientation() {
  if (millis() - lastOrientationCheck >= 5000) {
    lastOrientationCheck = millis();
    Serial.print("Current heading: ");
    Serial.print(currentHeading);
    Serial.println(" degrees");
  }
}

// Motion functions
void moveForward(uint8_t speed_A, uint8_t speed_B, unsigned long duration_ms) {
  readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, speed_A);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, speed_B);
  delay(duration_ms);
  StopMotors();
  checkOrientation();
}
void moveBackward(uint8_t speed_A, uint8_t speed_B, unsigned long duration_ms) {
  readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, speed_A);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, speed_B);
  delay(duration_ms);
  StopMotors();
  checkOrientation();
}
void turnLeft(uint8_t speed, int duration_ms) {
  readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, speed);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, 0);
  delay(duration_ms);
  StopMotors();
  updateHeadingFromTurn(-90);  
  checkOrientation();
}
void turnRight(uint8_t speed, int duration_ms) {
  readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, 0);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, speed);
  delay(duration_ms);
  StopMotors();
  updateHeadingFromTurn(90);  
  checkOrientation();
}
void StopMotors() {
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 0);
}

//Reading distance with ultrasonic
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1; 

  long distance = duration * 0.034 / 2;
  return distance;
}

void ultraSonicTest() {
  long distance = readDistanceCM();

  Serial.print("Distance: ");
  Serial.println(distance);
  if (distance > 0 && distance <= DISTANCE_THRESHOLD) {
    delay(500); 
    turnRight(110, 800);

    moveForward(100, 100, 1000);

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j <4; j++{
        moveForward(100, 100, 3000);
        turnRight(100, 800);
      }
    }

    StopMotors();
    while (1); 
  }
}

//movement based on object detected
void checkAndMoveIfObjectFound() {
    if (!huskylens.request()) {
        Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    }
    else if (!huskylens.isLearned()) {
        Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    }
    else if (!huskylens.available()) {
        Serial.println(F("No block or arrow appears on the screen!"));
    }
    else {
        Serial.println(F("###########"));
        while (huskylens.available()) {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);

            // Check if the result is a block with the target ID
            if (result.command == COMMAND_RETURN_BLOCK && result.ID == targetID) {
                Serial.println("Target object found! Moving forward...");
                turnRight(50,2000);
                moveForward(50, 50, 15000);
                break; 
            }
        }
    }
}

//HuskeyConnection (serial test)
void HuskeyCamera() {
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
        }    
    }
}

//Serial test 
void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}
//line tracking sensor code 
void readLineSensorsAndAct() {
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
       StopMotors();
       while(1);
    } 
   
}


//Set and Loop
void setup() {
  Serial.begin(9600);
  pin_setup();

//  MPU6050 initialization
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1); // Halt
  } else {
    Serial.println("MPU6050 connected");
  }
  Serial.begin(115200);
    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

void loop() {
  moveForward(60,60,5000);
  turnRight(65,1500);
  moveForward(60,60,6000);
  turnRight(65,1500);
  moveForward(60,60,7000);
  turnLeft(65,1500);
  moveForward(60,60,2000);
  turnRight(65,3500);
  moveForward(60,60,8000);
  turnRight(65,3500);
  moveForward(60,60,11000);
  turnRight(65,3500);

  checkAndMoveIfObjectFound();

  while(1);


}
