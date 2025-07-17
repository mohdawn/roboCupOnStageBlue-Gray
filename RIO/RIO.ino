//I2C, proximity sesnor 
#include <Wire.h>
#include "MPU6050_getdata.h"

MPU6050_getdata AppMPU6050getdata;

// Motors pins, standby, pulse width modulation, direction 
#define PIN_Motor_STBY 3
#define PIN_Motor_PWMA 5
#define PIN_Motor_AIN_1 7
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8

// Ultrasonic 
#define TRIG_PIN 13
#define ECHO_PIN 12

// Ultrasonic threshold
#define DISTANCE_THRESHOLD 20  


//line tracking sensor 
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
void moveBackward(uint8_t speed_A, uint8_t speed_B, unsigned long duration_ms) {
  float startYaw = getYawAngle();
  //readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, speed_A);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, speed_B);
  delay(duration_ms);
  checkAndCorrectYaw(startYaw);

}
void moveForward(uint8_t speed_A, uint8_t speed_B, unsigned long duration_ms) {
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
  float startYaw = getYawAngle();
  //readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, speed);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, 0);
  delay(duration_ms);
  StopMotors();
  checkAndCorrectYaw(startYaw);
}

void turnRight(uint8_t speed, int duration_ms) {
  float startYaw = getYawAngle();
  //readLineSensorsAndAct();
  digitalWrite(PIN_Motor_AIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, 0);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, speed);
  delay(duration_ms);
  StopMotors();
  checkAndCorrectYaw(startYaw);
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
      moveForward(100, 100, 1000);
      turnRight(100, 800);
    }

    StopMotors();
    while (1); 
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
    digitalWrite(PIN_Motor_AIN_1, LOW);
    analogWrite(PIN_Motor_PWMA, 60);
    digitalWrite(PIN_Motor_BIN_1, HIGH);
    analogWrite(PIN_Motor_PWMB, 60);
  } else {
    digitalWrite(PIN_Motor_AIN_1, HIGH);
    analogWrite(PIN_Motor_PWMA, 60);
    digitalWrite(PIN_Motor_BIN_1, LOW);
    analogWrite(PIN_Motor_PWMB, 60);
  }

  StopMotors();
}

//Set and Loop
void setup() {
   Serial.begin(115200);
  pin_setup();

  // MPU init
  bool res_error = AppMPU6050getdata.MPU6050_dveInit();
  AppMPU6050getdata.MPU6050_calibration();


}

void loop() {
   delay(2000);
  
  for(int i = 0; i<20; i++){
    for (int j = 0; j<4; j++){
      moveForward(50,50,1000);
      turnLeft(100,1500);
    }

  }

  while(1);
  // delay(43000);
  // moveForward(55,90,5500);
  // delay(18000);
  // turnLeft(85,1350);
  // moveForward(55,90,12000);
  // delay(1000);
  // turnLeft(85,1350);
  // moveForward(55,90,3000);
  // turnLeft(85,1350);
  // moveForward(55,90,3500);
  // delay(3000);
  // moveForward(55,90,3500);
  // delay(3000);
  // moveForward(55,90,3500);
  // delay(3000);
  // turnLeft(85,2000);
  // moveForward(55,90,5000);
  // turnRight(85,750);
  // for (int i = 0; i <=4; i++){
  //     delay(3000);
  //     for (int i = 0; i <=4; i++){
  //     moveForward(55,90,1000);
  //     turnLeft(85,1350);
  //   }
  // }


  while(1);
}
