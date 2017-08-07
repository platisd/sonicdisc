#include "SonicSensor.h"

SonicSensor sensors[] = {
    SonicSensor(PC3, PC2), // Ultrasonic_0 on the Sonic Disc
    SonicSensor(PC1, PC0), // Ultrasonic_1
    SonicSensor(PB5, PB4), // Ultrasonic_2
    SonicSensor(PB3, PB2), // Ultrasonic_3
    SonicSensor(PB0, PB1), // Ultrasonic_4
    SonicSensor(PD7, PD6), // Ultrasonic_5
    SonicSensor(PD4, PD5), // Ultrasonic_6
    SonicSensor(PD3, PD2)  // Ultrasonic_7
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
