#ifndef SONIC_SENSOR_H
#define SONIC_SENSOR_H

class SonicSensor {
public:
    /**
     * The default constructor of a SonicSensor (HC-SR04 Ultrasonic sensor)
     * @param triggerPin The trigger pin of the ultrasonic sensor
     * @param echoPin    The echo pin of the ultrasonic sensor
     */
    SonicSensor(uint8_t triggerPin, uint8_t echoPin);

    /**
     * Gets the trigger pin of the ultrasonic sensor
     * @return The trigger pin of the ultrasonic sensor
     */
    uint8_t getTriggerPin();

    /**
     * Gets the echo pin of the ultrasonic sensor
     * @return The echo pin of the ultrasonic sensor
     */
    uint8_t getEchoPin();

    /**
     * Sets the trigger time of the ultrasonic sensor
     * @param  microseconds The time (in microseconds) the sensor was triggered
     * @return              The already existing trigger time
     */
    unsigned long setTriggerTime(unsigned long microseconds);

    /**
     * Gets the time that this sensor was triggered
     * @return the time (in microseconds) that this sensor was triggered
     */
    unsigned long getTriggerTime();
private:
    uint8_t mTriggerPin, mEchoPin;
    unsigned long mTriggeredAt;
};

#endif
