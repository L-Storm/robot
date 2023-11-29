#include <Servo.h>

Servo BottomServo;  // create servo object to control a servo
Servo MiddleServo;
Servo TopServo;
Servo FlagsServo;

// twelve servo objects can be created on most boards
int BottomServoPin = 11;
int MiddleServoPin = 10;
int TopServoPin = 9;
int FlagsServoPin = 6;

int bottomPos = 0;    // variable to store the servo position
int middlePos = 0;
int topPos = 0;
int flagsPog = 0;

void setup() {
    BottomServo.attach(BottomServoPin);  // attaches the servo on pin 9 to the servo object
    MiddleServo.attach(MiddleServoPin);
    TopServo.attach(TopServoPin);
    FlagsServo.attach(FlagsServoPin);

    BottomServo.write(90);
    MiddleServo.write(90);
    FlagsServo.write(90);
    
    
    Serial.begin(9600);  
    delay(2000);
}

void loop() {
  
    // in steps of 1 degree
    //armFold();
    //armExtend();
   FlagsServo.write(0);
   delay(500);
   FlagsServo.write(90);
   delay(500);
   FlagsServo.write(180);
   delay(500);
}

void armFold(){
    
    middlePos = 0;
    MiddleServo.write(middlePos);
    bottomPos = 170;
    BottomServo.write(bottomPos);
    delay(3000);
}

void armExtend(){
    
    middlePos = 170;
    MiddleServo.write(middlePos);
    bottomPos = 0;
    BottomServo.write(bottomPos);

    delay(3000);
}

void armSpin(){
    topPos = 0;
    TopServo.write(topPos);
    delay(2000);
    for (int i = 0; i <= 360; i = i + 10){
        topPos = i;
        TopServo.write(topPos);
        delay(50);
    }
    topPos = 0;
    TopServo.write(topPos);
    delay(2000);
    
}