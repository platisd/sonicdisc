# SonicDisc
The firmware running on the Atmega328P of the SonicDisc.

## Overview
SonicDisc sends a trigger pulse to all the attached ultrasonic sensors and then uses interrupts to log down when the echo signals were received and the detected distances are calculated. If the echo has not arrived before the specified time, then the measurement is nulled.

The results of the measurements are being made available over the I2C interface when they are being sent as a package along with an error code. Additionally, a short pulse (3 μsec) is generated in order to notify other microcontrollers that new data are available.
