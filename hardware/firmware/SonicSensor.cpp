#include "SonicSensor.h"

SonicSensor::SonicSensor(uint8_t triggerPin, uint8_t echoPin) {
    mTriggerPin = triggerPin;
    mEchoPin = echoPin;
    mTriggerTime = 0;
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
uint8_t SonicSensor::getDistance() {
    //TO-DO calculate and return distance
    return 0;
}
