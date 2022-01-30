# Disable the rainbow splash screen
disable_splash=1

# Overclock the SD Card from 50 to 100MHz
# This can only be done with at least a UHS Class 1 card
${rpi.overclock_sd != true ? "#" : ""}dtoverlay=sdtweak,overclock_50=100

# Set the bootloader delay to 0 seconds. The default is 1s if not specified.
boot_delay=0

# Enable I2C
dtparam=i2c_arm=on
dtparam=i2s=on
dtparam=i2c1_baudrate=400000
dtparam=i2c_arm_baudrate=400000

# Disable Bluetooth
dtoverlay=disable-bt

# Disable audio
dtparam=audio=off

# Disable camera and display detect
camera_auto_detect=0
display_auto_detect=0

# Video setup
max_framebuffers=2

# Setup GPIO
${gpio.shutdown_pin == null ? "#" : ""}gpio=${gpio.shutdown_pin}=ip,pu
${gpio.status_pin == null ? "#" : ""}gpio=${gpio.status_pin}=op,dl

# Disable LEDs
${rpi.disable_leds != true ? "#" : ""}dtparam=act_led_trigger=none
${rpi.disable_leds != true ? "#" : ""}dtparam=act_led_activelow=off
${rpi.disable_leds != true ? "#" : ""}dtparam=pwr_led_trigger=none
${rpi.disable_leds != true ? "#" : ""}dtparam=pwr_led_activelow=off

[pi4]
# Run as fast as firmware / board allows
arm_boost=1

[pi0]
# Disable LEDs
${rpi.disable_leds != true ? "#" : ""}dtparam=act_led_trigger=none
${rpi.disable_leds != true ? "#" : ""}dtparam=act_led_activelow=on

[pi02]
# LED must be disabled in software for the Zero 2
${rpi.disable_leds != true ? "#" : ""}dtparam=act_led_trigger=none
${rpi.disable_leds != true ? "#" : ""}dtparam=act_led_activelow=off

[all]
