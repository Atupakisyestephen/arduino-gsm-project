#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
const int ledPin = 13;
const int irPin = 2; // IR sensor pin
const int buttonPin = 3; // Button pin
bool motionDetected = false;
int IR_state;
int buttonState;
const int soundSensorPin = A0; // Analog pin connected to the output of the sound sensor
const int threshold = 900;     // Threshold for detecting a clap (adjust as needed)
bool clapDetected = false;
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(irPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Assuming the button is connected to ground
  
  digitalWrite(ledPin, LOW);
  
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);
  mySerial.println("AT+CMGF=1"); // Set SMS to text mode
  delay(1000);
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Set module to automatically show new SMS
  delay(1000);
}

void loop() {
  // Read the analog value from the sound sensor
  int sensorValue = analogRead(soundSensorPin);

  // Print the sensor value to the Serial Monitor for debugging
  Serial.println(sensorValue);
  
  // Check if motion is detected by IR sensor
  
  IR_state=digitalRead(irPin);
  buttonState = digitalRead(buttonPin);
  if ( IR_state== LOW ) {
    motionDetected = true;
    digitalWrite(ledPin, HIGH);
    SendMessage("Motion detected!");
  }

  // If the sensor value is above the threshold, turn on the LED
 /*if (sensorValue > threshold) {
    digitalWrite(ledPin, HIGH);  // Turn on the LED
    clapDetected = true;
    SendMessage("SOUND DETECTED!");
    delay(500);                  // Delay to avoid multiple detections from a single clap
 }
 */   
  // Check if the reset button is pressed
  if ( buttonState== HIGH) {
      clapDetected = false;
      motionDetected = false;
      digitalWrite(ledPin, LOW);
      delay(50); // Debounce delay
      SendMessage("System reset!");
    }
  

  // Check if there are any messages from GSM module
  if (mySerial.available()) {
    RecieveMessage();
  }
}

String readSerial() {
  delay(100);
  if (mySerial.available()) {
    return mySerial.readString();
  }
  return "";
}

void SendMessage(String text) {
  mySerial.println("AT+CMGF=1"); 
  delay(100);
  Serial.println(readSerial());
  mySerial.println("AT+CMGS=\"+255626561774\"");
  delay(100);
  Serial.println(readSerial());
  mySerial.println(text);
  mySerial.println((char)26);
  delay(100);
  Serial.println(readSerial());
}

void RecieveMessage() {
  if (mySerial.available()) {
    String message = mySerial.readString();
    Serial.println("Message received: " + message);
  }
}
