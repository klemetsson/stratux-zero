#!/bin/bash
set -Eeuo pipefail

%{ if enable_hdmi == false ~}
# Turn off HDMI
if [ ! -f /boot/hdmi ]; then
    ${enable_hdmi == true ? "#" : ""}tvservice -o
fi
%{ endif ~}

%{ if disable_leds == true ~}
# Disable LEDs (this is only for Raspberry Pi Zero 2, the others are handled in config.txt)
model=$(tr -d '\0' < /proc/device-tree/model)
if [[ $model = "Raspberry Pi Zero 2"* ]]; then
    echo none | tee /sys/class/leds/led0/trigger
    echo 0 | tee /sys/class/leds/led0/brightness
fi
%{ endif ~}
