#include "SonicSensor.h"

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
