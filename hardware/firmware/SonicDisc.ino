#include <stdint.h>
#include <Wire.h>
#include "SonicSensor.h"

const uint8_t NUM_OF_SENSORS = 8; // The number of sonic sensors on the disc
const uint8_t I2C_PACKET_SIZE = 9; // The size of each I2C transmission in bytes
const uint8_t SONIC_DISC_I2C_ADDRESS = 0x09; // The address to assume as an I2C slave
// The pin to trigger an interrupt signal on the master MCU
// to indicate that a measurement is ready to be transmitted
const uint8_t INT_PIN = 0; // Note that this is also the RX pin
// The pin connected to the on-bard LED for debugging
const uint8_t LED_PIN = 1; // Note that this is also the TX pin

// Sonic Disc's operational states
enum State {
    STANDBY, // MCU and sensors are on but no measurements are being made
    MEASURING // Sonic Disc is conducting measurements using the sensors
};

volatile State currentState = STANDBY;

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

// The Sonic Disc pin mapping of sensors
// Sonic Sensors are defined as SonicSensor(trigger pin, echo pin)
SonicSensor sensors[NUM_OF_SENSORS] = {
    SonicSensor(A3, A2),  // Ultrasonic_0 on the Sonic Disc
    SonicSensor(A1, A0),  // Ultrasonic_1
    SonicSensor(13, 12),  // Ultrasonic_2
    SonicSensor(11, 10),  // Ultrasonic_3
    SonicSensor(8, 9),    // Ultrasonic_4
    SonicSensor(7, 6),    // Ultrasonic_5
    SonicSensor(4, 5),    // Ultrasonic_6
    SonicSensor(3, 2)     // Ultrasonic_7
};

/**
 * Hook for pin change interrupt of PCINT0 vector
 * Pins: D8 to D13
 */
ISR (PCINT0_vect) {

}

/**
 * Hook for pin change interrupt of PCINT1 vector
 * Pins: A0 to A5
 */
ISR(PCINT1_vect) {

}

/**
 * Hook for pin change interrupt of PCINT2 vector
 * Pins: D0 to D7
 */
ISR (PCINT2_vect) {

}

/**
 * Method to set up change interrupt for the specified pin
 * @param pin The Arduino-like name of the pin
 */
void setupChangeInterrupt(uint8_t pin) {
    pinMode(pin, INPUT);
    *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));  // Enable interrupt for pin
    PCIFR |= bit(digitalPinToPCICRbit(pin)); // Clear any outstanding interrupt
    PCICR |= bit(digitalPinToPCICRbit(pin)); // Enable interrupt for the group
}

/**
 * Callback for incoming I2C requests by transmiting the collected measurements.
 * A packet of I2C_PACKET_SIZE bytes is sent upon each request. The first byte stands
 * for the error code, while the rest are the values estimated using
 * the ultrasonic sensor readings. The order of the readings is clockwise.
 * For example, a typical package looks like this:
 * |error|us_0|us_1|us_2|us_3|us_4|us_5|us_6|us_7|
 *
 * Error code values:   NO_ERROR                | No error
 *                      INCOMPLETE_MEASUREMENT  | Incomplete measurement
 *                      STANDBY                 | In standby state
 *
 * Sensor values:       0       | Error in measurement (e.g. ping timeout)
 *                      1       | TBD
 *                      2-255   | Valid measurement (in cm)
 */
void handleRequests() {
    uint8_t packet[I2C_PACKET_SIZE] = {0};
    // Send packet via I2C
    Wire.write(packet, I2C_PACKET_SIZE);
}

/**
 * Callback for incoming I2C receptions of data from the master as illustrated below.
 *
 * Request              |   Action
 * STATE_TO_STANDBY         Set state to standby (stop measurements)
 * STATE_TO_MEASURING       Start measuring
 *
 * @param numOfBytes The number of bytes expected to be received
 */
void handleReceipts(int numOfBytes) {
    if (Wire.available()) {
        char masterCommand = Wire.read();
        switch (masterCommand) {
            case STATE_TO_STANDBY:
                currentState = STANDBY;
                break;
            case STATE_TO_MEASURING:
                currentState = MEASURING;
            default:
                break;
        }
    }
}

/**
 * Run once on boot or after a reset
 */
void setup() {
    // Set up ultrasonic sensor pins
    for (int i = 0; i < NUM_OF_SENSORS; i++){
        // Set up change interrupts for all the echo pins
        setupChangeInterrupt(sensors[i].getEchoPin());
        // Set up all trigger pins as outputs
        pinMode(sensors[i].getTriggerPin(), OUTPUT);
    }
    // Set up the interrupt signal and led pins
    pinMode(INT_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    // Set up the I2C bus and join it as a slave with address 9
    Wire.begin(SONIC_DISC_I2C_ADDRESS);
    // Set up callback for I2C requests
    Wire.onRequest(handleRequests);
    // Set up callback for I2C receipts
    Wire.onReceive(handleReceipts);
}

/**
 * Run continuously after setup()
 */
void loop() {
    switch(currentState) {
        case STANDBY:
            break;
        case MEASURING:
            break;
        default:
            break; // We should not get here
    }
}
