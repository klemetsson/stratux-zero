# Stratux Zero

Portable ADS-B/Flarm receiver, AHRS, GNSS and CO-alarm based on the Stratux project and Raspberry Pi Zero 2.

Note that this does not replace any primary systems and should only be used as a complement for increased safety and situational awareness.

> THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

## Features

- Dual USB 2.0 receptacles for [NooElec NESDR Nano 2+](https://www.nooelec.com/store/nesdr-nano2.html) or similar (USB2512)
- Pressure altimeter (BMP280)
- 9-axis motion tracking (ICM-20948)
- GNSS (GPS, Galileo, GLONASS, BeiDou) with 2.5 m CEP accuracy (CAM-M8Q)
- GNSS backup power for faster initial lock
- Carbon monoxide detector with audible alarm (MICS-4514)
- 18650 battery socket
- 4.2 V overvoltage, 2.5 V undervoltage and over-current protection
- USB C power with built-in 1 A charger with support for USB BC1.2, Apple, Samsung and legacy USB charge adapters
- High temperature and low temperature charge monitoring
- Fuel gauge that monitors the battery state of charge and aging
- Hard/soft power switch that first signals the Raspberry to shutdown cleanly and if that takes to long, power will be cut
- Raspberry Pi Zero 2 interface with pogo pins
- Watchdog that resets the Raspberry Pi
- Five LED indicators
    - Green power indicator that flashes if low battery
    - Green Stratux status indicator that flashes if starting up or shutting down
    - Red Carbon monoxide alert that flashes every 5 seconds if detected
    - Green external power indicator
    - Orange charge indicator that flashes while charging and turned on when fully charged

> CO-alarm, watchdog, charging temperature monitoring, battery and power management is handled by an Silicon Labs EFM8 8051 MCU.

## Contents of this repository

- CAD and CAM files for the custom PCB
- CAD files for the enclosure
- Source code for the EFM8 firmware
- Source code for the monitoring and shutdown service for the Raspberry Pi
- Packer build files and instructions on creating the Raspberry Pi image file
