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
     * Sets the time that a beginning of an echo pulse was received.
     * @param  microseconds The time (in microseconds) the pulse started
     * @return              The already existing time
     */
    unsigned long setStartOfPulse(unsigned long microseconds);

    /**
     * Gets the time that the pulse started.
     * @return the time (in microseconds) the pulse started
     */
    unsigned long getStartOfPulse();

    /**
     * Sets the time that an end of an echo pulse was detected.
     * @param  microseconds The time (in microseconds) the echo pulse ended
     * @return              The already existing end time
     */
    unsigned long setEndOfPulse(unsigned long microseconds);

    /**
     * Gets the time that an end of an echo pulse was detected.
     * @return the time (in microseconds) the echo pulse ended
     */
    unsigned long getEndOfPulse();

    /**
     * Creates a copy of the (volatile) echo value so to calculate
     * the distance. This should be contained inside a critical section
     * and its purpose is use the non-volatile copies of the echo values
     * when calculating the distance. Subsequently, the calculation of
     * distance (which might be computationally intensive) does not have
     * to be contained inside a critical section.
     * MUST be called BEFORE calculateDistance().
     */
    void prepareToCalculate();

    /**
    * Calculates the distance using the start and end times of
    * a measurement. If the end has not yet come, then distance
    * is set to 0. MUST be called AFTER prepareToCalculate().
    * @return The calculated distance in centimeters or error code
     */
    uint8_t calculateDistance();

    /**
     * Resets the beginning and end of pulse times to designate the beginning
     * of a new measurement.
     * Since the echo time is set by an interrupt, this function should
     * be contained inside a critical section with interrupts disabled.
     * It also should be called before initiating a new interrupt-based
     * measurement.
     */
    void reset();

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
    unsigned long mStartOfPulseNonVolatile, mEndOfPulseNonVolatile;
    volatile unsigned long mStartOfPulse, mEndOfPulse;
};

#endif
