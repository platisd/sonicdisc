#include <stdint.h>
#include <Wire.h>
#include "SonicSensor.h"

const uint8_t NUM_OF_SENSORS = 8; // The number of sonic sensors on the disc
const uint8_t I2C_PACKET_SIZE = 9; // The size of each I2C transmission in bytes
const uint8_t SONIC_DISC_I2C_ADDRESS = 0x09; // The address to assume as an I2C slave
// The pin to trigger an interrupt signal on the master MCU
// to indicate that a measurement is ready to be transmitted.
// It is set HIGH when there are data to be fetched and LOW otherwise.
const uint8_t INT_PIN = 0; // Note that this is also the RX pin
// The pin connected to the on-bard LED for debugging
const uint8_t LED_PIN = 1; // Note that this is also the TX pin
// How often the measurements should take place (in milliseconds)
const unsigned long MEASUREMENT_INTERVAL = 10;
// The time (in milliseconds) that the last measurement took place
unsigned long previousMeasurement = 0;
// Sonic Disc's operational states
enum State {
    STANDBY, // MCU and sensors are on but no measurements are being made
    MEASURING // Sonic Disc is conducting measurements using the sensors
};

volatile State currentState = STANDBY;

// Values to be received via I2C from master
enum I2C_RECEIPT_CODE {
    STATE_TO_STANDBY = 0x0A,
    STATE_TO_MEASURING = 0x0B
};

// Error codes to be transmitted via I2c to the master
enum I2C_ERROR_CODE {
    NO_ERROR,
    IN_STANDBY,
    INCOMPLETE_MEASUREMENT
};

// ISR vectors
enum Vector {
    VECTOR_0, // PCINT0_vect
    VECTOR_1, // PCINT1_vect
    VECTOR_2 // PCINT2_vect
};

// The Sonic Disc pin mapping of sensors
// Sonic Sensors are defined as SonicSensor(trigger pin, echo pin)
SonicSensor sensors[NUM_OF_SENSORS] = {
    SonicSensor(A3, A2),  // Ultrasonic_0 on the Sonic Disc
    SonicSensor(A1, A0),  // Ultrasonic_1
    SonicSensor(13, 12),  // Ultrasonic_2
    SonicSensor(11, 10),  // Ultrasonic_3
    SonicSensor(8, 9),    // Ultrasonic_4
    SonicSensor(7, 6),    // Ultrasonic_5
    SonicSensor(4, 5),    // Ultrasonic_6
    SonicSensor(3, 2)     // Ultrasonic_7
};

/**
 * Handles the echo signal from an ultrasonic sensor.
 * To be called from within an interrupt, it checks all the echo
 * pins to see if any of them is HIGH or LOW and updates the echo
 * pulse times of the respective sensor.
 * If a pulse has already been set, then the new value is ignored.
 * @param intVector The vector that the interrupt originated from
 */
void handleEcho(Vector intVector) {
    // Determine which vector the interrupt originated from
    // so we only check signals from those specific sensors
    int sensorsInVector[3] = {0}; // We have up to 3 sensors in each vector
    switch (intVector) {
        case VECTOR_0:
            // Ultrasonics 2, 3, 4 are on PCINT0_vect
            sensorsInVector[0] = 2;
            sensorsInVector[1] = 3;
            sensorsInVector[2] = 4;
            break;
        case VECTOR_1:
            // Ultrasonics 0, 1 are on PCINT1_vect
            sensorsInVector[0] = 0;
            sensorsInVector[1] = 1;
            sensorsInVector[2] = 1; // For simplicity
            break;
        case VECTOR_2:
            // Ultrasonics 5, 6, 7 are on PCINT2_vect
            sensorsInVector[0] = 5;
            sensorsInVector[1] = 6;
            sensorsInVector[2] = 7;
            break;
        default:
            break; // We should not be here
    }

    // Iterate through the specific vector's ultrasonic echo pins
    for (int i : sensorsInVector) {
        // If a pin is HIGH, it means that a pulse
        // is either just starting or has previously started.
        // We only care about the former.
        if (digitalRead(sensors[i].getEchoPin()) == HIGH) {
            // We only care for newly generated pulses and not ones
            // we have handled before.
            if (sensors[i].getStartOfPulse() == 0) {
                sensors[i].setStartOfPulse(micros());
            }
        } else {
            // If a pin is LOW, it means that a pulse has just ended,
            // has already ended or not started. We only care about
            // the first case. We can determine this by pulses which
            // we have not handled before AND that have already started
            if (sensors[i].getEndOfPulse() == 0 && sensors[i].getStartOfPulse() != 0) {
                sensors[i].setEndOfPulse(micros());
            }
        }
    }
}

/**
 * Hook for pin change interrupt of PCINT0 vector
 * Pins: D8 to D13
 */
ISR (PCINT0_vect) {
    handleEcho(VECTOR_0);
}

/**
 * Hook for pin change interrupt of PCINT1 vector
 * Pins: A0 to A5
 */
ISR(PCINT1_vect) {
    handleEcho(VECTOR_1);
}

/**
 * Hook for pin change interrupt of PCINT2 vector
 * Pins: D0 to D7
 */
ISR (PCINT2_vect) {
    handleEcho(VECTOR_2);
}

/**
 * Method to set up change interrupt for the specified pin
 * @param pin The Arduino-like name of the pin
 */
