# stratux-zero.pkr.hcl

source "arm" "raspios-arm64" {
    file_urls = [
        "https://downloads.raspberrypi.org/raspios_lite_arm64/images/raspios_lite_arm64-${var.raspios_release}-raspios-${var.raspios_version}-arm64-lite.zip",
    ]
    file_checksum_url     = "https://downloads.raspberrypi.org/raspios_lite_arm64/images/raspios_lite_arm64-${var.raspios_release}-raspios-${var.raspios_version}-arm64-lite.zip.sha256"
    file_checksum_type    = "sha256"
    file_target_extension = "zip"

    image_path         = "stratux-zero-raspios-arm64.img"
    image_build_method = "resize"
    image_size         = var.image_size
    image_type         = "dos"
    image_chroot_env = [
        "PATH=/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin",
    ]
    image_partitions {
        name         = "boot"
        type         = "c"
        start_sector = 8192
        filesystem   = "vfat"
        size         = "256M"
        mountpoint   = "/boot"
    }
    image_partitions {
        name         = "root"
        type         = "83"
        start_sector = 532480
        filesystem   = "ext4"
        size         = "0"
        mountpoint   = "/"
    }

    qemu_binary_source_path      = "/usr/bin/qemu-aarch64-static"
    qemu_binary_destination_path = "/usr/bin/qemu-aarch64-static"
}

