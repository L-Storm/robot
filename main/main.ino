#include <Servo.h>
#include <NewPing.h>
#include <Encoder.h>
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
      const byte bottomServoPin = 10; 
      const byte middleServoPin = 7; 
      const byte topServoPin = 8;
      const byte flagServoPin = 9; 
      const byte encoderPinA = 18; //this the main guy.
      const byte encoderPinB = 22; //less important just direction.
      const long rotate180 = 2320; 
      const long rotate90 = 1220;
      const long widthTime = 3230; //distance from x to line 
      const long echoOnLine = 1150;
      const long echoToDrawingBoard = 999;
      const long encoder180 = 4850; 
      const long encoder90 = 2425;
      const float corrFactor = 1.05; //increase for faster right motor       
};
Globals globals; //call the class 
NewPing sonar(globals.frontTriggerPin, globals.frontEchoPin);
Encoder wheel(globals.encoderPinA, globals.encoderPinB);

Servo BottomServo;  // create servo object to control a servo
Servo MiddleServo;
Servo TopServo;
Servo FlagsServo;
int bottomPos = 0;    // variable to store the servo position
int middlePos = 0;
int topPos = 0;
int flagsPog = 0;

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


  //servo stuff
  BottomServo.attach(globals.bottomServoPin);  // attaches the servo on pin 9 to the servo object
  MiddleServo.attach(globals.middleServoPin);
  TopServo.attach(globals.topServoPin);
  FlagsServo.attach(globals.flagServoPin);
  //below cmds initially fold servo
  armFold(); 
  FlagsServo.write(0);
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
  delay(5000);
  FlagsServo.write(0);
  //TURN1: initial 180
  stopIfEncode(globals.encoder180, 'R');
  Serial.print("encoder post turn1:");
  Serial.println(wheel.read());  
  delay(500);
  //FORWARD1: to line (no feedback in commented out function)
  //straight(150,globals.widthTime,'F'); //to line 
  stopIfEcho(globals.echoOnLine);
  delay(500);

  //TURN2: turn face whiteboard
  stopIfEncode(globals.encoder90, 'L');
  //rotate(100,globals.rotate90,'L'); //turn to face bridge 
  delay(500);

  //FORWARD2: move to whiteboard. 
  //straight(150,4000,'F'); //unsure if this time value is correct.
  stopIfEcho(globals.echoToDrawingBoard);
  delay(500);
  //DELAY1: wait for drawing to finish. 

  armExtend(); 
  armSpin();
  armFold(); 

  //REVERSE1: move away from drawing board. 
  //straight(150,1000,'B'); // reverse 
  //delay(500);
  
  //TURN3: turn to face red line.
  stopIfEncode(globals.encoder90, 'L');
  //rotate(100,globals.rotate90,'L'); 
  delay(500);

  //FORWARD3: move towards bridge hole. 
  //straight(150,globals.widthTime,'F'); //to line 
  stopIfEcho(globals.echoOnLine);
  delay(500);

  //TURN4: face starting position. 
  stopIfEncode(globals.encoder90, 'L');
  //rotate(100,globals.rotate90,'L'); //turn to face bridge 
  delay(500);
  
  //FORWARD4 return to home position. 
  //straight(150,3200,'F'); //to x
  stopIfEcho(globals.echoOnLine);
  delay(500);


  //TURN5: face away and return to starting position.
  stopIfEncode(globals.encoder90, 'R');
  //rotate(100,globals.rotate90,'R'); //turn to face bridge 
  delay(500);

  //RAISE FLAGS:
  FlagsServo.write(180);

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
    topPos = 50;
    TopServo.write(topPos);
    delay(2000);    
}

void stopIfEncode(long encodeVal, char direction) {
  //direction needs to be either 'L' or 'R' char input. 
  bool left;
  if (direction == 'L') {
    left = true;
  } else if (direction == 'R') {
    left = false;
  }

  setMotor(100, true, !left); // Set both motors forward 
  setMotor(100, false, left); // Set both

  bool fin = false;
  wheel.write(0);
  while (!fin) {
    if (abs(wheel.read()) >= encodeVal) {
      setMotor(0, true, true); // Stop the motor in one direction
      setMotor(0, false, true); // Stop the motor in the opposite direction
      fin = true;
      break;
    }
  }
  wheel.write(0);
}
