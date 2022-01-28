#!/usr/bin/env python3
"""A service that listens to a GPIO and triggers a shutdown on raising flank."""

import os
import sys
import time
import signal
import logging
import RPi.GPIO as GPIO


def _main(gpio):
    """Setup the GPIO watcher."""

    logging.info('Listening on GPIO pin {0} for shutdown'.format(gpio))

    # Setup the shutdown GPIO pin
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(gpio, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    GPIO.add_event_detect(
        gpio, GPIO.RISING,
        callback=lambda: os.system('shutdown -h now'),
        bouncetime=100
    )

    # Loop until shutting down
    signal.signal(signal.SIGINT, lambda _s, _f: sys.exit(0))
    while True:
        time.sleep(10)


if __name__ == "__main__":
    _main(int(sys.argv[1]))
