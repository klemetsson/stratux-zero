# variables.pkr.hcl

variable "raspios_version" {
    description = "Version of the Raspberry Pi OS base image to use."
    type        = string
    default     = "2021-11-08/2021-10-30"
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

variable "network_cidr" {
    description = "The CIDR for the Wifi access point network."
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
    description = "The GPIO pin to use for fancontrol. Set to `null` to disable this feature."
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

variable "apt_required_packages" {
    description = "A list of packages to install on the image."
    type        = list(string)
    default = [
        "libconfig9",
        "rpi-update",
        "dnsmasq",
        "i2c-tools",
        "python3-serial",
        "jq",
        "ifplugd",
        "iptables",
        "iptables-persistent",
        "libjpeg62-turbo",
        "libfftw3-3",
        "libncurses6",
    ]
}

variable "apt_extra_packages" {
    description = "A list of additional packages to install on the image."
    type        = list(string)
    default     = []
}
