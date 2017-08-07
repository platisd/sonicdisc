#include "SonicSensor.h"

SonicSensor::SonicSensor(uint8_t triggerPin, uint8_t echoPin) {
    mTriggerPin = triggerPin;
    mEchoPin = echoPin;
    mTriggeredAt = 0;
}

uint8_t SonicSensor::getTriggerPin() {
  return mTriggerPin;
}

uint8_t SonicSensor::getEchoPin() {
  return mEchoPin;
}

unsigned long SonicSensor::setTriggerTime(unsigned long microseconds) {
    unsigned long previousTrigger = mTriggeredAt;
    mTriggeredAt = microseconds;
    return previousTrigger;
}

unsigned long SonicSensor::getTriggerTime() {
    return mTriggeredAt;
}
