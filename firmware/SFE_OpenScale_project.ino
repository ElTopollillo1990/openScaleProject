/*
  OpenScale Application
  @languer 2020-2023
  GPL-3.0 license

  Project using OpenScale output with capability calibrate.
  Allows to bypass calibration if the cal data is present already.
  Allows ability to monitor RS232 input during measurements and re-start TARE function

  Arduino pin
  3 -> HX711 CLK (modified to work with openscale, use pin2 for HX711 breakout)
  2 -> DAT (modified to work with openscale, use pin3 for HX711 breakout)
  5V -> VCC
  GND -> GND

  Credits
  https://github.com/sparkfun/OpenScale
  https://github.com/bogde/HX711

	 Target
	 Board: Arduino/Genuino Uno
    Programmer: AVR ISP or AVR ISP mkII
*/

/************************* HX711 Setup *********************************/
#include "HX711.h"
#define RESOLUTION 3     //decimal places of HX711 reading
#define AVG_FACT 8        //averagin factor of HX711 reading
#define DOUT_PIN 2        //data IO of HX711
#define CLK_PIN 3         //clock input of HX711
HX711 scale;

/************************* VERSION Info *********************************/
#define VERSION_NO "1.0"

/************************* EEPROM Setup *********************************/
#include <EEPROM.h>
#define EEP_CAL_FLAG_ADDR  0		//eeprom calibration flag address, occupies 1 byte
#define EEP_CAL_DATA_ADDR  1		//eeprom calibration data address; occupies 4 bytes
byte eepCalFlag = 0;
long calibrationFactor_l = 535; //initial guess (for 0-5kg uxcell cell)


/************************* SETUP *********************************/
void setup() {
  Serial.begin(9600);
  Serial.print("OpenScale - version ");
  Serial.println(VERSION_NO);
  delay(2500);
  scale.begin(DOUT_PIN, CLK_PIN);
  if (queryCalibration()) {
    calibrateOpenScale();  // calibrate scale
  }
  else {
    zeroOutScale();	// zero out (TARE) scale
  }
  Serial.println("Begin OpenScale Readings");  // read scale
  delay(2500);
}


/************************* MAIN *********************************/
void loop() {
  //read measurement; if user enter "R" or "r" reset back to TARE process and back to measurements
  readOpenScale();
  char temp = Serial.read();
  if (temp == 'R' || temp == 'r') {
    zeroOutScale();
  }
}


/******************** IS CALIBRATION REQUIRED **************************/
boolean queryCalibration() {
  //get cal value from eeprom if exists
  boolean queryCalStatus = true;
  boolean skipCal = false;
  eepCalFlag = EEPROM.read(EEP_CAL_FLAG_ADDR);
  if (eepCalFlag == 128) {
    EEPROM.get(EEP_CAL_DATA_ADDR, calibrationFactor_l);
    Serial.println("CAL Data Exists, Skip Cal Sequence (Y/N)");
    float calibrationFactor_f = calibrationFactor_l;
    scale.set_scale(calibrationFactor_f);  //Adjust to this calibration factor
    while (queryCalStatus) {
      if (Serial.available()) {
        char temp = Serial.read();
        if (temp == 'Y' || temp == 'y') {
          skipCal = true;
          queryCalStatus = false;
        }
        else if (temp == 'N' || temp == 'n') {
          skipCal = false;
          queryCalStatus = false;
        }
      }
    }
  }
  if (skipCal) {
    return false;
  }
  else {
    return true;
  }
}


/************************ ZERO OUT SCALE *******************************/
void zeroOutScale() {
  Serial.println("Remove all weight from scale");
  delay(5000);
  Serial.println("Zeroing (tare) scale...");
  delay(2500);
  scale.tare();	//reset the scale to 0
  Serial.println("Scale zeroed ... Place desired weight on scale");
}


/************************* HX711 CALIBRATION *********************************/
void calibrateOpenScale() {
  //initialize calibration
  eepCalFlag = EEPROM.read(EEP_CAL_FLAG_ADDR);
  //get cal value from eeprom if exists
  if (eepCalFlag == 128) {
    EEPROM.get(EEP_CAL_DATA_ADDR, calibrationFactor_l);
  }

  Serial.println("OpenScale calibration sketch");
  Serial.println("Remove all weight from scale");
  delay(5000);
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press a to increase calibration factor by 10");
  Serial.println("Press d to increase calibration factor by 100");
  Serial.println("Press g to increase calibration factor by 1000");
  Serial.println("Press z to decrease calibration factor by 10");
  Serial.println("Press c to decrease calibration factor by 100");
  Serial.println("Press b to decrease calibration factor by 1000");
  Serial.println("Press x to exit calibration");
  delay(5000);

  float calibrationFactor_f = calibrationFactor_l;

  scale.set_scale();  //reset scale to default (0)
  scale.tare();  //reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading

  // This can be used to remove the need to tare the scale.
  // Useful in permanent scale projects.
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);

  boolean calibrateFlag = true;
  while (calibrateFlag) {
    scale.set_scale(calibrationFactor_f);  //Adjust to this calibration factor
    Serial.print("Reading (grams, raw, cal factor): ");
    Serial.print(scale.get_units(4), 3);
    Serial.print(" , ");
    Serial.print(scale.read_average());
    Serial.print(" , ");
    Serial.print(calibrationFactor_f);
    Serial.println();

    if (Serial.available()) {
      char temp = Serial.read();
      if (temp == 'a') {
        calibrationFactor_f += 10;
      }
      else if (temp == 'd') {
        calibrationFactor_f += 100;
      }
      else if (temp == 'g') {
        calibrationFactor_f += 1000;
      }
      else if (temp == 'z') {
        calibrationFactor_f -= 10;
      }
      else if (temp == 'c') {
        calibrationFactor_f -= 100;
      }
      else if (temp == 'b') {
        calibrationFactor_f -= 1000;
      }
      else if (temp == 'x') {
        calibrateFlag = false;
      }
    }
  }

  //exit calibration and store calibration in EEPROM
  {
    eepCalFlag = 128;
    EEPROM.write(EEP_CAL_FLAG_ADDR, eepCalFlag);
    calibrationFactor_l = calibrationFactor_f;
    EEPROM.put(EEP_CAL_DATA_ADDR, calibrationFactor_l);

    Serial.println("Exiting calibration sequence");
  }
}


/************************* READ HX711 *********************************/
void readOpenScale() {
  Serial.print("Reading: ");
  Serial.print(scale.get_units(AVG_FACT), RESOLUTION); //scale.get_units() returns float
  Serial.print(" grams");  //grams or kilograms depending on load cell
  Serial.println();
  delay(1000);
}

