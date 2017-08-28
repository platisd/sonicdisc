# SonicDisc
A 360° ultrasonic scanner that talks over I2C using an Atmega328P and eight HC-SR04 ultrasonic sensors.

![sonicdisc](http://i.imgur.com/tKw5zAx.jpg)

## Overview
SonicDisc is a sensor that allows you to sense your surroundings. It utilizes 8 ultrasound sensors (HC-SR04) placed at 45° angle from each other, that can measure distances from practically every direction. It can be used in hobby-grade automotive applications, e.g. autonomous driving, obstacle detection etc or wherever quickly determining surrounding distances is relevant.

The sensor transmits the collected data over I2C and uses a separate line to signal, with a short pulse, the existence of a new set of measurements. It can be mounted on top of an Arduino UNO or used standalone. Due to the nature of the HC-SR04 sensors, the measurements can be noisy, therefore it is suggested that they are filtered. A basic Arduino example that receives and filters the incoming data can be found in [examples/SonicDiscReader](examples/SonicDiscReader).

You can find the software running on the SonicDisc in [firmware/](firmware/), the PCB design in Eagle CAD format inside [hardware/](hardware/eagle) and more sample applications using the SonicDisc in [examples](examples/).

Read the story behind the module at [platis.solutions](https://platis.solutions/blog/2017/08/27/sonicdisc-360-ultrasonic-scanner/).

## How does it work
SonicDisc is comprised of an Atmega328P microcontroller that triggers all the ultrasound sensors at the same time, receives the echo pulses via pin change interrupts and then proceeds to calculate the detected distances. The use of interrupts, in contrast to how the HC-SR04 sensors are typically utilized, enable us to conduct all measurements at the "same" time which translates into new sets of `8` measurements every `10 milliseconds`.

As an I2C slave, whenever a new set of measurements is ready to be transmitted, a short pulse is sent over the `INT` pin, prompting the master to request the data. In other words the I2C master reading the data from the SonicDisc, does not have to poll but merely wait for the indicative signal and then make the I2C request. Despite not suggested, it is possible to use SonicDisc without connecting the `INT` pin to an interrupt, however the master will not be able to assure the existence of a new data.

## How to connect
| SonicDisc | Host MCU  |
| :----:    |:----:     |
| VCC       |  5V       |
| GND       |  GND      |
| SDA       |  SDA      |
| SCL       |  SCL      |
| INT       |  ISR      |

If you are mounting the SonicDisc on top of an Arduino UNO as a shield, you only have to (optionally but suggested) connect the `INT` signal to an ISR pin.

## I2C protocol
SonicDisc communicates using the I2C bus as a slave, meaning that it receives data from a master and responds to data requests.

### Address
SonicDisc's I2C address is currently hardcoded to `0x09`.

### Set operational state
SonicDisc has two operational states between which you can switch.

| **Action**                    | **Byte to send**  | **Notes**                                   |
| :-----------:                 |:-------------:    | :----:                                      |
| Set state to `STANDBY`        | 0x0A              | SonicDisc's initial state                   |
| Set state to `MEASURING`      | 0x0B              | Set to this state to start the measurements |

**Arduino example:**
```arduino
Wire.beginTransmission(0x09);
Wire.write(0x0B); // Instruct SonicDisc to start measuring
Wire.endTransmission(0x09);
```
### Get data
SonicDisc responds to I2C requests with packages of `9` bytes that include an error code and sensor measurements. A typical package structure is described below.

Regarding the error codes, when the first byte is a `0` then everything went well and the trailing measurements are valid. If it is `1`, it means that this set of data has already been transmitted and a measurement is currently under way. You will encounter this if you poll the sensor fast enough, without taking into consideration the `INT` signal which indicates SonicDisc is ready to transmit new data. Finally, `2` designates that the module is in standby state and no measurements are being conducted.

| **Byte Index** | **Range** | **Purpose** | **Notes**                                                               |
| :----:         |:----:   | :----:        | :----:                                                                  |
| 0              |  0-2    | Error code    | **0**: No error, **1**: Incomplete measurement, **2**: In standby state |
| 1              |  0-255  | Ultrasonic 0  | **0**: Error in measurement, **1**: TBD, **2-255**: Valid distance (cm) |
| 2              |  0-255  | Ultrasonic 1  | **0**: Error in measurement, **1**: TBD, **2-255**: Valid distance (cm) |
| 3              |  0-255  | Ultrasonic 2  | **0**: Error in measurement, **1**: TBD, **2-255**: Valid distance (cm) |
| 4              |  0-255  | Ultrasonic 3  | **0**: Error in measurement, **1**: TBD, **2-255**: Valid distance (cm) |
| 5              |  0-255  | Ultrasonic 4  | **0**: Error in measurement, **1**: TBD, **2-255**: Valid distance (cm) |
| 6              |  0-255  | Ultrasonic 5  | **0**: Error in measurement, **1**: TBD, **2-255**: Valid distance (cm) |
| 7              |  0-255  | Ultrasonic 6  | **0**: Error in measurement, **1**: TBD, **2-255**: Valid distance (cm) |
| 8              |  0-255  | Ultrasonic 7  | **0**: Error in measurement, **1**: TBD, **2-255**: Valid distance (cm) |

**Arduino example:**

```arduino
const unsigned int PACKET_SIZE = 9;
unsigned int packet[PACKET_SIZE] = {0};
unsigned int packetIndex = 0;
Wire.requestFrom(0x09, PACKET_SIZE);
while (Wire.available() && packetIndex < PACKET_SIZE) {
    i2cInput[packetIndex++] = Wire.read();
}
// Now the packet[] array holds the incoming data from SonicDisc
```

## Build one yourself
Below you can find the necessary components to build a SonicDisc yourself:

* [SonicDisc PCB](https://www.pcbway.com/project/shareproject/SonicDisc___A_360__ultrasonic_scanner.html)
* 8 x HC-SR04
* 1 x Atmega328P-PU
* 2 x 22pF capacitors
* 3 x 0.1uF capacitors
* 1 x 1206 LED
* 1 x 1206 220Ω resistor
* 3 x 4.7KΩ resistor
* 1 x 5-pin header
* 1 x 4-pin header
* 1 x tactile switch
* 1 x 16MHz oscillator
* 1 x ISCP pin header
* 1 x 10-pin stackable header
* 2 x 8-pin stackable header
* 1 x 6-pin stackable header
