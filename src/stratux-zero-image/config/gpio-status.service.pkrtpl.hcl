[Unit]
Description=Signal Stratux service status on GPIO-${gpio_pin}
After=stratux.service

[Service]
Type=simple
Restart=always
ExecStart=/usr/local/bin/gpio-status ${gpio_pin}

[Install]
WantedBy=multi-user.target
