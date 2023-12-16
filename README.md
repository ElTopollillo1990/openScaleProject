# OpenScale Project
Weight scale using [Sparkfun's OpenScale](https://www.sparkfun.com/products/13261).
![](/OpenScaleEnclosure1.jpg)

## Synopsis
This builds on Sparkfun's example repository, a 5 Kg strain gage load cell, and adds 3d-printed enclosure. The enclosure build is stored in [Prusa's Printables](https://www.printables.com/model/677114-enclosure-for-sparkfuns-openscale-and-1kg-20kg-str). OpenScale is based on Arduino core.

## Usage
Build instructions are included on the [Printables site](https://www.printables.com/model/677114-enclosure-for-sparkfuns-openscale-and-1kg-20kg-str).
Project requires a PC with RS232 connection. 
- Open COM terminal and set RS232 COM port baud rate to 9600 (9,600bps 8-N-1).
- Once COM port is configured; connect to OpenScale and follow instructions on screen  

The OpenScale board is directly powered from USB connection. Immediately after power is applied, the board powers up and waits for user entry to either calibrate the load cell; or if the board has been previously calibrated proceed to zero-out (TARE) and then proceed to measurements. While the board is in measurement mode, entering "R" or "r" characters on the RS232 terminal will re-start the zero-out (TARE) process. 

### Startup
Immediately after startup the OpenScale will determine if the board was previously calibrated. It does this by checking the internal EEPROM to determine if a proper previous calibration was stored. If a previous calibration exists it will prompt to either use it or re-calibrate: "_CAL Data Exists, Skip Cal Sequence (Y/N)_". Normally if calibration exists one would chose to skip a new calibration and proceed to measurements.

#### Normal Operation
If calibration is skipped, the OpenScale will proceed to ask to remove anything from the top of the scale plate (_"Remove all weight from scale"_) and continue to zero-out (or TARE) the scale for use (_"Zeroing (tare) scale..."_). Once that is complete it will indicate it is ready for measurements (_"Scale zeroed ... Place desired weight on scale"_).

#### Scale Calibration
If no calibration exists, or a new calibration is desired, the OpenScale will initiate the calibration sequence. It will ask to remove anything from the top of the scale (_"Remove all weight from scale"_), and will zero itself out. Once it is reedy to calibrate it will indicate when it is ready to start measuring (_"After readings begin, place known weight on scale"_).
Once measurements start, the keys "a", "d", "g" are used to increase the calibration factor (if the measurement is too low - compared to the known weight). And the keys "z", "c", "b" are used to decrease the calibration factor (if the measurement is too high - compared to the known weight). Once a reasonable measurement is achieved, calibration can be exited by entering "x". When calibration exits, the calibration factor is saved in the EEPROM for future use and the measurement cycles is started.

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

