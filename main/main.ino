// TB6612FNG Driver Board Sample board
//
// Author: Ben Taylor
// University of Sheffield
// Date: September 2021
#include <NewPing.h>
//class for storing all globals cause its good practise
//ONLY USE BYTE FOR PINS AND NOTHING ELSE
class Globals{
  
  public:
    
    //propulsion and motor pins - left motor is marked with tape
      const byte lineFollowLeftPin = 37; //left sensor
      const byte lineFollowCenterPin = 36; //center sensor 
      const byte lineFollowRightPin = 35; //right pin 
      const byte frontTriggerPin = 41; //trig pin front 
      const byte frontEchoPin = 40; //echo pin front  
      const byte rightFrontPin = 49; //A1 
      const byte rightReversePin = 53; //A2 
      const byte leftFrontPin = 52; //B1 
      const byte leftReversePin = 50; //B2 
      const byte leftDrivePin = 3;  // left drive PIN PWMB
      const byte rightDrivePin = 2; // right drive PIN PWMA
      const byte truePin = 51;       // pin for true value in DC motor control (standby)
      const long rotate180 = 2000; 
      const long rotate90 = rotate180/2 -30;
      const long widthTime = 3230; //distance from x to line 
      const long echoOnLine = 1250;
      const long echoToDrawingBoard = 300;
      const float corrFactor = 1.06;




      
};
Globals globals; //call the class 
NewPing sonar(globals.frontTriggerPin, globals.frontEchoPin);

//NewPing sonar(globals.frontTriggerPin, globals.frontEchoPin); //doesn't work inside globals. 
void setup()
{
  
  // Assign the digital I/O pin directions
  pinMode(globals.truePin, OUTPUT);
  pinMode(globals.leftFrontPin, OUTPUT);
  pinMode(globals.rightReversePin, OUTPUT); 
  pinMode(globals.rightFrontPin, OUTPUT);
  pinMode(globals.rightReversePin, OUTPUT);
  pinMode(globals.leftDrivePin, OUTPUT);
  pinMode(globals.rightDrivePin, OUTPUT);
  
  pinMode(globals.lineFollowLeftPin, INPUT); 
  pinMode(globals.lineFollowCenterPin, INPUT);
  pinMode(globals.lineFollowRightPin, INPUT); 

  // Initialize the serial port
  Serial.begin(9600);

  // Drive the standby pin high to enable the output and the true part of the TF values. 
  digitalWrite(globals.truePin, HIGH);
}


/*
  delay(50); // Wait 50ms between pings (this is the minimum recommended by NewPing library)
  
  unsigned int distance = sonar.ping_median(); // Get the distance in millimeters

  if (distance == NO_ECHO) {
    Serial.println("Error: No echo");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  }

  setMotor(150, true, true); // Set both motors forward 
  setMotor(150, false, true); // Set both motors forward 
  bool side = false;

  while (!side) {
    // While loop to stop the robot if the distance is below 866
    unsigned int distance = sonar.ping_median();
    Serial.println(distance);

    // If the distance is less than or equal to 866, stop the motors and exit the loop
    if (distance <= 866) {
      setMotor(0, true, true); // Stop the motor in one direction
      setMotor(0, false, true); // Stop the motor in the opposite direction
      side = true;
      break;
    }
  }
*/
void loop() {
  //TURN1: initial 180
  rotate(100,globals.rotate180,'R'); //init rotate //2210 not good on full charge. 
  
  //FORWARD1: to line (no feedback in commented out function)
  //straight(150,globals.widthTime,'F'); //to line 
  stopIfEcho(globals.echoOnLine);

  //TURN2: turn face whiteboard
  rotate(100,globals.rotate90,'L'); //turn to face bridge 

  //FORWARD2: move to whiteboard. 
  //straight(150,4000,'F'); //unsure if this time value is correct.
  stopIfEcho(globals.echoToDrawingBoard);
  //DELAY1: wait for drawing to finish. 
  delay(2000); 

  //REVERSE1: move away from drawing board. 
  straight(150,1000,'B'); // reverse 
  
  //TURN3: turn to face red line.
  rotate(100,globals.rotate90,'L'); 

  //FORWARD3: move towards bridge hole. 
  //straight(150,globals.widthTime,'F'); //to line 
  stopIfEcho(globals.echoOnLine);

  //TURN4: face starting position. 
  rotate(100,globals.rotate90,'L'); //turn to face bridge 
  
  //FORWARD4 return to home position. 
  //straight(150,3200,'F'); //to x
  stopIfEcho(globals.echoOnLine);


  //TURN5: face away and return to starting position.
  rotate(100,globals.rotate90,'R'); //turn to face bridge 

  delay(5000);
}



void setMotor(int pwmVal, bool leftMotor, bool forward)
{
  // setMotor sets the specific pwmVal into a specific motor. 
  // pwmVal max 255 (don't use more than 40 on live robot).
  // first input is LeftMotor = True, False for Right motor
  // 
  if (forward && leftMotor) { //left motor go forward
    digitalWrite(globals.leftFrontPin, HIGH);
    digitalWrite(globals.leftReversePin, LOW);
  } else if (forward && !leftMotor) { //right motor go forward
    digitalWrite(globals.rightFrontPin, HIGH);
    digitalWrite(globals.rightReversePin, LOW);
    pwmVal = pwmVal*globals.corrFactor;
  } else if (!forward && leftMotor) { //left motor go backwards
    digitalWrite(globals.leftFrontPin, LOW);
    digitalWrite(globals.leftReversePin, HIGH);
  } else if (!forward && !leftMotor) { //right motor go backwards
    digitalWrite(globals.rightFrontPin, LOW);
    digitalWrite(globals.rightReversePin, HIGH);
    pwmVal = pwmVal*globals.corrFactor;
  }
  int motor; 
  if (leftMotor)
  {
    motor = globals.leftDrivePin;
  } else {
    motor = globals.rightDrivePin;
  }


  analogWrite(motor, pwmVal); // set motor speed 
}

void rotate(int speed, long time, char direction) {
  //direction needs to be either 'L' or 'R' char input. 
  bool left;
  if (direction == 'L') {
    left = true;
  } else if (direction == 'R') {
    left = false;
  }

  // turn
  setMotor(speed, left, false);
  setMotor(speed, !left, true);
  delay(time);
  setMotor(0, true, true);
  setMotor(0, false, false);
}

void straight(int speed, long time, char direction) {
  //direction needs to be either 'F' or 'B' char input. 
  bool left;
  if (direction == 'F') {
    left = true;
  } else if (direction == 'B') {
    left = false;
  }

  // turn
  setMotor(speed, true, left);
  setMotor(speed, false, left);
  delay(time);
  setMotor(0, true, true);
  setMotor(0, false, false);
}

void stopIfEcho(long time) {
  setMotor(150, true, true); // Set both motors forward 
  setMotor(150, false, true); // Set both motors forward 
  bool side = false;
  while (!side) {
    // While loop to stop the robot if the distance is below 866
    unsigned int distance = sonar.ping_median();
    Serial.println(distance);
    // If the distance is less than or equal to 866, stop the motors and exit the loop
    if (distance <= time) {
      setMotor(0, true, true); // Stop the motor in one direction
      setMotor(0, false, true); // Stop the motor in the opposite direction
      side = true;
      break;
    }
  }
}
