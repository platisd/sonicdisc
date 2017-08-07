#include <stdint.h>
#include <Wire.h>
#include "SonicSensor.h"

const uint8_t NUM_OF_SENSORS = 8; // The number of sonic sensors on the disc
const uint8_t I2C_PACKET_SIZE = 9; // The size of each I2C transmission in bytes
const uint8_t SONIC_DISC_I2C_ADDRESS = 0x09; // The address to assume as an I2C slave

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

// The pin to trigger an interrupt signal on the master MCU
// to indicate that a measurement is ready to be transmitted
const uint8_t INT_PIN = 0; // Note that this is also the RX pin
// The pin connected to the on-bard LED for debugging
const uint8_t LED_PIN = 1; // Note that this is also the TX pin


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
 * Transmits the collected measurements via I2C.
 * A packet of I2C_PACKET_SIZE bytes is sent upon each request. The first byte stands
 * for the error code, while the rest are the values estimated using
 * the ultrasonic sensor readings. The order of the readings is clockwise.
 * For example, a typical package looks like this:
 * |error|us_0|us_1|us_2|us_3|us_4|us_5|us_6|us_7|
 *
 * Error code values:   0       | no error
 *                      1       | incomplete measurement
 * Sensor values:       0       | error (e.g. ping timeout)
 *                      1       | TBD
 *                      2-255   | Valid measurement (in cm)
 */
void sendMeasurements() {
    uint8_t packet[I2C_PACKET_SIZE] = {0};
    // Send packet via I2C
    Wire.write(packet, I2C_PACKET_SIZE);
}

void setup() {
    // Set up change interrupts for all the echo pins
    for (int i = 0; i < NUM_OF_SENSORS; i++){
        setupChangeInterrupt(sensors[i].getEchoPin());
    }
    // Set up the interrupt signal pin
    pinMode(INT_PIN, OUTPUT);
    // Set up the I2C bus and join it as a slave with address 9
    Wire.begin(SONIC_DISC_I2C_ADDRESS);
    // Set up callback for I2C requests
    Wire.onRequest(sendMeasurements);
}

void loop() {
  // put your main code here, to run repeatedly:

}
