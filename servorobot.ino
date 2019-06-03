#include <Servo.h>
#include <Keypad.h>

// D I/O ports for the servo motors
const int leftMotorID= 12;
const int rightMotorID = 13;

// ports for the ultrasonic sensor
const int trigPin = 10;
const int echoPin = 11;

// Time since arduino booted up
unsigned long time;

long duration;
double distance;

// Define Arduino Keypad
const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {6,7,8,9}; 
byte colPins[COLS] = {2,3,4,5}; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), colPins, rowPins, ROWS, COLS); 

// Constant team number list to compare the current list of keys to
const char teamNumberList[] = {'5','2','8','8'};
// Current list of keys pressed
char keysList[] = {'A','B','C','D'};

// Define servos
Servo leftMotor, rightMotor; 
// Define value used to stop servos from rotating 
int stopValue = 1410;// 1410

// Define the last key pressed/the key pressed in the current loop
char currentKey;

// Arduino initialization -> startup code
void setup() {
  Serial.begin(9600);
 leftMotor.attach(leftMotorID);   
 rightMotor.attach(rightMotorID);
 stopMotors();
 pinMode(trigPin,OUTPUT); // Sets the trigPin as an output
 pinMode(echoPin,INPUT); // sets the echoPin as an input
 
}

// Main Arduino loop
void loop() {
  currentKey = customKeypad.getKey();
  // If a key is pressed, add it to the current stack of 4 keys
  if(currentKey != NO_KEY) {

    arrayInsert(keysList,currentKey);
    // Print last 4 keys in stack to user
    Serial.print("CURRENT KEYS: ");
    printCharArray(keysList);
    // If the last 4 keys pressed is the team number (5288), get the car to move forward
    if(doArraysMatch(teamNumberList,keysList)) {
      Serial.print("Distance: ");
      Serial.println(getDistance());
      driveForward(5);
    }
  } 

}
// Swap two items in a character array
void swap(char listToSwap[], byte index1, byte index2) {
    char switcher = listToSwap[index1];
    listToSwap[index1] = listToSwap[index2];
    listToSwap[index2] = switcher;
}

// Drive robot forward until distance
void driveForward(double distConstantCM) {
  while(getDistance() > distConstantCM) {    
    leftMotor.write(180);
    rightMotor.write(0);
    delay(250);
  }
  stopMotors();
}

// Stop both car motors
void stopMotors() {
  leftMotor.writeMicroseconds(stopValue-15);
  rightMotor.writeMicroseconds(stopValue-10);
}

// Check if arrays (of size 4) match
bool doArraysMatch(char list1[], char list2[]) {
  bool arraysMatch = true;
  for(int i = 0; i < 4; i++) {
    if(list1[i] !=list2[i]) {
      arraysMatch = false;
    }
  }
  return arraysMatch;
}

// Append item to character array, pushing out the first element (size 4)
void arrayInsert(char listToBubble[], char toBubble) {
  int listSize =  sizeof(listToBubble);
  char savedVariable1 = toBubble;
  char savedVariable2;
  for(int i = 3;  i >-1; i--) {
    savedVariable2 = listToBubble[i];
    listToBubble[i] = savedVariable1;
    savedVariable1 = savedVariable2;

  }
}
// Print a character array to the user
void printCharArray(char listToPrint[]) {
  int listSize = sizeof(listToPrint);
  for(int i = 0; i < 4; i++) {
    Serial.print(listToPrint[i]);
  }
  Serial.print("\n");
}
// returns distance in centimetres
double getDistance() {
  // reset sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  // d = velocity*time -> velocity is speed of sound/2 because the sound waves bounce back
  distance= duration*0.034/2;
  return distance;
}