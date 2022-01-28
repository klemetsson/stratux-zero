#!/usr/bin/env python3
"""
Monitor the status of the Stratux service and signal this on a GPIO pin.

While the status is OK, the pin will pulse every 1 second. This allow
the external monitor to reset in case the service is not running.
For example because of a kernal panic.
"""

import os
import sys
import time
import signal
import logging
import requests
import RPi.GPIO as GPIO


STRATUX_STATUS_URL = 'http://127.0.0.1/getStatus'


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
        # Check status
        try:
            response = requests.get(STRATUX_STATUS_URL, timeout=5)
            response.raise_for_status()
        except requests.RequestException:
            GPIO.output(gpio, GPIO.LOW)
            time.sleep(1)
        else:
            # Send a pulse if Stratux is available
            GPIO.output(gpio, GPIO.HIGH)
            time.sleep(0.5)
            GPIO.output(gpio, GPIO.LOW)
            time.sleep(0.5)


if __name__ == "__main__":
    _main(int(sys.argv[1]))