build {
    name = "stratux-zero"

    sources = [
        "source.arm.raspios-arm64",
    ]

    # Filter out all the documentation from installed packages
    provisioner "file" {
        source      = "${path.root}/config/01_nodoc"
        destination = "/etc/dpkg/dpkg.cfg.d/01_nodoc"
    }

    # Refresh and upgrade packages
    provisioner "shell" {
        inline = [
            "apt-get update",
            "${var.raspios_upgrade == false ? "#" : ""}apt-get upgrade -y",
        ]
    }

    # Install required packages
    provisioner "shell" {
        inline = [
            "echo iptables-persistent iptables-persistent/autosave_v4 boolean true | debconf-set-selections",
            "echo iptables-persistent iptables-persistent/autosave_v6 boolean true | debconf-set-selections",
            "apt-get install -y ${join(" ", distinct(concat(
                var.apt_required_packages,
                var.apt_extra_packages,
            )))}",
        ]
    }

    # Setup boot string and device tree
    provisioner "shell" {
        inline = [
            # Disable serial console, disable rfkill state restore, enable wifi on boot
            "sed -i /boot/cmdline.txt -e \"s/console=serial0,[0-9]\\+ /systemd.restore_state=0 rfkill.default_state=1 /\"",

            # Add the workaround device tree support for the Zero 2
            # https://waldorf.waveform.org.uk/2021/the-pi-zero-2.html
            "cp -n /boot/bcm2710-rpi-3-b.dtb /boot/bcm2710-rpi-zero-2.dtb",
        ]
    }

    # Change default username
    provisioner "shell" {
        inline = [
            "${var.raspios_username != "pi" ? "#" : ""}exit 0",
            "usermod -l ${var.raspios_username} pi",
            "usermod -m -d /home/${var.raspios_username} ${var.raspios_username}",
            "groupmod -n ${var.raspios_username} pi",
            "sed -i /etc/sudoers.d/010_pi-nopasswd -e 's/^pi/${var.raspios_username}/g'",
        ]
    }

    # Change default password
    provisioner "shell" {
        inline = [
            "${var.raspios_password != "raspberry" ? "#" : ""}exit 0",
            "echo \"${replace(replace(var.raspios_password, "\\", "\\\\"), "\"", "\\\"")}\\n${replace(replace(var.raspios_password, "\\", "\\\\"), "\"", "\\\"")}\" | passwd ${var.raspios_username}",
        ]
    }

    # Create a Raspberry boot configuration
    provisioner "file" {
        content = templatefile(
            "config/config.txt.pkrtpl.hcl",
            {
                rpi = {
                    overclock_sd = var.rpi_overclock_sd
                    disable_leds = var.rpi_disable_leds
                }
                gpio = {
                    shutdown_pin = var.gpio_shutdown_pin
                    status_pin   = var.gpio_status_pin
                }
            }
        )
        destination = "/boot/config.txt"
    }

    # Setup system configuration
    provisioner "shell" {
        inline = [
            # Disable swap
            "systemctl disable dphys-swapfile",
            "apt-get purge -y --auto-remove dphys-swapfile",

            # Disable services we don't need
            "systemctl disable sshswitch.service",
            "systemctl disable keyboard-setup.service",
            "systemctl disable apt-daily.service",
            "systemctl disable hciuart.service",
            "systemctl disable triggerhappy.service",
            "systemctl disable dnsmasq.service",
            "systemctl disable dhcpcd.service",
            "systemctl disable rpi-display-backlight.service",
            "systemctl disable bluetooth.service",
            "systemctl disable rsync.service",
            "systemctl disable wpa_supplicant.service",
            "systemctl disable raspberrypi-net-mods.service",
            "systemctl disable console-setup.service",
            "systemctl disable raspi-config.service",
            "systemctl disable systemd-timesyncd.service",
            "systemctl disable apt-daily.timer",
            "systemctl disable apt-daily-upgrade.timer",
            "systemctl disable man-db.timer",

            # Generate ssh key for all installs. Otherwise it would have to be done on each boot, which takes a couple of seconds
            "ssh-keygen -A -v",
            "systemctl disable regenerate_ssh_host_keys",
            "/lib/console-setup/console-setup.sh",

            # Set hostname
            "echo '${var.hostname}' > /etc/hostname",
            "sed -i /etc/hosts -e \"s/raspberrypi/${var.hostname}/g\"",

            # Set the keyboard layout to US.
            "sed -i /etc/default/keyboard -e \"/^XKBLAYOUT/s/\\\".*\\\"/\\\"us\\\"/\"",

            # Run DHCP on eth0 when cable is plugged in
            "apt-get install -y ifplugd",
            "sed -i -e 's/INTERFACES=\"\"/INTERFACES=\"eth0\"/g' /etc/default/ifplugd",

            # Setup Avahi
            "systemctl enable avahi-daemon.service",
            "sed -i /etc/avahi/avahi-daemon.conf -e 's/^use\\-ipv6=yes$/use\\-ipv6=no/g'",

            # Setup SSH
            "${var.enable_ssh == false ? "#" : ""}systemctl enable ssh.service",
        ]
    }

    # Create firewall rules
    provisioner "file" {
        content = templatefile(
            "config/rules.v4.pkrtpl.hcl",
            {
                network_cidr = var.network_cidr
                enable_ssh   = var.enable_ssh
            }
        )
        destination = "/etc/iptables/rules.v4"
    }
    provisioner "file" {
        content = templatefile(
            "config/rules.v6.pkrtpl.hcl",
            {
            }
        )
        destination = "/etc/iptables/rules.v6"
    }

    # Massage of the day
    provisioner "file" {
        content = templatefile(
            "config/motd.pkrtpl.hcl",
            {
                build = formatdate("EEEE, DD-MMM-YY hh:mm:ss ZZZ", timestamp())
            }
        )
        destination = "/etc/motd"
    }

    # Boot configuration script
    provisioner "file" {
        content = templatefile(
            "config/boot_config.sh.pkrtpl.hcl",
            {
                disable_leds   = var.rpi_disable_leds
                enable_hdmi    = var.enable_hdmi
            }
        )
        destination = "/usr/local/bin/boot_config.sh"
    }
    provisioner "file" {
        source      = "${path.root}/config/boot_config.service"
        destination = "/etc/systemd/system/boot_config.service"
    }
    provisioner "shell" {
        inline = [
            "chmod +x /usr/local/bin/boot_config.sh",
            "systemctl enable boot_config.service",
        ]
    }

    # GPIO status services
    provisioner "file" {
        source      = "${path.cwd}/src/gpio-status/gpio-status.py"
        destination = "/usr/local/bin/gpio-status"
    }
    provisioner "file" {
        content = templatefile(
            "config/gpio-status.service.pkrtpl.hcl",
            {
                gpio_pin = var.gpio_status_pin == null ? 18 : var.gpio_status_pin
            }
        )
        destination = "/etc/systemd/system/gpio-status.service"
    }
    provisioner "shell" {
        inline = [
            "chmod +x /usr/local/bin/gpio-status",
            "${var.gpio_status_pin == null ? "#" : ""}systemctl enable gpio-status.service",
        ]
    }

    # GPIO shutdown services
    provisioner "file" {
        source      = "${path.cwd}/src/gpio-shutdown/gpio-shutdown.py"
        destination = "/usr/local/bin/gpio-shutdown"
    }
    provisioner "file" {
        content = templatefile(
            "config/gpio-shutdown.service.pkrtpl.hcl",
            {
                gpio_pin = var.gpio_shutdown_pin == null ? 17 : var.gpio_shutdown_pin
            }
        )
        destination = "/etc/systemd/system/gpio-shutdown.service"
    }
    provisioner "shell" {
        inline = [
            "chmod +x /usr/local/bin/gpio-shutdown",
            "${var.gpio_shutdown_pin == null ? "#" : ""}systemctl enable gpio-shutdown.service",
        ]
    }

    # Disable ALSA audio support
    provisioner "file" {
        content     = "blacklist snd_bcm2835"
        destination = "/etc/modprobe.d/blacklist_alsa.conf"
    }

    # Install build requirements
    provisioner "shell" {
        inline = [
            "apt-get install -y ${join(" ", var.apt_build_packages)}",
            # Install Go
            "wget -nv -c https://go.dev/dl/go${var.go_version}.linux-arm64.tar.gz -O - | tar -xz -C /usr/local",
        ]
    }

    # Download source code
    provisioner "shell" {
        inline = [
            # Stratux
            "git -C /tmp clone --recurse-submodules https://github.com/b3nn0/stratux.git",
            "git -C /tmp/stratux checkout -b build-temp ${var.stratux_version}",

            # osmocom version of librtlsdr
            "git -C /tmp clone https://github.com/osmocom/rtl-sdr.git",
            "git -C /tmp/rtl-sdr checkout -b build-temp ${var.rtl_sdr_version}",

            # kalibrate-rtl
            "git -C /tmp clone https://github.com/steve-m/kalibrate-rtl.git",
            "git -C /tmp/kalibrate-rtl checkout -b build-temp ${var.kalibrate_rtl_version}",

            # WiringPi
            "git -C /tmp clone https://github.com/WiringPi/WiringPi.git",
            "git -C /tmp/WiringPi checkout -b build-temp ${var.wiringpi_version}",
        ]
    }

    # Build and install the librtlsdr
    provisioner "shell" {
        inline = [
            "mkdir -p /tmp/rtl-sdr/build",
            "cd /tmp/rtl-sdr/build",
            "cmake .. -DENABLE_ZEROCOPY=0",
            "make -j8",
            "make install",
        ]
    }

    # Build and install kalibrate-rtl
    provisioner "shell" {
        inline = [
            "cd /tmp/kalibrate-rtl",
            "./bootstrap",
            "./configure",
            "make -j8",
            "make install",
        ]
    }

    # Build and install WiringPi
    provisioner "shell" {
        inline = [
            "cd /tmp/WiringPi",
            "./build",
        ]
    }

    # Add a fake fancontrol.service so that the Stratux "make install" doesn't fail
    provisioner "file" {
        source      = "${path.root}/config/fancontrol.service"
        destination = "/etc/systemd/system/fancontrol.service"
    }

    # Create an empty Stratux configuration
    provisioner "file" {
        content     = "{}"
        destination = "/boot/stratux.conf"
    }

    # Build and install Stratux
    provisioner "shell" {
        inline = [
            "cd /tmp/stratux",
            "make -j8",
            "make install",
        ]
        environment_vars = [
            "PATH=$PATH:/usr/local/go/bin",
        ]
    }

    # Setup network configuration
    provisioner "file" {
        content = templatefile(
            "config/interfaces.pkrtpl.hcl",
            {
                ip = {
                    address = cidrhost(var.network_cidr, var.network_host_number)
                    netmask = cidrnetmask(var.network_cidr)
                }
                wifi = {
                    tx_power_limit = var.wifi_tx_power_limit
                }
            }
        )
        destination = "/etc/network/interfaces"
    }
    provisioner "file" {
        content = replace(templatefile(
            "config/interfaces.template.pkrtpl.hcl",
            {
                ip = {
                    netmask = cidrnetmask(var.network_cidr)
                }
                wifi = {
                    tx_power_limit = var.wifi_tx_power_limit
                }
            }
        ), "{{", "\\{\\{") # Workaround for the {{ escape issue in Packer
        destination = "/opt/stratux/cfg/interfaces.template"
    }
    provisioner "shell" {
        inline = [
            # Workaround for the {{ escape issue in Packer
            "sed -i /opt/stratux/cfg/interfaces.template -e 's/\\\\{/{/g'",
        ]
    }

    # Setup configuration files from Stratux project
    provisioner "shell" {
        inline = [
            "cd /tmp/stratux/image",

            # Network config
            "cp -f stratux-dnsmasq.conf /etc/dnsmasq.d/stratux-dnsmasq.conf",
            "cp -f wpa_supplicant_ap.conf /etc/wpa_supplicant/wpa_supplicant_ap.conf",
            "sed -i /etc/wpa_supplicant/wpa_supplicant_ap.conf -e \"s/\\\"stratux\\\"/\\\"${var.wifi_ap_ssid}\\\"/g\"",

            # logrotate
            "cp -f logrotate.conf /etc/logrotate.conf",
            "cp -f logrotate_d_stratux /etc/logrotate.d/stratux",

            # sshd
            "cp -f sshd_config /etc/ssh/sshd_config",

            # Aliases
            "cp -f stxAliases.txt /etc/profile.d/stratux_aliases.sh",

            # rtl-sdr
            "cp -f rtl-sdr-blacklist.conf /etc/modprobe.d/",

            # Modules for i2c
            "cp -f modules.txt /etc/modules",

            # Update the Stratux configuration
            "echo '${jsonencode(
                {
                    DeveloperMode = var.enable_developer_mode
                    WiFiIPAddress = cidrhost(var.network_cidr, var.network_host_number)
                    WiFiSSID      = var.wifi_ap_ssid
                    DarkMode      = var.web_darkmode
                    GPS_Enabled   = var.enable_gnss
                    UAT_Enabled   = var.enable_uat
                    ES_Enabled    = var.enable_es
                    OGN_Enabled   = var.enable_ogn
                    AIS_Enabled   = var.enable_ais
                    BMP_Enabled   = var.enable_bmp
                    IMU_Enabled   = var.enable_imu
                    # fancontrol options
                    PWMPin = var.gpio_fan_pin != null ? var.gpio_fan_pin : 4
                }
            )}' > /boot/stratux.conf.tmp",
            "jq -Mn --argfile file1 /boot/stratux.conf --argfile file2 /boot/stratux.conf.tmp '$file1 + $file2'  > /boot/stratux.conf.new",
            "rm /boot/stratux.conf.tmp",
            "mv -f /boot/stratux.conf.new /boot/stratux.conf",

            # Enable fancontrol (first remove the dummy service and the rc.d)
            "rm /etc/systemd/system/fancontrol.service",
            "update-rc.d fancontrol disable",
            "${var.gpio_fan_pin == null ? "#": ""}/opt/stratux/bin/fancontrol install",
        ]
    }

    # Cleanup installation
    provisioner "shell" {
        inline = [
            "apt-get purge -y ${join(" ", distinct(concat(
                var.apt_build_packages,
                var.apt_remove_packages,
            )))}",
            "apt-get autoremove -y",
            "apt-get clean",
            "rm -Rf /root/go",
            "rm -Rf /root/.cache",
            "rm -Rf /usr/local/go",
        ]
    }

    post-processor "checksum" {
        checksum_types = [ "sha256", "sha512" ]
        output = "stratux-zero-{{.BuildName}}.{{.ChecksumType}}.checksum"
    }
}
