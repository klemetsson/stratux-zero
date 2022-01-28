#!/bin/bash
set -Eeuo pipefail

# Turn off HDMI
${enable_hdmi == true ? "#" : ""}tvservice -o

# Disable LEDs (this is only for Raspberry Pi Zero 2, the others are handled in config.txt)
${disable_leds == false ? "#" : ""}echo 0 | tee /sys/class/leds/led0/brightness
