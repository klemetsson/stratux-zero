#!/usr/bin/env python3
"""
Monitor the status of the Stratux service and signal this on a GPIO pin.

While the status is OK, the pin will pulse every 1 second. This allow
the external monitor to reset in case the service is not running.
For example because of a kernal panic.

If GNSS is enabled but not enough satellites, then the pulse will be
200 ms, otherwise 500 ms.
"""

import os
import sys
import time
import signal
import logging
import requests
import RPi.GPIO as GPIO


STRATUX_STATUS_URL = 'http://127.0.0.1/getStatus'
STRATUX_SETTINGS_URL = 'http://127.0.0.1/getSettings'

MIN_SATELLITES = 3


def _main(gpio):
    """Monitor the status of Stratux."""

    logging.info('Monitoring Stratux and signaling on GPIO pin {0}'.format(gpio))

    # Setup the status GPIO pin
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(gpio, GPIO.OUT)
    GPIO.output(gpio, GPIO.LOW)

    # Loop until shutting down
    signal.signal(signal.SIGINT, lambda _s, _f: sys.exit(0))
    while True:
        # Check if GPS is enabled
        try:
            response = requests.get(STRATUX_SETTINGS_URL, timeout=1)
            response.raise_for_status()
            data = response.json()
        except requests.RequestException:
            has_gnss = False
        else:
            has_gnss = 'GPS_Enabled' in data and data['GPS_Enabled']

        # Check status
        try:
            response = requests.get(STRATUX_STATUS_URL, timeout=2)
            response.raise_for_status()
            data = response.json()
        except requests.RequestException:
            GPIO.output(gpio, GPIO.LOW)
            time.sleep(1)
        else:
            if 'GPS_connected' in data and data['GPS_connected']:
                has_gnss_lock = 'GPS_satellites_locked' in data and data['GPS_satellites_locked'] >= MIN_SATELLITES
            else:
                has_gnss_lock = False
            # Send a pulse if Stratux is available
            if has_gnss and not has_gnss_lock:
                # Shorter pulse if alive, but waiting for GNSS
                GPIO.output(gpio, GPIO.HIGH)
                time.sleep(0.2)
                GPIO.output(gpio, GPIO.LOW)
                time.sleep(0.8)
            else:
                GPIO.output(gpio, GPIO.HIGH)
                time.sleep(0.5)
                GPIO.output(gpio, GPIO.LOW)
                time.sleep(0.5)


if __name__ == "__main__":
    _main(int(sys.argv[1]))
