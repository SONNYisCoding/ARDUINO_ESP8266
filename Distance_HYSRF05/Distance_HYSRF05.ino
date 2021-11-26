/***********************************
Count Distance using HY_SRF50 sensor
By Sony, August 15.2020 
***********************************/
//For Arduino Microcontroller
//const int trigPin = 3;
//const int echoPin = 4;

//For ESP8266 Microcontroller
const int trigPin = D1;
const int echoPin = D2;

//For ESP32 Microcontroller
//const int trigPin = 5;
//const int echoPin = 4; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
void distance(){
  unsigned long duration; // Time varailable
  int distance;           // Distance varailable
  digitalWrite(trigPin,0);   // Turn Off Trig pin
  delayMicroseconds(2);
  digitalWrite(trigPin,1);   // Pulse from trig pin
  delayMicroseconds(10);   // Pulse length = 10 microSeconds
  digitalWrite(trigPin,0);   // tắt chân trig
  duration = pulseIn(echoPin, HIGH);  
  // Count Distance
  distance = int(duration/2/29.412);
  // Print to Serial
  Serial.print(distance);
  Serial.println("cm");
  delay(1000);
}
void loop(){
  distance();
}
