# OpenScale Project
Weight scale using [Sparkfun's OpenScale](https://www.sparkfun.com/products/13261).
![](/OpenScaleEnclosure1.jpg)

## Synopsis
This builds on Sparkfun's example repository, a 5 Kg strain gage load cell, and adds 3d-printed enclosure. The enclosure build is stored in [Prusa's Printables](https://www.printables.com/model/677114-enclosure-for-sparkfuns-openscale-and-1kg-20kg-str). OpenScale is based on Arduino core.

## Usage
Build instructions are included on the [Printables site](https://www.printables.com/model/677114-enclosure-for-sparkfuns-openscale-and-1kg-20kg-str).
Project requires a PC with RS232 connection. 
- Open COM terminal and set RS232 COM port baud rate to 9600.
- Once COM port is configured; connect to OpenScale and follow instructions on screen  

The OpenScale board is directly powered from USB connection. Immediately after power is applied, the board powers up and waits for user entry to either calibrate the load cell; or if the board has been previously calibrated proceed to zero-out (TARE) and then proceed to measurements. While the board is in measurement mode, entering "R" or "r" characters on the RS232 terminal will re-start the zero-out (TARE) process. 

## Final Build
| ![](/OpenScale_1a.jpg) | ![](/OpenScale_3.jpg) |
![](/OpenScale_4.jpg)

## Resources
* https://www.sparkfun.com/products/13261
* https://www.sparkfun.com/products/13329
* https://www.adafruit.com/product/4630
* https://www.amazon.com/dp/B07NRW8RP6/
* https://github.com/sparkfun/OpenScale
* https://github.com/bogde/HX711

