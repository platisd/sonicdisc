#include "SonicSensor.h"

SonicSensor::SonicSensor(uint8_t triggerPin, uint8_t echoPin) {
    mTriggerPin = triggerPin;
    mEchoPin = echoPin;
    mStartOfPulse = 0;
    mEndOfPulse = 0;
    mDistance = 0;
    mStartOfPulseNonVolatile = 0;
    mEndOfPulseNonVolatile = 0;
}

uint8_t SonicSensor::getTriggerPin() {
  return mTriggerPin;
}

uint8_t SonicSensor::getEchoPin() {
  return mEchoPin;
}

unsigned long SonicSensor::setStartOfPulse(unsigned long microseconds) {
    unsigned long previousStart = mStartOfPulse;
    mStartOfPulse = microseconds;
    return previousStart;
}

unsigned long SonicSensor::getStartOfPulse() {
    return mStartOfPulse;
}

unsigned long SonicSensor::setEndOfPulse(unsigned long microseconds) {
    unsigned long previousEnd = mEndOfPulse;
    mEndOfPulse = microseconds;
    return previousEnd;
}

unsigned long SonicSensor::getEndOfPulse() {
    return mEndOfPulse;
}

uint8_t SonicSensor::calculateDistance() {
    // If the echo is before the trigger, then
    // the ultrasonic beam has not yet returned
    // and the echo value (if any) is remnant of
    // a previous measurement.
    if (mEndOfPulseNonVolatile < mStartOfPulseNonVolatile) return 0;
    unsigned long pulseWidth = mEndOfPulseNonVolatile - mStartOfPulseNonVolatile;
    // Accordig to the HC-SR04 datasheet (https://goo.gl/b22dp3)
    // the formula to calculate the distance in centimeters is:
    // cm = uS / 58
    unsigned long distance = pulseWidth / 58;
    // According to the datasheet the minimum range is 2 cm
    if (distance < 2) {
        distance = 0; // Set it to 0 to indicate an out of range error
    } else if (distance > 255) {
        // Since we want to contain the measurement in an 8-bit byte
        // We have to constrain the measurements up to 255
        distance = 255;
    }
    mDistance = (uint8_t) distance;
    return mDistance;
}

void SonicSensor::prepareToCalculate() {
    mEndOfPulseNonVolatile = mEndOfPulse;
    mStartOfPulseNonVolatile = mStartOfPulse;
}

void SonicSensor::reset() {
    mEndOfPulse = 0;
    mStartOfPulse = 0;
}

uint8_t SonicSensor::getDistance() {
    return mDistance;
}