void setupChangeInterrupt(uint8_t pin) {
    pinMode(pin, INPUT);
    // Enable interrupt for pin
    *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));
    // Clear any outstanding interrupt
    PCIFR |= bit(digitalPinToPCICRbit(pin));
    // Enable interrupt for the pin's group
    PCICR |= bit(digitalPinToPCICRbit(pin));
}

/**
 * Callback for incoming I2C requests by transmiting the collected measurements.
 * A packet of I2C_PACKET_SIZE bytes is sent upon each request. The first byte stands
 * for the error code, while the rest are the values estimated using
 * the ultrasonic sensor readings. The order of the readings is clockwise.
 * For example, a typical package looks like this:
 * |error|us_0|us_1|us_2|us_3|us_4|us_5|us_6|us_7|
 *
 * Error code values:   NO_ERROR                | No error
 *                      INCOMPLETE_MEASUREMENT  | Incomplete measurement
 *                      STANDBY                 | In standby state
 *
 * Sensor values:       0       | Error in measurement (e.g. ping timeout)
 *                      1       | TBD
 *                      2-255   | Valid measurement (in cm)
 */
void handleRequests() {
    uint8_t packet[I2C_PACKET_SIZE] = {0};
    //TO-DO: Compose packet to be sent
    // Send packet via I2C
    Wire.write(packet, I2C_PACKET_SIZE);
}

/**
 * Callback for incoming I2C receptions of data from the master as illustrated below.
 *
 * Request              |   Action
 * STATE_TO_STANDBY         Set state to standby (stop measurements)
 * STATE_TO_MEASURING       Start measuring
 *
 * @param numOfBytes The number of bytes expected to be received
 */
void handleReceipts(int numOfBytes) {
    if (Wire.available()) {
        char masterCommand = Wire.read();
        switch (masterCommand) {
            case STATE_TO_STANDBY:
                currentState = STANDBY;
                break;
            case STATE_TO_MEASURING:
                currentState = MEASURING;
            default:
                break;
        }
    }
}

/**
 * Check to see if it is OK to start a new cycle of measurements
 * based on the current time and the measurement interval.
 * @param  currentTime The current time in milliseconds to be compared
 *                     with the last time that a measurement took place.
 * @return             True if is time to conduct a new measurement and
 *                     False otherwise
 */
bool isTimeToMeasure(unsigned long currentTime) {
    bool isGoodTimeToMeasure = false;
    if (currentTime - previousMeasurement >= MEASUREMENT_INTERVAL) {
        isGoodTimeToMeasure = true;
        previousMeasurement = currentTime;
    }
    return isGoodTimeToMeasure;
}

/**
 * Triggers all sensors at once, using port manipulation for less
 * computation cycles. This is done by sending a pulse with a width
 * of 10 microseconds.
 */
void triggerSensors() {
    // Set all ultrasonic trigger pins to HIGH at the same time
    // Port B handles D8 to D13
    PORTB |= B00101001; // Set pins 8, 11, 13 HIGH
    // Port C handles A0 to A5
    PORTC |= B00001010; // Set pins A1, A3 HIGH
    // Port D handles D0 to D7
    PORTD |= B10011000; // Set pins D3, D4, D7 HIGH

    // Keep the signal HIGH for 10 microseconds
    delayMicroseconds(10);

    // Set the trigger pins back to LOW
    PORTB &= B11010110; // Set pins 8, 11, 13 LOW
    PORTC &= B11110101; // Set pins A1, A3 LOW
    PORTD &= B01100111; // Set pins D3, D4, D7 LOW
}

/**
 * Run once on boot or after a reset
 */
void setup() {
    // Set up ultrasonic sensor pins
    for (int i = 0; i < NUM_OF_SENSORS; i++){
        // Set up change interrupts for all the echo pins
        setupChangeInterrupt(sensors[i].getEchoPin());
        // Set up all trigger pins as outputs
        pinMode(sensors[i].getTriggerPin(), OUTPUT);
    }
    // Set up the interrupt signal and led pins
    pinMode(INT_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    // Set up the I2C bus and join it as a slave with address 9
    Wire.begin(SONIC_DISC_I2C_ADDRESS);
    // Set up callback for I2C requests
    Wire.onRequest(handleRequests);
    // Set up callback for I2C receipts
    Wire.onReceive(handleReceipts);
}

/**
 * Run continuously after setup()
 */
void loop() {
    switch(currentState) {
        case STANDBY:
            break;
        case MEASURING:
            if (isTimeToMeasure(millis())) {
                // Disable the interrupts so we can prepare to calculate
                // the distances and then we are ready to start a new
                // set of measurements.
                noInterrupts();             // Begin critical section
                for (int i = 0; i < NUM_OF_SENSORS; i++) {
                    // Copy the (volatile) start and end times of the pulses
                    // so that we can begin immediately measuring again,
                    // while distances of the (now previous) measurements are
                    // being calculated.
                    sensors[i].prepareToCalculate();
                    // Then reset previous the echoes so we can
                    // conduct new measurements
                    sensors[i].reset();
                }
                // Finally start a new set of measurements
                // by triggering all sensors at once
                triggerSensors();
                interrupts();               // End critical section

                // Now that we are certain that our measurements are consistent
                // time-wise, calculate the distance.
                for (int i = 0; i < NUM_OF_SENSORS; i++) {
                    // Calculate distance for each sensor.
                    // Will also timeout any pending measurements
                    sensors[i].calculateDistance();
                }
                // Signal that we have a new set of measurements
                digitalWrite(INT_PIN, HIGH);
            }
            break;
        default:
            break; // We should not get here
    }
}
