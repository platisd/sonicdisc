#include <Wire.h>
#include <stdint.h>

const uint8_t SONIC_DISC_I2C_ADDRESS = 0x09;
const uint8_t I2C_PACKET_SIZE = 9;
const uint8_t INT_PIN = 2;

// Sonic Disc's operational states
enum State {
  STANDBY, // MCU and sensors are on but no measurements are being made
  MEASURING // Sonic Disc is conducting measurements using the sensors
};

// Values to be received via I2C from master
enum I2C_RECEIPT_CODE {
  STATE_TO_STANDBY = 0x0A,
  STATE_TO_MEASURING = 0x0B
};

// Error codes to be transmitted via I2c to the master
enum I2C_ERROR_CODE {
  NO_ERROR,
  IN_STANDBY,
  INCOMPLETE_MEASUREMENT
};

// Flag to indicate the SonicDisc is ready to send a new set of data
volatile bool newData = false;

I2C_ERROR_CODE requestPacket(uint8_t i2cInput[], const uint8_t transmissionSize) {
  Wire.requestFrom(SONIC_DISC_I2C_ADDRESS, transmissionSize);
  uint8_t packetIndex = 0;
  while (Wire.available() && packetIndex < 9) {
    i2cInput[packetIndex++] = Wire.read();
  }
  return i2cInput[0]; // Return the packet's error code
}

void sendData(uint8_t byteToSend) {
  Wire.beginTransmission(SONIC_DISC_I2C_ADDRESS);
  Wire.write(byteToSend);
  Wire.endTransmission(SONIC_DISC_I2C_ADDRESS);
}

void newSonicDiscData() {
  newData = true;
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), newSonicDiscData, RISING);
  Serial.println("Requesting packet from SonicDisc");
  uint8_t dummyInput[I2C_PACKET_SIZE] = {0};
  while (requestPacket(dummyInput, I2C_PACKET_SIZE) == IN_STANDBY) {
    Serial.println("Setting state to MEASURING");
    sendData(STATE_TO_MEASURING);
  }
  Serial.println("Communication is established and Sonic Disc is measuring distances");
}

void loop() {
  if (newData) {
    newData = false; // Indicate that we have read the latest data

    uint8_t sonicDiscInput[I2C_PACKET_SIZE] = {0};
    requestPacket(sonicDiscInput, I2C_PACKET_SIZE);
    for (int i = 0; i < I2C_PACKET_SIZE; i++) {
      Serial.print(" |");
      Serial.print(sonicDiscInput[i]);
      Serial.print(" | ");
    }
    Serial.println();
  }
}
