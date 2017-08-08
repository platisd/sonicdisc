#ifndef SONIC_SENSOR_H
#define SONIC_SENSOR_H

#include <stdint.h>

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
     * Sets the trigger time of the ultrasonic sensor. Called when
     * a measurement begins.
     * @param  microseconds The time (in microseconds) the sensor was triggered
     * @return              The already existing trigger time
     */
    unsigned long setStartTime(unsigned long microseconds);

    /**
     * Gets the time that this sensor was triggered and the measurement started.
     * @return the time (in microseconds) that this sensor was triggered
     */
    unsigned long getStartTime();

    /**
     * Sets the echo time of the ultrasonic sensor. Called when
     * the measurement finishes.
     * @param  microseconds The time (in microseconds) the sensor got an echo
     * @return              The already existing echo time
     */
    unsigned long setFinishTime(unsigned long microseconds);

    /**
     * Gets the time that this sensor received an echo and
     * the measurement finished.
     * @return the time (in microseconds) that this sensor received an echo
     */
    unsigned long getFinishTime();

    /**
    * Calculates the distance using the start and end times of
    * a measurement. If the end has not yet come, then distance
    * is set to 0.
    * @return The calculated distance in centimeters or error code
     */
    uint8_t calculateDistance();

    /**
     * Resets the distance to designate the beginning of a new one.
     * @return The previous value of the distance
     */
    uint8_t resetDistance();

    /**
     * Returns the already calculated distance in cm.
     * It should be used in an ISR to quickly return a value in conjuction with
     * calculateDistance(). In case of a timeout (e.g. ultrasonic beam taking
     * too long to return) or detection of an invalid value an error code (0)
     * is returned.
     * @return Distance in centimeters or error code (0)
     */
    uint8_t getDistance();
private:
    uint8_t mTriggerPin, mEchoPin, mDistance;
    unsigned long mTriggerTime, mEchoTime;
};

#endif
