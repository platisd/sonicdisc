#include "SonicSensor.h"

SonicSensor::SonicSensor(uint8_t triggerPin, uint8_t echoPin) {
    mTriggerPin = triggerPin;
    mEchoPin = echoPin;
    mTriggerTime = 0;
    mDistance = 0;
}

uint8_t SonicSensor::getTriggerPin() {
  return mTriggerPin;
}

uint8_t SonicSensor::getEchoPin() {
  return mEchoPin;
}

unsigned long SonicSensor::setStartTime(unsigned long microseconds) {
    unsigned long previousTrigger = mTriggerTime;
    mTriggerTime = microseconds;
    return previousTrigger;
}

unsigned long SonicSensor::getStartTime() {
    return mTriggerTime;
}

unsigned long SonicSensor::setFinishTime(unsigned long microseconds) {
    unsigned long previousEcho = mEchoTime;
    mEchoTime = microseconds;
    return previousEcho;
}

unsigned long SonicSensor::getFinishTime() {
    return mEchoTime;
}

uint8_t SonicSensor::calculateDistance() {
    // If the echo is before the trigger, then
    // the ultrasonic beam has not yet returned
    // and the echo value (if any) is remnant of
    // a previous measurement.
    if (mEchoTimeNonVolatile < mTriggerTime) return 0;
    unsigned long pulseWidth = mEchoTimeNonVolatile - mTriggerTime;
    // Accordig to the HC-SR04 datasheet (https://goo.gl/b22dp3)
    // the formula to calculate the distance in centimeters is:
    // cm = uS / 58
    uint8_t distance = pulseWidth / 58;
    // According to the datasheet the minimum range is 2 cm
    return distance < 2 ? 0 : distance;
}

void SonicSensor::prepareToCalculate() {
    mEchoTimeNonVolatile = mEchoTime;
}

void SonicSensor::resetEcho() {
    mEchoTime = 0;
}

uint8_t SonicSensor::getDistance() {
    return mDistance;
}
