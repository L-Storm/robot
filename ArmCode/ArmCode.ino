#include <Servo.h>


Servo BottomServo;  // create servo object to control a servo
Servo MiddleServo;
Servo TopServo;
Servo FlagsServo;

// twelve servo objects can be created on most boards
int BottomServoPin = 10;
int MiddleServoPin = 7;
int TopServoPin = 8;
int FlagsServoPin = 9;

int bottomPos = 0;    // variable to store the servo position
int middlePos = 0;
int topPos = 0;
int flagsPog = 0;

void setup() {
    BottomServo.attach(BottomServoPin);  // attaches the servo on pin 9 to the servo object
    MiddleServo.attach(MiddleServoPin);
    TopServo.attach(TopServoPin);
    FlagsServo.attach(FlagsServoPin);

    BottomServo.write(0);
    MiddleServo.write(180);
    FlagsServo.write(0);
    
    
    Serial.begin(9600);  
    delay(2000);
}

void loop() {
  
  // in steps of 1 degree
  //armFold();
  //armExtend();
  MiddleServo.write(0);
  //FlagsServo.write(0);
  //delay(1000);
  //BottomServo.write(170);
  //FlagsServo.write(180);
  //armExtend();
  MiddleServo.write(180);
  //FlagsServo.write(180);
  delay(1000);
  //BottomServo.write(180);
  //armSpin();

}

void armFold(){
    
    middlePos = 180;
    MiddleServo.write(middlePos);
    bottomPos = 0;
    BottomServo.write(bottomPos);
    delay(3000);
}

void armExtend(){
    
    middlePos = 90;
    MiddleServo.write(middlePos);
    bottomPos = 70;
    BottomServo.write(bottomPos);

    delay(3000);
}

void armSpin(){
    topPos = 0;
    TopServo.write(topPos);
    delay(2000);
    for (int i = 0; i <= 180; i = i + 10){
        topPos = i;
        TopServo.write(topPos);
        delay(50);
    }
    topPos = 0;
    TopServo.write(topPos);
    delay(2000);
    
}