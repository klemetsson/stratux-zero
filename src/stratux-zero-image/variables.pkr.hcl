# variables.pkr.hcl

variable "raspios_version" {
    description = "Version of the Raspberry Pi OS base image to use."
    type        = string
    default     = "bullseye"
}

variable "raspios_release" {
    description = "Release of the Raspberry Pi OS base image to use. See https://downloads.raspberrypi.org/raspios_lite_arm64/images/ for the latest."
    type        = string
    default     = "2022-01-28/2022-01-28"
}

variable "raspios_upgrade" {
    description = "Upgrade packages in the Raspberry Pi OS image."
    type        = bool
    default     = true
}

variable "raspios_username" {
    description = "The username of the default system user."
    type        = string
    default     = "pi"
}

variable "raspios_password" {
    description = "The password of the default system user."
    type        = string
    default     = "raspberry"
}

variable "go_version" {
    description = "Version of Go to use for building Stratux."
    type        = string
    default     = "1.17.6"
}

variable "stratux_version" {
    description = "The version of b3nn0/stratux to checkout by commit."
    type        = string
    default     = "258ca7fc0e93bcd6efc0c7a3cf1de284fc814026"
}

variable "rtl_sdr_version" {
    description = "The version of osmocom/rtl-sdr to checkout by commit."
    type        = string
    default     = "0847e93e0869feab50fd27c7afeb85d78ca04631"
}

variable "kalibrate_rtl_version" {
    description = "The version of steve-m/kalibrate-rtl to checkout by commit."
    type        = string
    default     = "ed9b436ad5f1afa6509f9109bd04b7b2916cbeae"
}

variable "wiringpi_version" {
    description = "The version of WiringPi to checkout by commit."
    type        = string
    default     = "5de0d8f5739ccc00ab761639a7e8d3d1696a480a"
}

variable "image_size" {
    description = "Size of the finished disk image."
    type        = string
    default     = "3.5G"
}

variable "wifi_ap_ssid" {
    description = "The SSID to use for the Wifi access point."
    type        = string
    default     = "StratuxZero"
}

variable "web_darkmode" {
    description = "Use darkmode for the web interface."
    type        = bool
    default     = true
}

variable "hostname" {
    description = "The hostname to use for the OS."
    type        = string
    default     = "stratux"
}

variable "network_cidr" {
    description = "The CIDR for the Wifi access point network. Must be a /24 network and using anything other than `\"192.168.10.0/24\"` may cause issues with dnsmasq."
    type        = string
    default     = "192.168.10.0/24"
}

variable "network_host_number" {
    description = "The address within `network_cidr` to assign to the device."
    type        = number
    default     = 1
}

variable "enable_ssh" {
    description = "Enable and allow SSH traffic."
    type        = bool
    default     = false
}

variable "enable_hdmi" {
    description = "Enable the HDMI output."
    type        = bool
    default     = true
}

variable "enable_developer_mode" {
    description = "Put Stratux in developer mode."
    type        = bool
    default     = false
}

variable "enable_gnss" {
    description = "Enable GNSS (GPS, GLONASS, etc.)."
    type        = bool
    default     = true
}

variable "enable_uat" {
    description = "Enable 978 MHz UAT radio."
    type        = bool
    default     = false
}

variable "enable_es" {
    description = "Enable 1090 MHz ADS-B ES radio."
    type        = bool
    default     = true
}

variable "enable_ogn" {
    description = "Enable 868 MHz OGN radio."
    type        = bool
    default     = true
}

variable "enable_ais" {
    description = "Enable 162 MHz AIS radio."
    type        = bool
    default     = false
}

variable "enable_bmp" {
    description = "Enable BMP pressure altimeter."
    type        = bool
    default     = true
}

variable "enable_imu" {
    description = "Enable IMU sensor for AHRS."
    type        = bool
    default     = true
}

variable "gpio_shutdown_pin" {
    description = "The GPIO pin to use for signaling the Raspberry to shutdown. Set to `null` to disable this feature."
    type        = number
    default     = 17
}

variable "gpio_status_pin" {
    description = "The GPIO pin to use for indicating that Stratux is running. Set to `null` to disable this feature."
    type        = number
    default     = 18
}

variable "gpio_fan_pin" {
    description = "The GPIO pin to use for fancontrol. Set to `null` to disable this feature. Note that this setting is currently overwritten by the Stratux web app forcing it to default to 18."
    type        = number
    default     = null
}

variable "rpi_overclock_sd" {
    description = "Overclock the SD Card from 50 to 100MHz to improve boot time. This can only be done with at least a UHS Class 1 card."
    type        = bool
    default     = true
}

variable "rpi_disable_leds" {
    description = "Disable the status and power LEDs on the Raspberry Pi."
    type        = bool
    default     = true
}

variable "wifi_tx_power_limit" {
    description = "Reduce the range of the Wifi by limiting the transmit power. Entered in mBm, i.e. 100 means 1.00 dBm. Set to `null` for default power."
    type        = number
    default     = 200
}

variable "apt_required_packages" {
    description = "A list of packages to install on the image."
    type        = list(string)
    default = [
        "libconfig9",
        "rpi-update",
        "dnsmasq",
        "i2c-tools",
        "python3-serial",
        "python3-smbus",
        "python3-requests",
        "jq",
        "ifplugd",
        "iptables",
        "iptables-persistent",
        "libjpeg62-turbo",
        "libfftw3-3",
        "libncurses6",
        "avahi-daemon",
        "rpi.gpio",
    ]
}

variable "apt_extra_packages" {
    description = "A list of additional packages to install on the image."
    type        = list(string)
    default     = []
}

variable "apt_build_packages" {
    description = "A list of packages needed for building Stratux."
    type        = list(string)
    default = [
        "libjpeg62-turbo-dev",
        "git",
        "cmake",
        "cmake-data",
        "libusb-1.0-0-dev",
        "build-essential",
        "autoconf",
        "libfftw3-dev",
        "libncurses-dev",
        "libtool",
        "m4",
        "automake",
    ]
}

variable "apt_remove_packages" {
    description = "A list of packages to remove from the image in addition to `apt_build_packages`."
    type        = list(string)
    default = [
        "bluez",
        "bluez-firmware",
        "cifs-utils",
        "v4l-utils",
        "rsync",
        "pigz",
        "pi-bluetooth",
        "perl",
        "cpp",
        "cpp-10",
    ]
}
