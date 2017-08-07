#include "SonicSensor.h"

// The Sonic Disc pin mapping of sensors
// Sonic Sensors are defined as SonicSensor(trigger pin, echo pin)
SonicSensor sensors[] = {
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
   Pin change interrupt for D8 to D13
*/
ISR (PCINT0_vect) {

}

/**
   Pin change interrupt for A0 to A5
*/
ISR(PCINT1_vect) {

}

/**
   Pin change interrupt for D0 to D7
*/
ISR (PCINT2_vect) {

}

void setupChangeInterrupt(uint8_t pin) {
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void setup() {
    setupChangeInterrupt(1);
}

void loop() {
  // put your main code here, to run repeatedly:

}
