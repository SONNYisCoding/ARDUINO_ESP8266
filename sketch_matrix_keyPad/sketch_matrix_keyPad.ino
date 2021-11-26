/********************
 4x4 matrix keypad
 By Sony, Jul 19. 2018
 Connect:
  Row 1 - Digital D1
  Row 2 - Digital D2
  Row 3 - Digital D3
  Row 4 - digital D4
        --
  Col A - Digital D5
  Col B - Digital D6
  Col C - Digital D7
  Col D - Digital D8
********************/
int rowCounter = 0; // Set row counter
int columnCounter = 0; // Set column counter
//int foundCol = 0;
boolean foundRow = false;
int keyValue = 0;
int noKey = 0;
boolean readKey = false;
int debounce = 300; 
const byte rows = 4; // 4 rows
const byte columns = 4; // 4 columns
byte rowfound = 4;
byte columnfound = 4;
// For Arduino Microcontroller
//byte rowPins[rows] = {9, 8, 7, 6}; 
//byte colPins[columns] = {5, 4, 3, 2}; 

// For ESP8266 Microcontroller
byte rowPins[rows] = {D1, D2, D3, D4};  
byte colPins[columns] = {D5, D6, D7, D8}; 

// For ESP32 Microcontroller
//byte rowPins[rows] = {23, 22, 3, 21}; 
//byte colPins[columns] = {19, 18, 5, 17}; 

char keys[rows][columns] =
{
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},  
 {'*', '0', '#', 'D'},
};
void setup() {
  for (byte r=0; r<rows; r++) {
    pinMode(rowPins[r],INPUT_PULLUP);
    //pinMode(rowPins[r],OUTPUT);
  }
  for (byte c=0; c<columns; c++) {
    pinMode(colPins[c],OUTPUT);
    //pinMode(colPins[c],INPUT_PULLUP);
  }
  Serial.begin(115200);
}
void keyPad(){
  if(noKey == 16){
    readKey = true;
  }
  noKey = 0; 
  for(columnCounter = 0; columnCounter < columns ; columnCounter++){  
    scanColumn();
    for(rowCounter = 0; rowCounter < rows; rowCounter++){
      readRow();
      if(foundRow == true){
        keyValue = columnCounter + 4*rowCounter;
        rowfound = rowCounter;
        columnfound = columnCounter;
      }
    }
  }
  if(readKey == true && noKey == 15){
    Serial.println(keys[rowfound][columnfound]);// debug
    Serial.print(rowfound);// debug
    Serial.println(columnfound);// debug
  }
  readKey = false;//reset readKey 
  delay(debounce);//debounce
}
void scanColumn(){
    for (byte c=0; c<columns; c++) {
      digitalWrite(colPins[c], HIGH);//set all rows High
    }
  digitalWrite(colPins[columnCounter], LOW);
}
void readRow(){
  foundRow = digitalRead(rowPins[rowCounter]);// wrong varailable
  Serial.println(foundRow);
  if(foundRow == false){
    foundRow = true;
  }else{
    foundRow = false;
    noKey = noKey + 1;
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  keyPad();
}
