[Unit]
Description=Wait for GPIO-${gpio_pin} high to trigger a shutdown
Wants=network-online.target
After=network.target network-online.target

[Service]
Type=simple
Restart=always
ExecStart=/usr/local/bin/gpio-shutdown ${gpio_pin}

[Install]
WantedBy=multi-user.target
