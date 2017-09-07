#include <Servo.h>

// !!! PIN SETTINGS !!!
int servoPin1 = 9;
int servoPin2 = 10;
int servoPin3 = 11;
int IRPin1 = 0;
int IRPin2 = 1;
int IRPin3 = 2;
int ledPin1 = 3;
int ledPin2 = 4;
int ledPin3 = 5;
int buzzerPin = 12;
// END PIN SETTINGS

// Number of servo activations per sequence.
int seqLength = 10;

// Keep track of the last activated servo - the first servo to activate is always 0.
int lastServo = 10;
int servoId = 0;

// Pin 13 has an LED connected on most Arduino boards.
int ledPin = 13;

// Sensor input pins.
int sensors[] = {IRPin1, IRPin2, IRPin3};

// LED strip input pins.
int leds[] = {ledPin1, ledPin2, ledPin3};

// Create the servos.
Servo servo0;
Servo servo1;
Servo servo2;
Servo servos[] = {servo0, servo1, servo2};

// Function to rotate the servo 90 degrees "out".
void moveOut(Servo servo);
void moveOut(Servo servo) {
  for(int angle = 90; angle > 0; angle--) {
    servo.write(angle);
    delay(10);
  }
  
  /*servo.write(0);
  delay(1000);*/
}

// Function to rotate the servo 90 degrees "in".
void moveIn(Servo servo);
void moveIn(Servo servo) {
  for(int angle = 0; angle < 90; angle++) {
    servo.write(angle);
    delay(10);
  }
  
  /*servo.write(90);
  delay(1000);*/
}

// Randomises the next servo to activate, 0 = sequence start, results in 1.
int nextServo(int lastServo) {
  if(lastServo == 10) {
    return 0;
  }
  else {
    int numberArray[2];
  
    switch(lastServo) {
    
      case 0: 
      numberArray[0] = 1;
      numberArray[1] = 2;
      break;

      case 1: 
      numberArray[0] = 0;
      numberArray[1] = 2;
      break;

      case 2: 
      numberArray[0] = 0;
      numberArray[1] = 1;
      break;
    }
  
    int i = random(0, 2);
    int nextServo = numberArray[i];
    Serial.print("Array: ");
    Serial.print(numberArray[0]);
    Serial.print(" ");
    Serial.println(numberArray[1]);
    
  
    return nextServo;
  }
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  
  // Declare leds as output and turn off.
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  
  // Attach servos to their pins and reset positions to 0.
  servo0.attach(servoPin1);
  servo1.attach(servoPin2);
  servo2.attach(servoPin3);
  moveIn(servo0);
  moveIn(servo1);
  moveIn(servo2);

  Serial.println("Servos reset.");
  
  // Seed the randomiser.
  randomSeed(analogRead(3));
  
}

// the loop routine runs over and over again forever:
void loop() {
  
  for(int i = 0; i < seqLength; i++) {
    Serial.println("");
    Serial.print("Round ");
    Serial.println(i);
    servoId = nextServo(lastServo);
    Serial.print("Randomiser: ");
    Serial.println(servoId);
    Serial.println("");

    digitalWrite(leds[servoId], HIGH);
    tone(buzzerPin, 500);
    moveOut(servos[servoId]);
    noTone(buzzerPin);

    Serial.print("Delay");

    // Sensor read distance: Change value in while condition higher for more sensitivity, lower for less.
    while(analogRead(servoId) > 820) {
      Serial.print(". ");
      delay(500);
    }

    Serial.println("");

    digitalWrite(leds[servoId], LOW);
    moveIn(servos[servoId]);

    lastServo = servoId;
    delay(3000);
    
  }
  
  
  
}
