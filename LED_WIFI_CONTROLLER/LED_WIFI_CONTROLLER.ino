/****************************************
Web Server for ESP8266 + Relay Controller + Keypad V2
Product of: GECT Company
Created by: Nguyễn Minh Triết
Date of manufacture: 16/8/2020 
-----THANKS FOR USING-----
****************************************/
// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "note8";
const char* password = "minhtriet@123456";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";
String output0State = "off";
String output1State = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;
const int output0 = 0;
const int output1 = 1;

//Keypad variables
int rowCounter = 0; // Set row counter
int columnCounter = 0; // Set column counter
boolean foundCol = false;
int keyValue = 0;
int noKey = 0;
boolean readKey = false;
int debounce = 300; 
const byte rows = 1; // 1 rows
const byte columns = 4; // 4 columns
byte rowfound = 1;
byte columnfound = 4;
// For ESP8266 Microcontroller
byte rowPins[rows] = {D8}; 
byte colPins[columns] = {D4, D5, D6, D7};
char keys[rows][columns] = {'1', '2', '3', 'A'};

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output0, OUTPUT);
  pinMode(output1, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output0, LOW);
  digitalWrite(output1, LOW);

  //Keypad setup
  pinMode(rowPins[0],OUTPUT);
  for (byte c=0; c<columns; c++) {
    pinMode(colPins[c],INPUT_PULLUP);
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void keyPad(){
  if(noKey == 4){
    readKey = true;
  }
  noKey = 0; 
  for(rowCounter = 0; rowCounter < rows ; rowCounter++){  
    scanRow();
    for(columnCounter = 0; columnCounter < columns; columnCounter++){
      readColumn();
      if(foundCol == true){
        keyValue = columnCounter + 4*rowCounter;
        rowfound = rowCounter;
        columnfound = columnCounter;
      }
    }
  }
  if(readKey == true && noKey == 3){
    Serial.println(keys[rowfound][columnfound]);// debug
    Serial.print(rowfound);// debug
    Serial.println(columnfound);// debug
  }
  readKey = false;//reset readKey 
  delay(debounce);//debounce
}
void scanRow(){
  digitalWrite(rowPins[rowCounter], HIGH);//set all rows High
  digitalWrite(rowPins[rowCounter], LOW);
}
void readColumn(){
  foundCol = digitalRead(colPins[columnCounter]);// wrong varailable
  if(foundCol == false){
    foundCol = true;
  }else{
    foundCol = false;
    noKey = noKey + 1;
  }
}

void loop(){
  keyPad();
  switch(keys[rowfound][columnfound]){
    case '1' ://Turn GPIO 5 ON/OFF
      if (output5State=="off"){
        Serial.println("GPIO 5 on");
        output5State = "on";
        digitalWrite(output5, HIGH);
        rowfound = 1;
        columnfound = 4;
      }else if (output5State=="on"){
        Serial.println("GPIO 5 off");
        output5State = "off";
        digitalWrite(output5, LOW);
        rowfound = 1;
        columnfound = 4;
      }
      break;
    case '2' ://Turn GPIO 4 ON/OFF
      if (output4State=="off"){
        Serial.println("GPIO 4 on");
        output4State = "on";
        digitalWrite(output4, HIGH);
        rowfound = 1;
        columnfound = 4;
      }else if (output4State=="on"){
        Serial.println("GPIO 4 off");
        output4State = "off";
        digitalWrite(output4, LOW);
        rowfound = 1;
        columnfound = 4;
      }
      break;
    case '3' ://Turn GPIO 0 ON/OFF
      if (output0State=="off"){
        Serial.println("GPIO 0 on");
        output0State = "on";
        digitalWrite(output0, HIGH);
        rowfound = 1;
        columnfound = 4;
      }else if (output0State=="on"){
        Serial.println("GPIO 0 off");
        output0State = "off";
        digitalWrite(output0, LOW);
        rowfound = 1;
        columnfound = 4;
      }
      break;
    case 'A' ://Turn GPIO 1 ON/OFF
      if (output1State=="off"){
        Serial.println("GPIO 1 on");
        output1State = "on";
        digitalWrite(output1, HIGH);
        rowfound = 1;
        columnfound = 4;
      }else if (output1State=="on"){
        Serial.println("GPIO 1 off");
        output1State = "off";
        digitalWrite(output1, LOW);
        rowfound = 1;
        columnfound = 4;
      }
      break;
  }
  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            } else if (header.indexOf("GET /0/on") >= 0) {
              Serial.println("GPIO 0 on");
              output0State = "on";
              digitalWrite(output0, HIGH);
            } else if (header.indexOf("GET /0/off") >= 0) {
              Serial.println("GPIO 0 off");
              output0State = "off";
              digitalWrite(output0, LOW);
            } else if (header.indexOf("GET /1/on") >= 0) {
              Serial.println("GPIO 1 on");
              output1State = "on";
              digitalWrite(output1, HIGH);
            } else if (header.indexOf("GET /1/off") >= 0) {
              Serial.println("GPIO 1 off");
              output1State = "off";
              digitalWrite(output1, LOW);
            } 
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #00DD00; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #0000CC;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>LED 1 - GPIO 5 - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>LED 2 - GPIO 4 - State " + output4State + "</p>");
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // Display current state, and ON/OFF buttons for GPIO 0  
            client.println("<p>LED 3 - GPIO 0 - State " + output0State + "</p>");
            // If the output0State is off, it displays the ON button       
            if (output0State=="off") {
              client.println("<p><a href=\"/0/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/0/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // Display current state, and ON/OFF buttons for GPIO 1  
            client.println("<p>LED 4 - GPIO 1 - State " + output1State + "</p>");
            // If the output1State is off, it displays the ON button       
            if (output1State=="off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
