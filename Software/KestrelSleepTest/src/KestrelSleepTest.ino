/*
 * Project KestrelSleepTest
 * Description:
 * Author:
 * Date:
 */

// #include <MCP23018.h> //Use for <v1.5
#include <PACL9535A.h> //Use for >=v1.5
#include "SparkFun_Ublox_Arduino_Library.h"

SFE_UBLOX_GPS myGPS;
PACL9535A ioAlpha(0x20); //Use for >=v1.5
PACL9535A ioBeta(0x21); //Use for >=v1.5
// MCP23018 ioAlpha(0x20); //Use for <v1.5

const uint8_t RTC_INT = D22; //FIX!
const uint8_t CSA_EN = 14; //FIX!
const uint8_t LED_EN = 13; //FIX!
const uint8_t AUX_EN = 15; //FIX!
// const uint8_t I2C_OB_EN = A6; //FIX!

const uint8_t FRAM_ADR = 0x50; 
const uint8_t ACCEL_ADR = 0x15;

const uint8_t I2C_OB_EN = D21;
const uint8_t I2C_GLOBAL_EN = D23;

SYSTEM_MODE(AUTOMATIC);

void setup() {
  pinMode(I2C_OB_EN, OUTPUT);
  digitalWrite(I2C_OB_EN, HIGH);
  Wire.begin();
  
  ioAlpha.begin();
  ioBeta.begin();
  ioBeta.clearInterrupt(PACL9535A::IntAge::CURRENT);
  ioAlpha.pinMode(12, OUTPUT); //LOW
  ioAlpha.pinMode(11, OUTPUT); //LOW
  ioAlpha.pinMode(10, OUTPUT); //LOW
  ioAlpha.pinMode(9, INPUT); 
  ioAlpha.pinMode(8, INPUT);
  ioAlpha.pinMode(7, INPUT);
  ioAlpha.pinMode(6, INPUT);
  ioAlpha.pinMode(5, INPUT);
  ioAlpha.pinMode(4, INPUT);
  ioAlpha.pinMode(3, INPUT);
  ioAlpha.pinMode(2, INPUT); //DEBUG! Return to INPUT_PULLUP
  ioAlpha.pinMode(1, INPUT);
  ioAlpha.pinMode(0, INPUT);

  ioAlpha.pinMode(CSA_EN, OUTPUT);
  ioAlpha.pinMode(LED_EN, OUTPUT);
  ioAlpha.pinMode(AUX_EN, OUTPUT);
  ioAlpha.digitalWrite(AUX_EN, HIGH); //DEBUG!
  myGPS.begin();

  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  // pinMode(I2C_OB_EN, OUTPUT);
  // digitalWrite(I2C_OB_EN, HIGH); //Enable onboard communication 
}

void loop() {

  //Clear Clock, config OSC
  Wire.beginTransmission(0x6F);
  Wire.write(0x07);
  Wire.write(0x88); //Turn on EXT OSC, turn off alarms
  Wire.endTransmission();

  Wire.beginTransmission(0x6F);
  Wire.write(0x0D);
  Wire.write(0x88); //Clear ALARM0
  Wire.endTransmission();

  Wire.beginTransmission(0x6F);
  Wire.write(0x14);
  Wire.write(0x88); //Clear ALARM1
  Wire.endTransmission();

  // //Shutdown Accel
	Wire.beginTransmission(ACCEL_ADR);
	Wire.write(0x0D); //Write to control register
	Wire.write(0x01); //Set power down
	Wire.endTransmission();

  //Shutdown FRAM
	Wire.beginTransmission(0xA0);
	Wire.write((FRAM_ADR << 1) | 0x01); //Shift to add "r/w" bit //FIX!
	Wire.endTransmission(false);
	Wire.beginTransmission(0x43);
	Wire.endTransmission();

  //Use for <v1.5
  // Wire.beginTransmission(0x20);
  // Wire.write(0x0A);
  // Wire.write(0b01000000); //Set INT pins to be ORed and driven
  // Wire.endTransmission();

  // Wire.beginTransmission(0x21);
  // Wire.write(0x0A);
  // Wire.write(0b01000000); //Set INT pins to be ORed and driven
  // Wire.endTransmission();

  ioAlpha.digitalWrite(12, LOW);
  ioAlpha.digitalWrite(11, LOW);
  ioAlpha.digitalWrite(10, LOW);
  ioAlpha.digitalWrite(CSA_EN, LOW); //Turn off CSAs
  ioAlpha.digitalWrite(LED_EN, HIGH); //Turn off LEDs for ultra low power
  ioAlpha.digitalWrite(AUX_EN, LOW); //Turn off auxiliary power
  // ioAlpha.digitalWrite(AUX_EN, HIGH); //DEBUG!
  // myGPS.powerOff(60000);

  digitalWrite(I2C_OB_EN, LOW);

  // SystemSleepConfiguration config;
	// 	config.mode(SystemSleepMode::ULTRA_LOW_POWER)
	// 	.gpio(RTC_INT, FALLING);

  SystemSleepConfiguration config;
		config.mode(SystemSleepMode::ULTRA_LOW_POWER)
		.gpio(RTC_INT, FALLING);
    // .gpio(A4, RISING)
    // .gpio(A5, RISING);
		SystemSleepResult result = System.sleep(config);
}