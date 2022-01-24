# Stratux Zero

Portable ADS-B/Flarm receiver, AHRS, GNSS and CO-alarm based on the Stratux project and Raspberry Pi Zero 2.

Note that this does not replace any primary systems and should only be used as a complement for increased safety and situational awareness.

> THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

## Features

- Dual USB 2.0 receptacles for [NooElec NESDR Nano 2+](https://www.nooelec.com/store/nesdr-nano2.html) or similar, [Microchip USB2512](https://www.microchip.com/en-us/product/USB2512)
- Pressure altimeter, [Bosch BMP280](https://www.bosch-sensortec.com/products/environmental-sensors/pressure-sensors/bmp280/)
- 9-axis motion tracking, [TDK ICM-20948](https://invensense.tdk.com/products/motion-tracking/9-axis/icm-20948/)
- GNSS (GPS, Galileo, GLONASS, BeiDou) with 2.5 m CEP accuracy, [u-blox CAM-M8Q](https://www.u-blox.com/en/product/cam-m8-series)
- GNSS backup power for faster initial lock
- Carbon monoxide detector with audible alarm, [SGX MICS-4514](https://sgx.cdistore.com/products/detail/mics4514-sgx-sensortech/333417/)
- 18650 battery socket
- 4.2 V overvoltage, 2.5 V undervoltage and over-current protection, [TI BQ2972](https://www.ti.com/product/BQ2972)
- USB C power with built-in 1 A charger with support for USB BC1.2, Apple, Samsung and legacy USB charge adapters, [Maxim MAX77751](https://www.maximintegrated.com/en/products/power/battery-management/MAX77751.html)
- High temperature and low temperature charge monitoring
- Fuel gauge that monitors the battery state of charge and aging, [TI BQ27441](https://www.ti.com/product/BQ27441-G1)
- Hard/soft power switch that first signals the Raspberry to shutdown cleanly and if that takes to long, power will be cut
- Raspberry Pi Zero 2 interface with pogo pins
- Watchdog that resets the Raspberry Pi
- Five LED indicators
    - Green power indicator that flashes if low battery
    - Green Stratux status indicator that flashes if starting up or shutting down
    - Red Carbon monoxide alert that flashes every 5 seconds if detected
    - Green external power indicator
    - Orange charge indicator that flashes while charging and turned on when fully charged

The CO-alarm, watchdog, charging temperature monitoring, battery and power management is handled by an [Silicon Labs EFM8BB10 8051 MCU](https://www.silabs.com/mcu/8-bit-microcontrollers/efm8-busy-bee/device.efm8bb10f4g-qfn20).

## Contents of this repository

- CAD and CAM files for the custom PCBs
- CAD files for the enclosure
- Source code for the EFM8 firmware
- Source code for the monitoring and shutdown service for the Raspberry Pi
- [Hashicorp Packer](https://www.packer.io/) build files and instructions on creating the Raspberry Pi image file

## Design decisions

Because of the global component shortage, many of the components are selected out of availability. This leads to a more expensive design with several difficult component footprints.
Still, several of the components are impossible to acquire separatly and may need to be desoldered from various break-out and development kits.

The integrated 1090 MHz antenna for ADS-B data is a simple dipole design. This however, requires a balun with impedence matching which is located on a separate PCB.

## Building the Stratux Zero image

These instructions are written for a AMD64 Ubuntu Linux environment.

### Prerequisites

Install all required components for compiling and building the Raspberry Pi Zero 2 disk image.

**Install required packages:**

```bash
sudo apt-get install git unzip qemu-user-static e2fsprogs dosfstools libarchive-tools
```

**Install Packer:**

```bash
curl -fsSL https://apt.releases.hashicorp.com/gpg | sudo apt-key add -
sudo apt-add-repository "deb [arch=amd64] https://apt.releases.hashicorp.com $(lsb_release -cs) main"
sudo apt-get update && sudo apt-get install packer
```

**Install Go 1.17:**

```bash
wget -c https://go.dev/dl/go1.17.6.linux-amd64.tar.gz -O - | sudo tar -xz -C /usr/local
export PATH=$PATH:/usr/local/go/bin
echo 'export PATH=$PATH:/usr/local/go/bin' | sudo tee /etc/profile.d/go_path.sh
```

**Build and install packer-builder-arm plugin:**

```bash
git clone https://github.com/mkaczanowski/packer-builder-arm
cd packer-builder-arm
go mod download
go build
sudo mv packer-builder-arm $(dirname $(which packer))
cd ../
rm -Rf packer-builder-arm
```
