#include <Servo.h>

Servo BottomServo;  // create servo object to control a servo
Servo MiddleServo;

// twelve servo objects can be created on most boards
int BottomServoPin = 11;
int MiddleServoPin = 10;

int bottomPos = 0;    // variable to store the servo position
int middlePos = 0;

void setup() {
    BottomServo.attach(BottomServoPin);  // attaches the servo on pin 9 to the servo object
    MiddleServo.attach(MiddleServoPin);
    
    
    Serial.begin(9600);  
}

void loop() {
  
    // in steps of 1 degree
    armFold();
    armExtend();
   

}

void armFold(){
    bottomPos = 170;
    BottomServo.write(bottomPos);
    middlePos = 0;
    MiddleServo.write(middlePos);
    delay(3000);
}

void armExtend(){
    
    middlePos = 170;
    MiddleServo.write(middlePos);
    bottomPos = 0;
    BottomServo.write(bottomPos);

    delay(3000);
}