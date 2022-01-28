auto lo

iface lo inet loopback

# allow-hotplug eth0 # configured by ifplugd
iface eth0 inet dhcp

allow-hotplug wlan0

# AP or AP+Client -> create seperate ap0 virtual interface for the AP; optionally use wlan0 for client connection
iface ap0 inet static
  address ${ip.address}
  netmask ${ip.netmask}
  post-up /opt/stratux/bin/stratux-wifi.sh ap0 0

# Pure AP mode
iface wlan0 inet manual
  pre-up iw wlan0 set type managed; iw phy0 interface add ap0 type __ap; ifup ap0
  wireless-power off
  post-down ifdown ap0; ifconfig ap0 down; iw ap0 del
  ${wifi.tx_power_limit != null ? "up iw wlan0 set txpower limit ${wifi.tx_power_limit}" : ""}
