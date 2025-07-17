// Motors Pins
#define PIN_Motor_STBY 3
#define PIN_Motor_PWMA 5
#define PIN_Motor_AIN_1 7
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8

unsigned long startTime;
bool hasRotated180 = false;


//Set up & Loops
void pin_setup() {
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_STBY, OUTPUT);
  digitalWrite(PIN_Motor_STBY, HIGH);
}

void StopMotors() {
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 0);
}

void turnLeft180(uint8_t speed) {
  digitalWrite(PIN_Motor_AIN_1, LOW);    
  analogWrite(PIN_Motor_PWMA, speed);
  digitalWrite(PIN_Motor_BIN_1, HIGH);  
  analogWrite(PIN_Motor_PWMB, speed);
  delay(1650); 
  StopMotors();
}      

void setup() {
  pin_setup();
  StopMotors();          
  startTime = millis(); 
}

void loop() {
   if (!hasRotated180 && millis() - startTime >= 110000) {
    turnLeft180(100);
    hasRotated180 = true;
    while(1);   }

  }