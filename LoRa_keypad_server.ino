// ---------------
// |Server  Code:|
// ---------------

#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to connect
  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
  rf95.setFrequency(915.0);
  rf95.setTxPower(23, false);
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len)) {
//      Serial.print("Received from client: ");
      Serial.print((char*)buf);
    }
  }
}
