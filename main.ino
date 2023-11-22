// TB6612FNG Driver Board Sample board
//
// Author: Ben Taylor
// University of Sheffield
// Date: September 2021

//class for storing all globals cause its good practise
//ONLY USE BYTE FOR PINS AND NOTHING ELSE
class Globals{
  
  public:
    
    //propulsion and motor pins - 
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

void loop()
{

  setMotor(150, true, false); // 255 max pin val. leftmotor is false forward is true 
  setMotor(150, false, true);
  delay(640);
  setMotor(0, true, false); // 255 max pin val. leftmotor is false forward is true 
  setMotor(0, false, true);
  delay(2000);
  setMotor(150, true, true); // 255 max pin val. leftmotor is false forward is true 
  setMotor(150, false, true);
  delay(2000);
  setMotor(0, true, false); // 255 max pin val. leftmotor is false forward is true 
  setMotor(0, false, true);



  //Serial.println("Compiled bitch");
  // Wait 300ms
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
  //avoids the use of a delay function with a for loop. 
  bool left;
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0; // Make previousMillis static to retain its value between function calls

  if (direction == 'L') {
    left = true;
  } else if (direction == 'R') {
    left = false;
  }

  // turn
  setMotor(speed, left, true);
  setMotor(speed, !left, false);
  if (currentMillis - previousMillis > time) {
    previousMillis = currentMillis; // Doesn't use delay
  }
  setMotor(0, true, true);
  setMotor(0, false, false);
}
