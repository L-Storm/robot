// TB6612FNG Driver Board Sample board
//
// Author: Ben Taylor
// University of Sheffield
// Date: September 2021
#include <NewPing.h>
#define TRIGGER_PIN 22
#define ECHO_PIN 23
NewPing sonar(TRIGGER_PIN, ECHO_PIN);

//class for storing all globals cause its good practise
//ONLY USE BYTE FOR PINS AND NOTHING ELSE
class Globals{
  
  public:
    
    //propulsion and motor pins - left motor is marked with tape
      const byte rightFrontPin = 7; //A1 
      const byte rightReversePin = 9; //A2 
      const byte leftFrontPin = 11; //B1 
      const byte leftReversePin = 12; //B2 
      const byte leftDrivePin = 6;  // left drive PIN PWMB
      const byte rightDrivePin = 5; // right drive PIN PWMA
      const byte truePin = 10;       // pin for true value in DC motor control (standby)
      const float corrFactor = 1.07;



      
};
Globals globals; //call the class 
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
*/
void loop() {
  rotate(150, 1550, 'R'); // Turn right 
  Serial.println("rotate");

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
  delay(5000);

  // Exit the loop() function
  return;
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
  unsigned long currentMillis = millis();

  if (direction == 'L') {
    left = true;
  } else if (direction == 'R') {
    left = false;
  }

  // turn
  setMotor(speed, left, true);
  setMotor(speed, !left, false);
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
  setMotor(speed, false, true);
  delay(time);
  setMotor(0, true, true);
  setMotor(0, false, false);
}
