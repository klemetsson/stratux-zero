#!/usr/bin/env python3
"""Helper for finding threshold values for CO alarm."""

# The calculation is based of https://github.com/ElectronicCats/MICS4514-Croquette
# and is not verified.

LOAD_RESISTOR_OHM = 47000
VSUP = 3.3
VREF = 3.3
ADC_RESOLUTION_BITS = 10

def _main():
    """Present the ppm value for each ADC raw value for the MiCS-4515."""

    for adc in range(2 ** ADC_RESOLUTION_BITS):
        try:
            vco = (VREF * adc) / ((2 ** ADC_RESOLUTION_BITS) - 1)
            rco = LOAD_RESISTOR_OHM / ((VSUP - vco) / vco)
            conco = LOAD_RESISTOR_OHM / rco
            ppmco = 4.4138 * pow(conco, -1.178)
        except ZeroDivisionError:
            continue
        print('0x{0:04x} // {1} ppm CO'.format(adc, round(ppmco, 1)))

if __name__ == "__main__":
    _main()
