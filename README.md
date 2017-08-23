# SonicDisc
A 360° ultrasonic scanner that talks over I2C using an Atmega328P and 8 HC-SR04 ultrasonic sensors.

![sonicdisc](http://i.imgur.com/tKw5zAx.jpg)

## Overview
SonicDisc is a sensor that allows you to sense your surroundings. It utilizes 8 ultrasound sensors (HC-SR04) placed at 45° angle from each other, that can measure distances from practically every direction. It can be used in hobby-grade automotive applications, e.g. autonomous driving, obstacle detection etc or wherever quickly determining surrounding distances is relevant.

The sensor transmits the collected data over I2C and uses a separate line to signal, with a short pulse, the existence of a new set of measurements. It can be mounted on top of an Arduino UNO or used standalone. Due to the nature of the HC-SR04 sensors, the measurements can be noisy, therefore it is suggested that they are filtered. An example that receives and filters the incoming data can be found in [examples/SonicDiscReader](examples/SonicDiscReader).

You can find the software running on the SonicDisc in [firmware/](firmware/) and the PCB design in Eagle CAD format inside [hardware/](hardware/eagle).

## How does it work
SonicDisc is comprised of an Atmega328P microcontroller that triggers all the ultrasound sensors at the same time, receives the echo pulses via pin change interrupts and then proceeds to calculate the detected distances. The use of interrupts, in contrast to how the HC-SR04 sensors are typically utilized, enable us to conduct all measurements at the "same" time which translates into new sets of `8` measurements every `10 milliseconds`.

As an I2C slave, whenever a new set of measurements is ready to be transmitted, a short pulse is sent over the `INT` pin, prompting the master to request the data. In other words the I2C master reading the data from the SonicDisc, does not have to poll but merely wait for the indicative signal and then make the I2C request. Despite not suggested, it is possible to use SonicDisc without connecting the `INT` pin to an interrupt, however the master will not be able to assure the existence of a new data.

## How to connect
**SonicDisc** <----------> **Host MCU**

VCC <------------------> 5V

GND <------------------> GND

SDA <------------------> SDA

SCL <------------------> SCL

INT <------------------> ISR

If you are mounting the SonicDisc on top of an Arduino UNO as a shield, you only have to (optionally but suggested) connect the `INT` signal to an ISR pin.

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
