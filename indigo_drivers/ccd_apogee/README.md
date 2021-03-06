# Apogee CCD driver
http://www.andor.com/scientific-cameras/apogee-camera-range

## Supported devices
Apogee Alta, AltaF, Ascent and Aspen cameras.

See https://github.com/indigo-astronomy/indigo/blob/master/indigo_drivers/ccd_apogee/externals/libapogee/conf/apnmatrix.txt for complete list.

This driver supports USB and Ethernet hot-plug (multiple devices).

## Supported platforms
This driver works on Linux (Intel 32/64 bit) and MacOSX.

## License
INDIGO Astronomy open-source license.

Apogee SDK is subject to the terms of the Mozilla Public License, v. 2.0

## Use
In case Apogee configuration files are not in the default location, INDIGO_FIRMWARE_BASE
environment variable must be set to the correct path before starting indigo_server.
The default INDIGO_FIRMWARE_BASE is: "/usr/local/etc/" and configuration files should be in:
"$INDIGO_FIRMWARE_BASE/apogee"

indigo_server indigo_ccd_apogee

## Status: Stable
Driver is developed and tested with:
* Apogee Aspen-16M
* Apogee AltaF-2000
