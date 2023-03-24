#include <Keypad.h>
#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;

const byte ROWS = 4; // number of rows on the keypad
const byte COLS = 3; // number of columns on the keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
}; // define the character values for each key

byte rowPins[ROWS] = {A4, A3, A2, A1}; // connect the row pins of the keypad to these pins
byte colPins[COLS] = {5, 4, 3}; // connect the column pins of the keypad to these digital pins

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// RadioHead library settings
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
//#define RFM95_CS 10
//#define RFM95_RST 9
//#define RFM95_INT 2
#define RF95_FREQ 915.0

//RH_RF95 rf95(RFM95_CS, RFM95_INT);

char buffer[100]; // buffer to store the text entered by the user
int bufferIndex = 0; // index of the next available position in the buffer




void setup() {
   Serial.begin(9600); // initialize serial communication
  
  if (!rf95.init()) {
    Serial.println("RF95 radio init failed");
    while (1);
  }
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  
  rf95.setTxPower(23, false); // set the transmit power to 23 dBm
  Serial.println("RF95 ready.");
}

//  Serial.begin(9600);
//  
//  if (!rf95.init()) {
//    Serial.println("RF95 init failed. Check your connections.");
//    while (true); // halt
//  }
//  Serial.println("RF95 ready.");
//}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    static char lastKey = NO_KEY;
    static unsigned long lastKeyPressTime = 0;
    unsigned long now = millis();

    if (key == lastKey) {
      // the same key was pressed again, so output the next character
      int numTaps = (now - lastKeyPressTime) / 250; // determine how many times the key was tapped
      char output = getMultiTapOutput(key, numTaps);

      buffer[bufferIndex++] = output; // add the output to the buffer
      Serial.print(output);       
      
    }
    
    if (bufferIndex > 0 && keypad.isPressed('#')) { // check if the send button is pressed
        buffer[bufferIndex] = '\0'; // add the null terminator to the end of the buffer
        Serial.print("Sending message: ");
        Serial.println(buffer);
        rf95.send((uint8_t *)buffer, strlen(buffer)); // send the message to the server node
        rf95.waitPacketSent(); // wait for the message to be sent
        bufferIndex = 0; // reset the buffer index to start over
      }
    lastKey = key;
    lastKeyPressTime = now;
  }
}

char getMultiTapOutput(char key, int numTaps) {
  switch (key) {
    case '2':
      return "ABC"[numTaps % 3];
    case '3':
      return "DEF"[numTaps % 3];
    case '4':
      return "GHI"[numTaps % 3];
    case '5':
      return "JKL"[numTaps % 3];
    case '6':
      return "MNO"[numTaps % 3];
    case '7':
      return "PQRS"[numTaps % 4];
    case '8':
      return "TUV"[numTaps % 3];
    case '9':
      return "WXYZ"[numTaps % 4];
    case '0':
      return " "[numTaps % 1];
    default:
      return key;
  }
}
