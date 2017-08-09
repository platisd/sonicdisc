#include "SonicSensor.h"

SonicSensor::SonicSensor(uint8_t triggerPin, uint8_t echoPin) {
    mTriggerPin = triggerPin;
    mEchoPin = echoPin;
    mStartOfPulse = 0;
    mDistance = 0;
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
    unsigned long pulseWidth = mEndOfPulseNonVolatile - mStartOfPulse;
    // Accordig to the HC-SR04 datasheet (https://goo.gl/b22dp3)
    // the formula to calculate the distance in centimeters is:
    // cm = uS / 58
    uint8_t distance = pulseWidth / 58;
    // According to the datasheet the minimum range is 2 cm
    return distance < 2 ? 0 : distance;
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
