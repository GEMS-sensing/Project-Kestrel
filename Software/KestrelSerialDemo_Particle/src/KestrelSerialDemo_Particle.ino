// #include <Wire.h>
 // #include "Adafruit_MCP23008.h"
// #include <Adafruit_MCP23X17.h>
// #include <MCP2301x.h>
#include <Arduino.h>
#include <Particle.h>
#include "dct.h"
#include <MCP23018.h>

// #define NUM_MCP2301x_ICs   1

// const uint8_t I2C_GLOBAL_EN = 0; //FIX!

namespace Pins { //Use for B402
	// constexpr uint16_t WD_HOLD  = 12;
	// constexpr uint16_t EXT_EN   = 13;
	// constexpr uint16_t SD_EN    = 1;
	constexpr uint16_t SD_CS    = D8;
	// constexpr uint16_t SD_CD    = 18;
	// constexpr uint16_t SDA_CTRL = 5;
	// constexpr uint16_t TIP      = 18;
	// constexpr uint16_t GPIOXA 	= 17;
	constexpr uint16_t Clock_INT 	= D22;
	// constexpr uint16_t Port1D 	= 11;
	// constexpr uint16_t Port2D 	= 10;
	// constexpr uint16_t Port3D  	= 9;
	// constexpr uint16_t Port1A 	= A2;
	// constexpr uint16_t Port2A 	= A1;
	// constexpr uint16_t Port3A 	= A0;
	constexpr uint16_t TALON1_GPIOA = A3;
	constexpr uint16_t TALON1_GPIOB = D7;
	constexpr uint16_t TALON2_GPIOA = A2;
	constexpr uint16_t TALON2_GPIOB = D6;
	constexpr uint16_t TALON3_GPIOA = A1;
	constexpr uint16_t TALON3_GPIOB = D5;
	constexpr uint16_t I2C_GLOBAL_EN = D23; //FIX!
	constexpr uint16_t I2C_OB_EN = A6; //FIX!
}

namespace PinsIO { //For Kestrel v1.1
	constexpr uint16_t VUSB = 5;
}

namespace PinsIOAlpha {
	constexpr uint16_t I2C_EXT_EN = 10;
	constexpr uint16_t SD_CD = 8;
	constexpr uint16_t SD_EN = 12;
	constexpr uint16_t AUX_EN = 15;
	constexpr uint16_t CE = 11;
}

namespace PinsIOBeta { //For Kestrel v1.1
	constexpr uint16_t SEL1 = 0;
	constexpr uint16_t SEL2 = 4;
	constexpr uint16_t SEL3 = 8;
	constexpr uint16_t SEL4 = 12;
	constexpr uint16_t I2C_EN1 = 1;
	constexpr uint16_t I2C_EN2 = 5;
	constexpr uint16_t I2C_EN3 = 9;
	constexpr uint16_t I2C_EN4 = 13;
	constexpr uint16_t EN1 = 3;
	constexpr uint16_t EN2 = 7;
	constexpr uint16_t EN3 = 11;
	constexpr uint16_t EN4 = 15;
	constexpr uint16_t FAULT1 = 2;
	constexpr uint16_t FAULT2 = 6;
	constexpr uint16_t FAULT3 = 10;
	constexpr uint16_t FAULT4 = 14;
}
// const uint8_t TX = 1;
// const uint8_t RX = 0;
// const uint8_t FOut = 1;
// const uint8_t Dir = 0;

//IO Expander pins
const uint8_t POS_DETECT = 12; 
const uint8_t SENSE_EN = 13;

//IO Sense pins
const uint8_t MUX_EN = 3;
const uint8_t MUX_SEL0 = 0;
const uint8_t MUX_SEL1 = 1;
const uint8_t MUX_SEL2 = 2;

const float VoltageDiv = 2; //Program voltage divider
const float CurrentDiv = 0.243902439; //82mOhm, 50V/V Amp

//Configured for pins on the Particle Boron
//const uint8_t EXT_EN = 8;
//const uint8_t SDA_CTRL = 2;

 const uint8_t I2C_EN = 13;

char ReadArray[25] = {0};

 // Adafruit_MCP23008 io;
// Adafruit_MCP23X17 io;
MCP23018 ioAlpha(0x20);
MCP23018 ioBeta(0x21);
// MCP2301x io;

// #define IOEXP_MODE  (IOCON_INTCC | IOCON_INTPOL | IOCON_ODR | IOCON_MIRROR)
// #define ADDRESS      (MCP2301X_CTRL_ID+7)
// #define LED_PIN      D4   // WeMOS D1 & D1 mini
// #define IN_PORT      0
// #define OUT_PORT     1

// #define RELAY_ON      LOW
// #define RELAY_OFF     HIGH
// #define ALL_OFF       ALL_LOW

// #define DIR 0x00
// #define GPIO 0x12
// #define LAT 0x14
// #define PU 0x0C
// #define ADR 0x20


//////////////// GPS /////////////////
#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
SFE_UBLOX_GPS GPS;

/////////////// VEML3328 //////////////
#include "VEML3328.h"

VEML3328 ALS;

////////////// SHTC3 ////////////////
#include "SHTC3.h" 

SHTC3 RH; //Instatiate RH sensor

///////////// MXC6655 (Accel) ////////////////
#include <math.h>

//////////// MMC5633 (Mag) ////////////////

//////////// CSA ///////////////////
#include <PAC1934.h>
// PAC1934 CSA_Alpha(10, 10, 2, 2, 0x10); //Initalize with R32 = 10m, R34 = 10m, R23 = 2m, R10 = 2m, ADR = 0x10
// PAC1934 CSA_Beta(2, 10, 2, 2, 0x14); //Initalize with R6 = 2m, R15 = 10m, R7 = 2m, R8 = 2m, ADR = 0x11

PAC1934 CSA_Alpha(2, 2, 2, 2, 0x18); //Initalize with R32 = 10m, R34 = 10m, R23 = 2m, R10 = 2m, ADR = 0x10
PAC1934 CSA_Beta(2, 10, 10, 10, 0x14); //Initalize with R6 = 2m, R15 = 10m, R7 = 2m, R8 = 2m, ADR = 0x14

/////////// SD /////////////////
#include <SdFat.h>
#include <SPI.h>
SdFat SD;
char FileNameC[11]; //Used for file handling
boolean SD_Init = false;

/////////// FRAM /////////////

/////////// RTC /////////////
#include "MCP7940.h" //FIX! Update
MCP7940 Clock; // Instantiate Clock


//////////// LED ////////////
#include <PCA9634.h>

PCA9634 led(0x52); //Instatiate LED driver with 0x50 address

/////////////// SDI-12 Talon //////////////////////
//Input Pins
#include <MCP3221.h>
MCP3221 apogeeADC;
const uint8_t TX_SDI12 = 9;
const uint8_t RX_SDI12 = 10;

//IO Expander Pins
const uint8_t FOut = 14;
const uint8_t Dir = 15;

MCP23018 ioTalonSDI12(0x22);

#define MARKING_PERIOD 9 //>8.33ms for standard marking period
const unsigned long TimeoutStandard = 380; //Standard timeout period for most commands is 380ms 

/////////////// AUX Talon //////////////////////
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x49); 

MCP23018 ioAUXAlpha(0x20);
MCP23018 ioAUXBeta(0x23);
MCP23018 ioAUXGamma(0x24);

//IO Expander Pins - ALPHA
const uint8_t REG_EN = 7;
const uint8_t ADC_INT = 6;

//IO Expander Pins - BETA
const uint8_t LOAD = 7;
const uint8_t RST = 6;
const uint8_t OUT1 = 0;
const uint8_t COUNT_EN1 = 3;
const uint8_t OVF1 = 8;

const uint8_t EN1 = 1;
const uint8_t FAULT1 = 0;

// const uint8_t MUX_EN = 11;
// const uint8_t MUX_SEL2 = 10;
// const uint8_t MUX_SEL1 = 9;
// const uint8_t MUX_SEL0 = 8;

/////////////// HAAR (Sensor1) ////////////////////// 
#include <Dps368.h>
#include <Adafruit_SHT31.h>
#include <SparkFun_PCA9536_Arduino_Library.h>
#include <MCP3421.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <Adafruit_SGP30.h>


MCP23018 ioTalonI2C(0x22);

MCP3421 adcSense(0x6B); //Initialize MCP3421 with A3 address

PCA9536 ioSense;

Dps368 pressureSenseA = Dps368(); //Instantiate DPS368 Pressure sensor
Adafruit_SHT31 rhSenseA = Adafruit_SHT31(); //Instantiate the SHT31 RH/Temp sensor

Dps368 pressureSenseB = Dps368(); //Instantiate DPS368 Pressure sensor
Adafruit_SHT31 rhSenseB = Adafruit_SHT31(); //Instantiate the SHT31 RH/Temp sensor

SCD30 airSensor; //Instatiate SCD30 CO2 sensor
Adafruit_SGP30 sgp; //Instantiate SGP30 Gas Sensor

const uint8_t dps368_oversampling = 7; //Define the oversampling amount for the DPS368 sensor
unsigned long period = 5000; //Number of ms to wait between sensor readings 



SYSTEM_MODE(MANUAL);
// SYSTEM_MODE(AUTOMATIC); //DEBUG!

void setup() 
{
	
	Cellular.setActiveSim(INTERNAL_SIM);
	// Cellular.setCredentials("vzwinternet"); // Replace with the correct APN
	Cellular.setCredentials(""); // Go back to base APN
	const uint8_t val = 0x01;
	dct_write_app_data(&val, DCT_SETUP_DONE_OFFSET, 1);
	// pinMode(I2C_EN, OUTPUT); 
	// digitalWrite(I2C_EN, HIGH); //Turn on external port connection
	Serial.begin(115200);

	I2C_OnBoardEn(true); //RESTORE
	// Wire.setClock(10000); //DEBUG!
	Wire.setClock(400000); //Confirm operation in fast mode
	Wire.begin();
	// pinSetDriveStrength(D0, DriveStrength::HIGH);
	// pinSetDriveStrength(D1, DriveStrength::HIGH);
	ioAlpha.begin(); //RESTORE
	ioBeta.begin(); //RESTORE
	// ioAlpha.pinMode(PinsIOAlpha::CE, OUTPUT);
	// ioAlpha.digitalWrite(PinsIOAlpha::CE, HIGH); //DEBUG! Turn off charging 

	SetDefaultPins();
	// SetPinMode();
	// SetPullups();
	// io.digitalWrite(6, HIGH); //Turn on 5V
	// io.digitalWrite(7, HIGH); //Turn on 12V
	// io.digitalWrite(2, LOW); //Enable Data 1
	// io.digitalWrite(3, LOW); //Enable Data 2
	// io.digitalWrite(4, LOW); //Enable Data 3

	pinMode(TX_SDI12, OUTPUT); //DEBUG1!
	// ioTalonSDI12.pinMode( Dir, OUTPUT); //Set to output 
	// ioTalonSDI12.pinMode( FOut, OUTPUT); //Set to output 
	// ioTalonSDI12.digitalWrite( Dir, HIGH); //Set to transmit 
//  io.digitalWrite( FOut, LOW); //DEBUG!
	// Serial1.begin(1200, SERIAL_7E1);
	Serial1.begin(1200, SERIAL_8N1);
	delay(1000);
	Serial.println("Begin I2C Demo...");
}

void loop() 
{
	// io.pinMode(0, TX, LOW);
	// io.pinMode(0, FOut, LOW);
	// delay(1000);
	// io.pinMode(0, FOut, LOW);
	// io.pinMode(0, TX, HIGH);
	// delay(1000);
	// io.pinMode(0, FOut, HIGH);
	// io.pinMode(0, TX, LOW);
	// delay(1000);
	// io.pinMode(0, FOut, HIGH);
	// io.pinMode(0, TX, HIGH);
	// delay(1000);
	static int ReadLength = 0;
  	String ReadString;
	if(Serial.available() > 0) {
    char Input = Serial.read();

    // Increment counter while waiting for carrage return or newline
    // if(Input != 13 || Input != 10) {
    if(Input != '#' && Input != '!' && Input != '*') { //Wait for control, data, or SDI-12 line end
      ReadArray[ReadLength] = Input;
      ReadLength++;
    }

    // if(Input == 13 || Input == 10) { // carriage or newline
    if(Input == '!') { //Command line ending
		// ReadArray[ReadLength] = Input; //Append final character
		// ReadLength++;

		// ReadString = String(ReadArray);
		// ReadString.trim();
		// memset(ReadArray, 0, sizeof(ReadArray));
		// ReadLength = 0;
		// // int Pin = (ReadString.substring(0,1)).toInt(); //Grab the pin to operate on

		// Serial.print(">");
		// Serial.println(ReadString); //Echo back to serial monitor

		// switch(Pin) { //Call the interface function for the appropriate sensor to test
		// 	case 0:
		// 		VoltageSense();
		// 		break;
		// 	case 1:
		// 		Sensor1();
		// 		break;
		// 	case 2:
		// 		Sensor2();
		// 		break;
		// 	case 3:
		// 		Sensor3();
		// 		break;
		// 	case 4:
		// 		Sensor4();
		// 		break;
		// 	default:
		// 		Serial.println(">You didn't say the magic word!"); 
		// }

		ReadArray[ReadLength] = Input; //Append final character
		ReadLength++;

		ReadString = String(ReadArray);
		ReadString.trim();
		memset(ReadArray, 0, sizeof(ReadArray));
		ReadLength = 0;

		// while(Serial.peek() != '\n'); //Wait for new command
		// String NewCommand = Serial.readStringUntil('\n');
		Serial.print(">");
		Serial.println(ReadString); //Echo back to serial monitor
		Serial.println(SendCommand(ReadString));
		
	}

	if(Input == '*') {
		ReadString = String(ReadArray);
		ReadString.trim();
		memset(ReadArray, 0, sizeof(ReadArray));
		ReadLength = 0;

		Serial.print(">");
		Serial.println(ReadString); //Echo back to serial monitor

		if(ReadString.equalsIgnoreCase("Cell")) {
			TestCell(); //Connect to network and test cell status
		}

		if(ReadString.equalsIgnoreCase("GPS")) {
			TestGPS(); //Call GPS test
		}

		if(ReadString.equalsIgnoreCase("ALS")) {
			TestALS(); //Call ALS test
		}

		if(ReadString.equalsIgnoreCase("Atmos")) {
			TestAtmos(); //Call Temp/pressure/RH test
		}

		if(ReadString.equalsIgnoreCase("Accel")) {
			TestAccel(); //Call Accelerometer test
		}

		if(ReadString.equalsIgnoreCase("Mag")) {
			TestMag(false, false); //Call Magnetometer test (no offset compensation)
			TestMag(true, false); //Call Magnetometer test SET
			TestMag(false, true); //Call Magnetometer test RESET
		}

		if(ReadString.equalsIgnoreCase("CSA")) {
			TestCSA(); //Call CSA test
		}

		if(ReadString.equalsIgnoreCase("SD")) {
			TestSD(); //Call SD test
		}

		if(ReadString.equalsIgnoreCase("SDRead")) {
			ReadSD(); //Call SD test
		}

		if(ReadString.equalsIgnoreCase("FRAM")) {
			TestFRAM(false, false); //Call FRAM test
		}

		if(ReadString.equalsIgnoreCase("FRAMRead")) {
			TestFRAM(true, false); //Read the last FRAM write location
		}

		if(ReadString.equalsIgnoreCase("FRAMReadAll")) {
			TestFRAM(false, true); //Dump all FRAM data, Serial Bomb Incoming...
		}

		if(ReadString.equalsIgnoreCase("RTC")) {
			TestRTC(false, false); //Call RTC test
			//Include EEPROM??
		}

		if(ReadString.equalsIgnoreCase("RTCRead")) {
			TestRTC(true, false); //Read the last RTC EEPROM/SRAM data
			//Include EEPROM??
		}

		if(ReadString.equalsIgnoreCase("RTCReadAll")) {
			TestRTC(false, true); //Dump all EEPROM/SRAM data, Serial Bomb Incoming...
			//Include EEPROM??
		}


		if(ReadString.equalsIgnoreCase("Time")) {
			ReadRTC(); //Call RTC read only
			//Include EEPROM??
		}

		if(ReadString.equalsIgnoreCase("LED")) {
			TestLED(); //Call LED test
		}

		if(ReadString.equalsIgnoreCase("Battery")) {
			TestBattery(); //Read battery state from Gonk 
		}

		if(ReadString.equalsIgnoreCase("USB")) {
			TestUSB(); //Run USB test 
		}

		if(ReadString.equalsIgnoreCase("I2C")) {
			TestI2C(); //Run I2C scan 
		}

		if(ReadString.equalsIgnoreCase("Sensor1")) {
			SensorDemo1(); //Run sensor 1 demo  
		}

		if(ReadString.equalsIgnoreCase("Sensor2")) {
			SensorDemo2(); //Run sensor 2 demo  
		}

		if(ReadString.equalsIgnoreCase("Sensor3")) {
			SensorDemo3(); //Run sensor 3 demo  
		}

	}
	//Format (reduced) - ppCs#
	//pp - two digit pin number (base 10)
	//C - command ('M' - Mode, 'R' - Read, 'W' - Write)
	//s - state (0 - Output/Off, 1 - Input/On), required only for write and mode operations 

	//Format (extended) - DppCs#
	//D - device you wish to address ('A' - I/O Exp Alpha, 'B' - I/O Exp Beta, 'D' - Default aka Controller)
	//pp - two digit pin number (base 10)
	//C - command ('M' - Mode, 'R' - Read, 'W' - Write)
	//s - state (0 - Output/Off, 1 - Input/On), required only for write and mode operations 
	if(Input == '#') { //Control line ending
		ReadString = String(ReadArray);
		ReadString.trim();
		memset(ReadArray, 0, sizeof(ReadArray));
		ReadLength = 0;
		int Pin = 0;
		int State = 0;
		int Result = 0; //Used to grab result of either setting of pin mode or response from a read
		char Device = 'D'; //Specify default device 
		if(ReadString.charAt(0) < '0' || ReadString.charAt(0) > '9') { //If first digit is not a number, assume providing extended set 
			Device = ReadString.charAt(0); 
			ReadString.remove(0, 1); //Clear the leading character so it can be read normally 
		}
		Pin = (ReadString.substring(0,2)).toInt(); //Grab the pin to operate on
		String Operation = ReadString.substring(2,3); //Grab the middle char
		State = (ReadString.substring(3,4)).toInt(); //Grab the state to set
		

		// digitalWrite(I2C_EN, LOW); //Turn off external port connection
		if(Operation.equals("M")) { //if call is for pinmode setting
			switch(Device) { 
				case 'D':
					// pinMode(Pin, State); //Use normal state logic 
					if(State == 0) pinMode(Pin, OUTPUT);
          			if(State == 1) pinMode(Pin, INPUT);
          			Result = 0; //Set manually
					break;
				case 'A':
					Result = ioAlpha.pinMode(Pin, !State); //Use inverse of state to corespond with 1 = input, 0 = output
					break;
				case 'B':
					Result = ioBeta.pinMode(Pin, !State); //Use inverse of state to corespond with 1 = input, 0 = output
					break;
			}
		}
		
		if(Operation.equals("R")) { //If call is for digital read
			switch(Device) {
				case 'D':
					digitalRead(Pin);
					Result = 0; //Set manually
					break;
				case 'A':
					Result = ioAlpha.digitalRead(Pin);
					break;
				case 'B':
					Result = ioBeta.digitalRead(Pin);
					break;
			}
		}

		if(Operation.equals("W")) { //If call is for digital write
			switch(Device) {
				case 'D':
					// digitalWrite(Pin, State);
					if(State == 0) digitalWrite(Pin, LOW);
          			if(State == 1) digitalWrite(Pin, HIGH);
					Result = State; //Set manually //DEBUG!
					break;
				case 'A':
					Result = ioAlpha.digitalWrite(Pin, State);
					break;
				case 'B':
					Result = ioBeta.digitalWrite(Pin, State);
					break;
			}
		}
		// digitalWrite(I2C_EN, HIGH); //Turn on external port connection
		// while(Serial.peek() != '\n'); //Wait for new command
		// String NewCommand = Serial.readStringUntil('\n');
		Serial.print(">");
		Serial.println(ReadString); //Echo back to serial monitor
		Serial.print(">");
		Serial.println(Result); //Return various result 
	}
	// GetAddress();
	// delay(5000);

	}
}

// void VoltageSense() //Voltage sense (AKA Sensor0)
// {
// 	Serial.print(">Voltage Sense:\n");
// 	digitalWrite(I2C_EN, LOW); //Turn off external I2C
// 	io.digitalWrite(SENSE_EN, HIGH); //Make sure sense power is turned on
// 	ioSense.begin(); //Initalize voltage sensor IO expander
// 	for(int i = 0; i < 4; i++) { //Set all pins to output
// 		ioSense.pinMode(i, OUTPUT); 
// 	}
// 	ioSense.digitalWrite(MUX_EN, HIGH); //Turn MUX on 
// 	int SenseError = adc.Begin(); //Initialize ADC 
// 	if(SenseError == 0) { //Only proceed if ADC connects correctly
// 		adc.SetResolution(18); //Set to max resolution (we paid for it right?) 

// 		ioSense.digitalWrite(MUX_SEL2, LOW); //Read voltages
// 		for(int i = 0; i < 4; i++){ //Increment through 4 voltages
// 			ioSense.digitalWrite(MUX_SEL0, i & 0b01); //Set with lower bit
// 			ioSense.digitalWrite(MUX_SEL1, (i & 0b10) >> 1); //Set with high bit
// 			delay(1); //Wait for voltage to stabilize
// 	  		Serial.print("\tPort");
// 	  		Serial.print(i);
// 	  		Serial.print(":");
// 	  		Serial.print(adc.GetVoltage(true)*VoltageDiv, 6); //Print high resolution voltage
// 	  		Serial.print(" V\n");  
// 		}
// 		ioSense.digitalWrite(MUX_SEL2, HIGH); //Read currents
// 		for(int i = 0; i < 4; i++){ //Increment through 4 voltages
// 			ioSense.digitalWrite(MUX_SEL0, i & 0b01); //Set with lower bit
// 			ioSense.digitalWrite(MUX_SEL1, (i & 0b10) >> 1); //Set with high bit
// 			delay(1); //Wait for voltage to stabilize
// 	  		Serial.print("\tPort");
// 	  		Serial.print(i);
// 	  		Serial.print(":");
// 	  		Serial.print(adc.GetVoltage(true)*CurrentDiv*1000, 6); //Print high resolution current measure in mA
// 	  		Serial.print(" mA\n");  
// 		}
// 	}
// 	else Serial.print("\tFAIL!\n");
// 	digitalWrite(I2C_EN, HIGH); //Turn on external I2C
// }


// void Sensor1() 
// {
// 	// Serial.println(">Spector1");
// 	/////////////// SETUP /////////////////////
// 	uint8_t error_dps368 = 0; //DEBUG! FIX! Change library to return stuff properly
// 	pressureSense.begin(Wire); //Initialize DPS368 sensor //DEBUG! FIX! Change library to return stuff properly
// 	uint8_t error_sht31 = rhSense.begin(0x44); //Initialize SHT31 sensor
	
// 	Serial.println(">Haar Status:"); //Print resulting sensor status 
// 	Serial.print("\tDPS368: "); 
// 	if(error_dps368 == 0) Serial.print("PASS\n"); //Print error grabbed from sensor
// 	else Serial.print("FAIL!\n"); //Fail if result is not 0
// 	Serial.print("\tSHT31: ");
// 	if(error_sht31 == 1) Serial.print("PASS\n"); //Print error grabbed from sensor
// 	else Serial.print("FAIL!\n"); //Fail if result is 0

// 	///////////////// RUN ////////////////////
// 	float temp_dps368; //Storage for temp measurement from DPS368 
//   	float pressure; //Storage for pressure measurment from DPS368
  	

// 	int16_t error1 = pressureSense.measureTempOnce(temp_dps368, dps368_oversampling); //Grab new temp values [°C]
// 	int16_t error2 = pressureSense.measurePressureOnce(pressure, dps368_oversampling); //Grab new pressure values [Pa]
// 	pressure = pressure/100.0f; //Convert from Pa to mBar, because Pa is a stupid unit, and hPa is more stupid. 

// 	float humidity = rhSense.readHumidity(); //Grab new humidity values [%]
// 	float temp_sht31 = rhSense.readTemperature(); //Grab new temp values [°C]

// 	Serial.print("\tTemp (SHT31): "); Serial.print(temp_sht31, 4); Serial.print(" °C\t"); //Print temp to 4 decimal places
// 	Serial.print("Temp (DPS368): "); Serial.print(temp_dps368, 2); Serial.print(" °C\t"); //Print temp to 2 decimal places
// 	Serial.print("Pressure: "); Serial.print(pressure, 2); Serial.print(" mBar\t"); //Print pressure to 2 decimal places
// 	Serial.print("Humidity: "); Serial.print(humidity, 2); Serial.print(" %\n"); //Print humidity to 2 decimal places
// }

// void Sensor2() 
// {
// 	// Serial.println(">Spector2");
// 	Wire.begin();

// 	Serial.println(">Hedorah Status:");
// 	Serial.print("\tSCD30: ");
// 	if (airSensor.begin() == false)
// 	{
// 		Serial.print("FAIL!\n");
// 	}
// 	else {
// 		Serial.print("PASS\n");
// 		Serial.print("\t...");
// 		unsigned long LocalTime = millis();
// 		while(millis() - LocalTime < 10000){
// 			if (airSensor.dataAvailable())
// 			{
// 				Serial.print("\tCO2: "); //Print sensor CO2 reading
// 				Serial.print(airSensor.getCO2());
// 				Serial.print(" PPM\t");

// 				Serial.print("Temp:"); //Print sensor temp
// 				Serial.print(airSensor.getTemperature(), 1);
// 				Serial.print(" °C\t");

// 				Serial.print("RH:"); //Print sensor RH
// 				Serial.print(airSensor.getHumidity(), 1);
// 				Serial.print(" %\n");
// 				break; //Exit while loop
// 			}
// 		}
// 		if(millis() - LocalTime > 10000) Serial.print("\tERROR: Timeout\n"); //Report timeout error if time has elapsed on exit
// 	}

// }

// void Sensor3() 
// {

// }

// void Sensor4() 
// {

// }

void TestCell()
{
	// Serial.print("\tCellular On... ");
	// Cellular.on();
	// while(!Cellular.isOn()); //Wait until switched on
	// Serial.println("Done");
	// Serial.print("\tCellular Connect... ");
	// Cellular.connect(); //Connect to cloud
	// while(!Cellular.ready()); //Wait until connected
	// Serial.println("Done");
	delay(10000); //DEBUG!
	unsigned long LocalTime = millis();
	Serial.print("\tParticle Connect... ");
	Particle.connect();
	while(!Particle.connected()) {//Wait while connecting
		Particle.process();
	}
	Serial.println("Done");
	Serial.print("\tParticle Publish... ");
	Particle.publish("Kestrel - Bang");
	Serial.println("Done");
	Serial.print("\tGet Cell Stats... ");
	CellularSignal sig = Cellular.RSSI();
	float strength = sig.getStrength();
	float quality = sig.getQuality();
	Serial.println("Done");
	Serial.println("\tStats:");
	Serial.print("\t\tStrength: ");
	Serial.print(strength);
	Serial.println("%");
	Serial.print("\t\tQuality: ");
	Serial.print(quality);
	Serial.println("%");
	Serial.print("\t\tTTC: ");
	Serial.print(millis() - LocalTime);
	Serial.println(" ms");
}

void TestGPS() 
{
	
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	if (GPS.begin() == false) //Connect to the Ublox module using Wire port
	{
		Serial.println("\tERROR: MAX-M8W");
		// while (1);
	}
	else { //If connected, read position
		Wire.setClock(400000); //Confirm operation in fast mode
		GPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
	  	GPS.saveConfiguration(); //Save the current settings to flash and BBR
		unsigned long LocalTime = millis();
		// while((millis() - LocalTime) < 5000) //Wait up to 5 seconds for a reading
		// {
			long latitude = GPS.getLatitude();
			Serial.print("\tLat: ");
			Serial.println(latitude);

			long longitude = GPS.getLongitude();
			Serial.print("\tLong: ");
			Serial.println(longitude);
			// Serial.print(F(" (degrees * 10^-7)"));

			long altitude = GPS.getAltitude();
			Serial.print("\tAlt: ");
			Serial.print(altitude);
			Serial.println(" (mm)");

			byte SIV = GPS.getSIV();
			Serial.print("\tSIV: ");
			Serial.println(SIV);

			byte fixType = GPS.getFixType();
			Serial.print("\tFix: ");
			Serial.println(fixType);

			long precision = GPS.getPDOP();
			Serial.print("\tPDOP: ");
			Serial.print(precision);
			Serial.println(" [m * 10^-2]");
			

			String Time = String(GPS.getYear()) + "/" + String(GPS.getMonth()) + "/" + String(GPS.getDay()) + " " + String(GPS.getHour()) + ":" + String(GPS.getMinute()) + ":" + String(GPS.getSecond());
			Serial.print("\tTime: ");
			Serial.println(Time);

			// Serial.println();
		// }
	}
	I2C_GlobalEn(true);
}

void TestALS() 
{
	I2C_GlobalEn(false); //Turn off external I2C
	I2C_OnBoardEn(true); //Turn on local I2C
	if(ALS.begin() == 0) {
		Serial.print("\tStart Auto-Range...");
		ALS.AutoRange();
		Serial.println("Done");
		Serial.print("\tReading Sensor...");
		float Clear = ALS.GetValue(VEML3328::Channel::Clear);
		float Red = ALS.GetValue(VEML3328::Channel::Red);
		float Green = ALS.GetValue(VEML3328::Channel::Green);
		float Blue = ALS.GetValue(VEML3328::Channel::Blue);
		float Lux = ALS.GetLux();
		Serial.println("Done");
		
		Serial.print("\tClear: "); Serial.print(Clear, 4); Serial.println(" uW/cm^2");
		Serial.print("\tRed: "); Serial.print(Red, 4); Serial.println(" uW/cm^2");
		Serial.print("\tGreen: "); Serial.print(Green, 4); Serial.println(" uW/cm^2");
		Serial.print("\tBlue: "); Serial.print(Blue, 4); Serial.println(" uW/cm^2");
		Serial.print("\tAmbient: "); Serial.print(Lux, 4); Serial.println(" lx");
	}
	else Serial.println("\tERROR: VEML3328");
	I2C_GlobalEn(true); //Turn back on external I2C
	
}

void TestAtmos() 
{
	// I2C_GlobalEn(false); //Turn off external I2C
	// I2C_OnBoardEn(true); //Turn on local I2C
	// if(RH.begin() == 0) { //Setup RH with defaults
	// 	// Wire.begin();
	// 	// Wire.setClock(400000); //Confirm operation in fast mode
	// 	// Wire.setClock(10000); //DEBUG!
	// 	float AtmosData[2] = {0}; //Create array for envrio data
	// 	RH.Wake(false); //Wake RH sensor
	// 	RH.GetData(AtmosData); //Read data from sensor into array
	// 	RH.Sleep(); //Sleep sensor again to reduce power

	// 	Serial.print("\tHumidity = ");
	// 	Serial.print(AtmosData[0]); //Print RH
	// 	Serial.print(" %\t");
	// 	Serial.print("\tTemperature = ");
	// 	Serial.print(AtmosData[1]); //Print temp
	// 	Serial.print(" ºC\n"); 
	// }
	// else Serial.println("\tERROR: SHTC3");
	// // I2C_GlobalEn(true); //Turn back on external I2C

	// Wire.beginTransmission(0x44);
	// Wire.write(0xE0);
	// Wire.endTransmission();

	// delay(2);

	// // Wire.beginTransmission(0x44);
	// // Wire.write(0x00);
	// // Wire.endTransmission(false);

	// Wire.requestFrom(0x44, 6, true);
	// Serial.print("\tHumidity = \n");
	// for(int i = 0; i < 6; i++) {
	// 	Serial.print("\t\t0x");
	// 	Serial.println(Wire.read(), HEX);
	// }

	I2C_GlobalEn(false); //Turn off external I2C
	I2C_OnBoardEn(true); //Turn on local I2C
	ioAlpha.pinMode(PinsIOAlpha::AUX_EN, OUTPUT);
	ioAlpha.digitalWrite(PinsIOAlpha::AUX_EN, HIGH); //Turn on 3V3 AUX

	unsigned long Timeout = 10; //Wait up to 10ms for measurment response 
	const uint8_t ADR = 0x44; 
	const uint8_t MEASURE_LOW = 0xE0;
	const uint8_t MEASURE_MED = 0xF6;
	const uint8_t MEASURE_HIGH = 0xFD;

	Wire.beginTransmission(ADR);
	Wire.write(MEASURE_HIGH);
	Wire.endTransmission();
	delay(10); //Wait for measure 
	Wire.requestFrom(ADR, 6, true);
	unsigned long LocalTime = millis();
	while(Wire.available() < 6 && (millis() - LocalTime) < Timeout); //Wait until read in
	if(Wire.available() < 6) {
		Serial.println("ERROR: Timeout");
		return; //Exit if timeout
	}

	uint8_t Data[6] = {0}; 
	for(int i = 0; i < 6; i++) {
		Data[i] = Wire.read(); //Read in block of data 
	}

	int16_t TempRaw = (Data[0] << 8) | Data[1];
	uint8_t TempCRC = Data[2];
	uint16_t HumidityRaw = (Data[3] << 8) | Data[4];
	uint8_t HumidityCRC = Data[5];
	//FIX! Test CRC
	float Temp = (-45.0 + float(TempRaw)*0.0026703288); //Calc temp in °C - Note: 175/65535 = 0.0026703288
	float RH = (-6.0 + float(HumidityRaw)*0.0019073777); //Calc RH in % - Note: 125/65535 = 0.0019073777
	Serial.print("\tTemp: ");
	Serial.print(Temp);
	Serial.println("°C");
	Serial.print("\tRH: ");
	Serial.print(RH);
	Serial.println("%");


	// Wire.requestFrom(0x44, 3, true);
	// Serial.print("\tTemperature = ");
	// for(int i = 0; i < 3; i++) {
	// 	Serial.print("\t\t0x");
	// 	Serial.println(Wire.read(), HEX);
	// }
}

void TestMag(bool Set, bool Reset) 
{
	
	const uint8_t ADR = 0x30;
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	GPS.powerOff(60000); //DEBUG!
	// Wire.begin();
	// Wire.setClock(100000); //Confirm operation in standard mode

	uint16_t DataRaw[3] = {0}; //X, Y, Z accel data, raw
	float Data[3] = {0}; //Converted X, Y, Z accel data [g]
	
	Wire.beginTransmission(ADR);
	Wire.write(0x1C); //Write to Control Register 1
	// Wire.write(0x80); //Call for chip reset
	Wire.write(0x00); //Clear reg
	int Error = Wire.endTransmission();

	// delay(500);

	if(Set && !Reset) {
		Wire.beginTransmission(ADR);
		Wire.write(0x1B); //Write to Control Register 0
		// Wire.write(0x21); //Set both magnetic measurment bit and periodic measurment bit
		Wire.write(0x80); //Trigger SET
		// Wire.write(0x23); //Set both magnetic and temp measurment bits and periodic measurment bit
		Error = Wire.endTransmission();
		Serial.println("\tSET");
	}

	if(Reset && !Set) {
		Wire.beginTransmission(ADR);
		Wire.write(0x1B); //Write to Control Register 0
		// Wire.write(0x21); //Set both magnetic measurment bit and periodic measurment bit
		Wire.write(0x10); //Trigger RESET
		// Wire.write(0x23); //Set both magnetic and temp measurment bits and periodic measurment bit
		Error = Wire.endTransmission();
		Serial.println("\tRESET");
	}

	Wire.beginTransmission(ADR);
	Wire.write(0x1B); //Write to Control Register 0
	// Wire.write(0x21); //Set both magnetic measurment bit and periodic measurment bit
	Wire.write(0x21); //Set both magnetic measurment bit and reset bit
	// Wire.write(0x23); //Set both magnetic and temp measurment bits and periodic measurment bit
	Error = Wire.endTransmission();

	// if(Error != 0) {
	// 	Serial.print("\tERROR: MMC5633 - ");
	// 	Serial.println(Error);
	// }
	// else {
		unsigned long LocalTime = millis();
		bool NewData = false; //Used to keep track if new data has been recieved 
		uint8_t Status = 0x00; //Used to store temp status reading 
		// NewData = true; //DEBUG!
		// delay(100); //DEBUG!
		while(!NewData && (millis() - LocalTime) < 500) { //Wait untill new data or 10ms timeout 
			Wire.beginTransmission(ADR);
			Wire.write(0x18); //Read device status register 1
			Wire.endTransmission();

			Wire.requestFrom(ADR, 1); //Get status byte back
			Status = Wire.read();
			Serial.println(Status, HEX); //DEBUG!
			delay(5); //DEBUG!
			// if((Status & 0xDC) == 0x00) NewData = true; //Set flag if both measurment bits are cleared again //FIX! Adjust to be more general 
			if(((Status & 0x40) >> 6) == 0x01) NewData = true; //Set flag if mag measurment bit is cleared //FIX! Adjust to be more general 
			// if(((Status & 0x80) >> 7) == 0x01) NewData = true; //Set flag if temp measurment bit is cleared //FIX! Adjust to be more general 
		}
		if(NewData) {
			for(int i = 0; i < 3; i++) {
				Wire.beginTransmission(ADR);
				Wire.write(0x00 + i*2); //Address the begining of each data array
				Wire.endTransmission();

				Wire.requestFrom(ADR, 2);
				DataRaw[i] = Wire.read();
				DataRaw[i] = (DataRaw[i] << 8) | Wire.read();

				// Wire.beginTransmission(ADR);
				// Wire.write(0x06 + i); //Address the last 4 bits for each reading - weird addressing scheme 
				// Wire.endTransmission(true);

				// Wire.requestFrom(ADR, 1);
				// DataRaw[i] = (DataRaw[i] << 4) | (Wire.read() >> 4); //Align to low 4 bits and add

				// Data[i] = (float(DataRaw[i]) - 32768.0)/32.7680; //Divide by count per Gauss, subtract from mean, return results in mG FIX! Make more general, Need negative padding??
				Data[i] = (float(DataRaw[i]) - 32768.0)/1.024; //Divide by count per Gauss, subtract from mean, return results in mG FIX! Make more general, Need negative padding??
			}

			// //DEBUG!
			// Wire.beginTransmission(ADR);
			// Wire.write(0x00); //Address the begining of each data array
			// Wire.endTransmission(true);

			// Wire.requestFrom(ADR, 6);
			// Serial.println("\tData:");
			// for(int i = 0; i < 6; i++) {
			// 	Serial.print("\t\t");
			// 	Serial.println(Wire.read(), HEX);
			// }
			// //DEBUG!

			Serial.print("\tX: ");
			Serial.print(Data[0]);
			Serial.print("mG\t");
			Serial.print(DataRaw[0]);
			Serial.println(" Counts");

			Serial.print("\tY: ");
			Serial.print(Data[1]);
			Serial.print("mG\t");
			Serial.print(DataRaw[1]);
			Serial.println(" Counts");

			Serial.print("\tZ: ");
			Serial.print(Data[2]);
			Serial.print("mG\t");
			Serial.print(DataRaw[2]);
			Serial.println(" Counts");

			Serial.print("\tHeading: ");
			Serial.print(atan(Data[0]/Data[1])*180.0/3.14);
			Serial.println("°");
		}
		else Serial.println("ERROR: Timeout");
	// }


	I2C_GlobalEn(true);
}

void TestAccel() 
{
	const uint8_t ADR = 0x15; 
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);

	//Initialize
	Wire.beginTransmission(ADR);
	Wire.write(0x0D); //Write to control register
	Wire.write(0x00); //Make sure register is cleared - Self test off, FSR = 2g, Power down false
	Wire.endTransmission();

	// ////// RUN SELF TEST ///////
	// Wire.beginTransmission(ADR);
	// Wire.write(0x0B); 
	// Wire.write(0x80); //Disable temp comp
	// Wire.endTransmission();

	// Wire.beginTransmission(ADR);
	// Wire.write(0x0D);
	// Wire.write(0xC0); //Activate self test, 8G mode
	// Wire.endTransmission();
	// ////// RUN SELF TEST ///////

	int16_t DataRaw[3] = {0}; //X, Y, Z accel data, raw
	float Data[3] = {0}; //Converted X, Y, Z accel data [g]

	Wire.beginTransmission(ADR);
	Wire.write(0x03); //Address XOut register, increment from there
	Wire.endTransmission(); 

	Wire.requestFrom(ADR, 6); //Get all bytes at once
	for(int i = 0; i < 3; i++) {
		DataRaw[i] = (Wire.read() << 8); //Read in high byte 
		DataRaw[i] = DataRaw[i] | Wire.read(); //Read in low byte 
		DataRaw[i] = DataRaw[i] >> 4; //Shift out empty bits
		Data[i] = (float(DataRaw[i]/2048.0))*2; //Divide by 12 bit resolution, multiply by FSR
		// Data[i] = (float(DataRaw[i]/2048.0))*8; //Divide by 12 bit resolution, multiply by FSR = 8
	}
	//Grab Tilt/orientation bits //DEBUG!
	Wire.beginTransmission(ADR);
	Wire.write(0x01);
	Wire.endTransmission();
	Serial.print("\tINT_SRC1: ");
	Wire.requestFrom(ADR, 1);
	Serial.println(Wire.read(), BIN);

	Wire.beginTransmission(ADR);
	Wire.write(0x02);
	Wire.endTransmission();
	Serial.print("\tSTATUS: ");
	Wire.requestFrom(ADR, 1);
	Serial.println(Wire.read(), BIN);

	//Grab raw Z axis regs //DEBUG!
	Wire.beginTransmission(ADR);
	Wire.write(0x07);
	Wire.endTransmission();
	Serial.print("\tZOUT_Upper: ");
	Wire.requestFrom(ADR, 1);
	Serial.println(Wire.read(), HEX);

	Wire.beginTransmission(ADR);
	Wire.write(0x08);
	Wire.endTransmission();
	Serial.print("\tZOUT_Lower: ");
	Wire.requestFrom(ADR, 1);
	Serial.println(Wire.read(), HEX);



	//Print out results 
	Serial.print("\tX: ");
	Serial.print(Data[0], 6);
	Serial.print("g\t");
	Serial.println(DataRaw[0], HEX);

	Serial.print("\tY: ");
	Serial.print(Data[1], 6);
	Serial.print("g\t");
	Serial.println(DataRaw[1], HEX);

	Serial.print("\tZ: ");
	Serial.print(Data[2], 6);
	Serial.print("g\t");
	Serial.println(DataRaw[2], HEX);

	
	// DataRaw[1] = Wire.read() << 8; //Read in high byte of Y
	// DataRaw[1] = DataRaw[1] | Wire.read(); //Read in low byte of Y
	// DataRaw[2] = Wire.read() << 8; //Read in high byte of Z
	// DataRaw[2] = DataRaw[2] | Wire.read(); //Read in low byte of Z

	
	// DataRaw[1] = DataRaw[1] >> 4; //Shift out empty bits
	// DataRaw[2] = DataRaw[2] >> 4; //Shift out empty bits

	// Data[0] = (float(DataRaw[0]/2048.0))*2; //Divide by 12 bit resolution, multiply by FSR

	// //Shutdown
	// Wire.beginTransmission(ADR);
	// Wire.write(0x0D); //Write to control register
	// Wire.write(0x01); //Set power down
	// Wire.endTransmission();

	Wire.beginTransmission(ADR);
	Wire.write(0x0B); //Write to control register
	Wire.write(0x80); //Turn off temp compensation 
	Wire.endTransmission();

	I2C_GlobalEn(true);
}

void TestCSA() 
{
	I2C_GlobalEn(false); //Turn off external I2C
	I2C_OnBoardEn(true); //Turn on local I2C

	int ErrorAlpha = CSA_Alpha.begin();
	int ErrorBeta = CSA_Beta.begin();
	if(ErrorAlpha == 1 && ErrorBeta == 1) { //Only proceed if both passed initialization 
		CSA_Alpha.SetCurrentDirection(CH1, BIDIRECTIONAL); //Battery charge, bidirectional
		CSA_Alpha.SetCurrentDirection(CH2, UNIDIRECTIONAL); //System voltage, unidirectional
		CSA_Alpha.SetCurrentDirection(CH3, UNIDIRECTIONAL); //Prime voltage, unidirectional
		CSA_Alpha.SetCurrentDirection(CH4, UNIDIRECTIONAL); //Solar input, unidirectional

		CSA_Beta.SetCurrentDirection(CH1, UNIDIRECTIONAL); //Bulk voltage, unidirectional
		CSA_Beta.SetCurrentDirection(CH2, UNIDIRECTIONAL); //3v3 Core, unidirectional
		CSA_Beta.SetCurrentDirection(CH3, UNIDIRECTIONAL); //3v3 Aux, unidirectional
		CSA_Beta.SetCurrentDirection(CH4, UNIDIRECTIONAL); //3v3 Bulk, unidirectional

		bool UseAverage = true; //Set to use an 8 reading average, clear to report single measurement 
		//Get Alpha bank of sense values
		float VBat = CSA_Alpha.GetBusVoltage(CH1, UseAverage); //Grab battery voltage
		float IBat = CSA_Alpha.GetCurrent(CH1, UseAverage); //Grab battery current 

		float VSys = CSA_Alpha.GetBusVoltage(CH2, UseAverage); //Grab sys voltage rail
		float ISys = CSA_Alpha.GetCurrent(CH2, UseAverage); //Grab sys current

		float VPrime = CSA_Alpha.GetBusVoltage(CH3, UseAverage); //Grab prime voltage
		float IPrime = CSA_Alpha.GetCurrent(CH3, UseAverage); //Grab prime current 
		// float IPrime = CSA_Alpha.GetSenseVoltage(CH3, UseAverage); //DEBUG!

		float VSolar = CSA_Alpha.GetBusVoltage(CH4, UseAverage); //Grab solar voltage
		float ISolar = CSA_Alpha.GetCurrent(CH4, UseAverage); //Grab solar current

		//Get Beta bank of sense values
		float VBulk = CSA_Beta.GetBusVoltage(CH1, UseAverage); //Grab bulk voltage
		float IBulk = CSA_Beta.GetCurrent(CH1, UseAverage); //Grab bulk current

		float VCore = CSA_Beta.GetBusVoltage(CH2, UseAverage); //Grab 3v3 core voltage
		float ICore = CSA_Beta.GetCurrent(CH2, UseAverage); //Grab 3v3 core current 

		float VAux = CSA_Beta.GetBusVoltage(CH3, UseAverage); //Grab 3v3 Aux voltage
		float IAux = CSA_Beta.GetCurrent(CH3, UseAverage); //Grab 3v3 aux current

		float VBulk_3v3 = CSA_Beta.GetBusVoltage(CH4, UseAverage); //Grab 3v3 bulk voltage (tied to 3v3 aux voltage)
		float IBulk_3v3 = CSA_Beta.GetCurrent(CH4, UseAverage); //Grab 3v3 bulk current 

		//Print out all results 
		Serial.print("\tBat:\t\t");
		PrintVIFormat(VBat, IBat);

		Serial.print("\tSystem:\t\t");
		PrintVIFormat(VSys, ISys);

		Serial.print("\tPrime:\t\t");
		PrintVIFormat(VPrime, IPrime);

		Serial.print("\tSolar:\t\t");
		PrintVIFormat(VSolar, ISolar);

		Serial.print("\tBulk:\t\t");
		PrintVIFormat(VBulk, IBulk);

		Serial.print("\tCore:\t\t");
		PrintVIFormat(VCore, ICore);

		Serial.print("\tAux:\t\t");
		PrintVIFormat(VAux, IAux);

		Serial.print("\tBulk, 3v3:\t");
		PrintVIFormat(VBulk_3v3, IBulk_3v3);
	}

	else { //If either one fails, print out error states 
		Serial.print("\tERROR:\n\r");
		Serial.print("\t\tALPHA: ");
		if(ErrorAlpha == 1) Serial.println("PASS");
		else Serial.println("FAIL");
		// Serial.println(ErrorAlpha);
		Serial.print("\t\tBETA: ");
		if(ErrorBeta == 1) Serial.println("PASS");
		else Serial.println("FAIL");
	}

	I2C_GlobalEn(true); //Turn back on external I2C
}

void PrintVIFormat(float Voltage, float Current) 
{
	String Print = String(Voltage, 4) + "V, " + String(Current) + "mA\n\r"; //Format substring
	Serial.print(Print); //Print substring
}

boolean TestSD() 
{
	// pinMode(Pins::SD_EN, OUTPUT); //Ensure EN pin is setup as output
	// digitalWrite(Pins::SD_EN, HIGH); //Set enable pin to turn on power
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	ioAlpha.pinMode(PinsIOAlpha::SD_CD, INPUT);
	ioAlpha.pinMode(PinsIOAlpha::SD_EN, OUTPUT);

	ioAlpha.digitalWrite(PinsIOAlpha::SD_EN, HIGH); //Turn on 3v3 to SD

	boolean SDError = false;
	pinMode(Pins::SD_CS, OUTPUT);
	// SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
	SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0)); //Test max speed mode
	// SPI.setDataMode(SPI_MODE0);
	// SPI.setClockDivider(SPI_CLOCK_DIV2); //Sts SPI clock to 4 MHz for an 8 MHz system clock
	if(ioAlpha.digitalRead(PinsIOAlpha::SD_CD)) {
		Serial.println("\tERROR: SD Not Inserted");
		return SDError;
	}

	if(!SD_Init) {
		if (!SD.begin(Pins::SD_CS)) {
			Serial.println("\tERROR: Init Failed");
			SDError = true; 
			return SDError;
		}
		Serial.println("\tInitialized...");
		SD_Init = true;
	}
	String FileName = "HWTest";

	(FileName + ".txt").toCharArray(FileNameC, 11);
	SD.remove(FileNameC); //Remove any previous files

	randomSeed(millis()); //Seed with a random number to try to endsure randomness
	int RandVal = random(30557); //Generate a random number between 0 and 30557 (the number of words in Hamlet)
	char RandDigits[5] = {0};
	sprintf(RandDigits, "%d", RandVal); //Convert RandVal into a series of digits
	int RandLength = (int)((ceil(log10(RandVal))+1)*sizeof(char)); //Find the length of the values in the array

	File DataWrite = SD.open(FileNameC, FILE_WRITE);
	if(DataWrite) {
		DataWrite.println(RandVal);
		DataWrite.println("\nHe was a man. Take him for all in all.");
		DataWrite.println("I shall not look upon his like again.");
		DataWrite.println("-Hamlet, Act 1, Scene 2");
	}
	DataWrite.close();

	// if(Debug){
		Serial.print("\tRand Val = "); Serial.println(RandVal);
	// }

	char TestDigits[5] = {0};
	File DataRead = SD.open(FileNameC, FILE_READ);
	if(DataRead) {
		DataRead.read(TestDigits, RandLength);

		// if(Debug){
		  Serial.print("\tRead Val = ");
		// }
		for(int i = 0; i < RandLength - 1; i++){ //Test random value string
			Serial.print(TestDigits[i]);
			if(TestDigits[i] != RandDigits[i]) {
				SDError = true;
				Serial.print("x"); //DEBUG!
				Serial.print(RandDigits[i]);
			}
		}
		Serial.print("\n");

		// if(Debug) {
		//      byte Data = DataRead.read();
			uint8_t Data = 0; //Used to store read data temporarily 
			Serial.println("\tTest Data:");
			while(DataRead.available()) {
				Data = DataRead.read();
				if(Data != '\n') Serial.write(Data);
				if(Data == '\n') Serial.print("\n\t"); //Add tab indent to data
				
			//        Data = DataRead.read();
			}
		// }
	}
	DataRead.close();

	Serial.print("\n");

	// SD.remove(FileNameC); //Remove dummy file created 
	Serial.print("\tResult: ");
	Serial.println(SDError);
	return SDError;
}

boolean ReadSD() 
{
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	ioAlpha.pinMode(PinsIOAlpha::SD_CD, INPUT);
	ioAlpha.pinMode(PinsIOAlpha::SD_EN, OUTPUT);

	ioAlpha.digitalWrite(PinsIOAlpha::SD_EN, HIGH); //Turn on 3v3 to SD

	boolean SDError = false;
	pinMode(Pins::SD_CS, OUTPUT);
	SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
	// SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0)); //Test max speed mode
	// SPI.setDataMode(SPI_MODE0);
	// SPI.setClockDivider(SPI_CLOCK_DIV2); //Sts SPI clock to 4 MHz for an 8 MHz system clock
	if(ioAlpha.digitalRead(PinsIOAlpha::SD_CD)) {
		Serial.println("\tERROR: SD Not Inserted");
		return SDError;
	}

	if(!SD_Init) {
		if (!SD.begin(Pins::SD_CS)) {
			Serial.println("\tERROR: Init Failed");
			SDError = true; 
			return SDError;
		}
		Serial.println("\tInitialized...");
		SD_Init = true;
	}
	String FileName = "HWTest";

	File DataWrite = SD.open(FileNameC, FILE_WRITE);
	if(DataWrite) {
		while (DataWrite.available()) { //Print out all contents of file
      		Serial.write(DataWrite.read());
    	}
	}
	DataWrite.close();
	return SDError;
}

void TestFRAM(boolean ReadPrev, boolean ReadAll) 
{
	const uint8_t ADR = 0x50; //Address for FRAM
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	// Wire.setClock(400000); //Confirm operation in fast mode
	randomSeed(millis()); //Seed with a random number to try to endsure randomness
	uint8_t RandVal = random(231); //Generate a random number between 0 and 231 (pages in Soul of a New Machine)
	static int RandPos[2] = {0};

	//Wake up from sleep
	Wire.beginTransmission(ADR);
	Wire.endTransmission();
	delayMicroseconds(500); //Delay at least t_Rec (400us)

	if(ReadPrev && !ReadAll) {
		Wire.beginTransmission(ADR);
		Wire.write(RandPos[0]); //Command write to previous random position 
		Wire.write(RandPos[1]); //Command write to previous random position 
		uint8_t Error = Wire.endTransmission();

		Wire.requestFrom(ADR, 1);
		uint8_t ReadVal = Wire.read(); //Read single byte back

		Serial.print("\tFRAM Previous Data:\n");
		Serial.print("\t\tData: ");
		Serial.println(ReadVal);
		Serial.print("\t\tPos: 0x");
		Serial.print(RandPos[0], HEX);
		Serial.println(RandPos[1], HEX);
		Serial.print("\t\tError: ");
		Serial.println(Error);
	}
	else if(!ReadPrev && !ReadAll) {
		RandPos[0] = random(255); //Generate a random offset
		RandPos[1] = random(255); //Generate a random offset

		Wire.beginTransmission(ADR);
		Wire.write(RandPos[0]); //Command write to random position 
		Wire.write(RandPos[1]); //Command write to random position
		Wire.write(RandVal);
		uint8_t Error = Wire.endTransmission();

		Serial.print("\tFRAM Write:\n");
		Serial.print("\t\tData: ");
		Serial.println(RandVal);
		Serial.print("\t\tPos: 0x");
		Serial.print(RandPos[0], HEX);
		Serial.println(RandPos[1], HEX);
		Serial.print("\t\tError: ");
		Serial.println(Error);

		Wire.beginTransmission(ADR);
		Wire.write(RandPos[0]); //Command write to random position 
		Wire.write(RandPos[1]); //Command write to random position 
		Error = Wire.endTransmission();

		Wire.requestFrom(ADR, 1);
		RandVal = Wire.read(); //Read single byte back

		Serial.print("\tFRAM Read:\n");
		Serial.print("\t\tData: ");
		Serial.println(RandVal);
		Serial.print("\t\tPos: 0x");
		Serial.print(RandPos[0], HEX);
		Serial.println(RandPos[1], HEX);
		Serial.print("\t\tError: ");
		Serial.println(Error);
	}

	if(ReadAll) {
		unsigned int Pos[2] = {0};
		uint8_t Val = 0;
		Serial.println("\tPOS\tVAL");
		for(int i = 0; i < 65536; i++) {
			Pos[0] = i & 0xFF;
			Pos[1] = (i >> 8);
			Wire.beginTransmission(ADR);
			Wire.write(Pos[0]); //Command write to random position 
			Wire.write(Pos[1]); //Command write to random position 
			Wire.endTransmission();

			Wire.requestFrom(ADR, 1);
			Val = Wire.read(); //Read single byte back
			Serial.print("\t0x");
			Serial.print(i, HEX);
			Serial.print("\t0x");
			Serial.println(Val);
		}
	}

	// //Set device to sleep mode - Hacky but tested and it works. See Page 12 of datasheet for details 
	// Wire.beginTransmission(0x7C);
	// Wire.write((ADR << 1) | 0x01); //Shift to add "r/w" bit
	// Wire.endTransmission(false);
	// Wire.beginTransmission(0x43);
	// Wire.endTransmission();

	I2C_GlobalEn(true);
}

void ReadRTC()
{
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	Clock.Begin(true); //Initalize RTC with external oscilator 
	Serial.print("\tCurrent Time: ");
	Serial.println(Clock.GetTime(MCP7940::Format::ISO_8601)); //FIX! //Grab time from Clock)
}

void TestRTC(boolean ReadStorage, boolean ReadAll) 
{
	bool Clock_Error = 1; //Default error
	// pinMode(Pins::SDA_CTRL, OUTPUT); //Setup SDA control as an output
	// digitalWrite(Pins::SDA_CTRL, HIGH); //Set turn on on board SDA
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	// Wire.setClock(400000); //Confirm operation in fast mode
	pinMode(Pins::Clock_INT, INPUT); //Setup Clock pin as input
	if(!ReadStorage && !ReadAll) { //Only do time/alarm testing if not reading old stuff
		Serial.print("\tCurrent Time: ");
		Serial.println(Clock.GetTime(MCP7940::Format::ISO_8601)); //FIX! //Grab time from Clock)
		Clock.Begin(true); //Initalize RTC with external oscilator 
		Clock.ClearAlarm(0);
		Clock.ClearAlarm(1);
		Clock.SetMode(MCP7940::Mode::Normal); //Use dual alarm setup 
		Serial.print("\tUpdate Time... ");
		Clock.SetTime(2021, 06, 10, 2, 12, 0, 0); //Set arbitrary time (incept date) //DEBUG!
		Serial.print("\tNew Time: ");
		Serial.println(Clock.GetTime(MCP7940::Format::ISO_8601)); //FIX! //Grab time from Clock)
		Clock.SetAlarm(5); //Set an alarm for 5 seconds from now

		unsigned long LocalTime = millis();
		Serial.print("\tAlarm State = "); 
		Serial.println(Clock.ReadAlarm());
		Serial.print("\tWaiting for alarm...\n");
		while(digitalRead(Pins::Clock_INT) != LOW && (millis() - LocalTime < 7000)); //Wait for Clock to trigger, or 7 seconds to go by 
		Serial.print("\tAlarm State = "); 
		Serial.println(Clock.ReadAlarm());
		if(millis() - LocalTime < 7000 && (millis() - LocalTime > 3000)) { //Ensure time passed is greater than 3 seconds, less than 7 seconds 
			Serial.print("\tEnd Time: ");
			Serial.println(Clock.GetTime(MCP7940::Format::ISO_8601)); //FIX! //Grab time from Clock)
			if(Clock.ReadAlarm(0) == true) Clock_Error = 0; //Clear error
			else Clock_Error = 1; //Set error flag if interrupt bit not setg
		}
		else {
			Serial.println("\tERROR: No Alarm!");
			Clock_Error = 1; //Set alarm flag
		}
	}

	//////////// WRITE/READ SRAM //////////////
	randomSeed(millis()); //Seed with a random number to try to endsure randomness
	uint8_t RandVal = random(231); //Generate a random number between 0 and 231 (pages in Soul of a New Machine)
	static int RandPosEEPROM = random(127); //Generate a random offset
	static int RandPosSRAM = random(63); //Generate a random offset

	if(ReadStorage && !ReadAll) {
		Wire.beginTransmission(0x6F);
		Wire.write(RandPosSRAM + 0x20); //Command write to random position 
		uint8_t Error = Wire.endTransmission();

		Wire.requestFrom(0x6F, 1);
		RandVal = Wire.read(); //Read single byte back

		Serial.print("\tSRAM Previous Read:\n");
		Serial.print("\t\tData: 0x");
		Serial.println(RandVal, HEX);
		Serial.print("\t\tPos: 0x");
		Serial.println(RandPosSRAM + 0x20, HEX);
		Serial.print("\t\tError: ");
		Serial.println(Error);

		Wire.beginTransmission(0x57); //EEPROM address
		Wire.write(RandPosEEPROM); //Command write to random position 
		Error = Wire.endTransmission();

		Wire.requestFrom(0x57, 1);
		RandVal = Wire.read(); //Read single byte back

		Serial.print("\tEEPROM Previous Read:\n");
		Serial.print("\t\tData: 0x");
		Serial.println(RandVal, HEX);
		Serial.print("\t\tPos: 0x");
		Serial.println(RandPosEEPROM, HEX);
		Serial.print("\t\tError: ");
		Serial.println(Error);
	}
	else if(!ReadStorage && !ReadAll) {
		RandPosSRAM = random(63);
		Wire.beginTransmission(0x6F);
		Wire.write(RandPosSRAM + 0x20); //Command write to random position 
		Wire.write(RandVal);
		uint8_t Error = Wire.endTransmission();

		Serial.print("\tSRAM Write:\n");
		Serial.print("\t\tData: 0x");
		Serial.println(RandVal, HEX);
		Serial.print("\t\tPos: 0x");
		Serial.println(RandPosSRAM + 0x20, HEX);
		Serial.print("\t\tError: ");
		Serial.println(Error);

		Wire.beginTransmission(0x6F);
		Wire.write(RandPosSRAM + 0x20); //Command write to random position 
		Error = Wire.endTransmission();

		Wire.requestFrom(0x6F, 1);
		RandVal = Wire.read(); //Read single byte back

		Serial.print("\tSRAM Read:\n");
		Serial.print("\t\tData: 0x");
		Serial.println(RandVal, HEX);
		Serial.print("\t\tPos: 0x");
		Serial.println(RandPosSRAM + 0x20, HEX);
		Serial.print("\t\tError: ");
		Serial.println(Error);

		//////////// WRITE/READ EEPROM /////////////
		randomSeed(millis()); //Seed with a random number to try to endsure randomness
		RandVal = random(231); //Generate a random number between 0 and 231 (pages in Soul of a New Machine)
		RandPosEEPROM = random(127); //Generate a random offset

		Wire.beginTransmission(0x57); //EEPROM address
		Wire.write(RandPosEEPROM); //Command write to random position 
		Wire.write(RandVal);
		Error = Wire.endTransmission();

		Serial.print("\tEEPROM Write:\n");
		Serial.print("\t\tData: 0x");
		Serial.println(RandVal, HEX);
		Serial.print("\t\tPos: 0x");
		Serial.println(RandPosEEPROM, HEX);
		Serial.print("\t\tError: ");
		Serial.println(Error);

		unsigned long LocalTime = millis();
		//Poll for write cycle to complete (see page 41 of datasheet)
		do { //Max EEPROM write time is 5ms, wait for cycle to finish or to timeout
			Wire.beginTransmission(0x57); //EEPROM address
			Wire.write(RandPosEEPROM); //Command write to random position 
			Error = Wire.endTransmission();
		} while(Error != 0 && (millis() - LocalTime) < 6);

		if(Error != 0) Serial.print("\tERROR: Write Cycle Timeout"); //Throw error if timeout occoured 

		Wire.beginTransmission(0x57); //EEPROM address
		Wire.write(RandPosEEPROM); //Command write to random position 
		Error = Wire.endTransmission();

		Wire.requestFrom(0x57, 1);
		RandVal = Wire.read(); //Read single byte back

		Serial.print("\tEEPROM Read:\n");
		Serial.print("\t\tData: 0x");
		Serial.println(RandVal, HEX);
		Serial.print("\t\tPos: 0x");
		Serial.println(RandPosEEPROM, HEX);
		Serial.print("\t\tError: ");
		Serial.println(Error);
	}
	if(ReadAll) {
		unsigned int Pos[2] = {0};
		uint8_t Val = 0;
		Serial.println("\tSRAM Data");
		Serial.println("\tPOS\tVAL");
		for(int i = 0; i < 63; i++) {
			Wire.beginTransmission(0x6F);
			Wire.write(i + 0x20); //Command write to random position 
			uint8_t Error = Wire.endTransmission();

			Wire.requestFrom(0x6F, 1);
			Val = Wire.read(); //Read single byte back

			Serial.print("\t0x");
			Serial.print(i + 0x20, HEX);
			Serial.print("\t0x");
			Serial.println(Val, HEX);
		}
		Serial.println("\tEEPROM Data");
		Serial.println("\tPOS\tVAL");
		for(int i = 0; i < 127; i++) {
			Wire.beginTransmission(0x57); //EEPROM address
			Wire.write(i); //Command write to random position 
			uint8_t Error = Wire.endTransmission();

			Wire.requestFrom(0x57, 1);
			Val = Wire.read(); //Read single byte back

			Serial.print("\t0x");
			Serial.print(i, HEX);
			Serial.print("\t0x");
			Serial.println(Val, HEX);
		}
	}
	////////////////// READ UUID /////////////////
	uint64_t UUID = 0; //Used to store UUID from EEPROM
	Wire.beginTransmission(0x57); //EEPROM address
	Wire.write(0xF0); //Begining of EUI-64 data
	Wire.endTransmission();

	Serial.print("\tUUID:\n\t\t");
	uint8_t Val = 0; 
	Wire.requestFrom(0x57, 8); //EEPROM address
	for(int i = 0; i < 8; i++) {
		Val = Wire.read();
		UUID = UUID | (Val << (8 - i)); //Concatonate into full UUID
		Serial.print(Val, HEX); //Print each hex byte from left to right
		if(i < 7) Serial.print('-'); //Print formatting chracter, don't print on last pass
	}
	Serial.print("\n");



	I2C_GlobalEn(true);

	// return Clock_Error; //Return the resulting state
}

void TestLED() 
{
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	static bool LedState = true; //Toggle each time called 
	if(led.begin() != 0) { //Initalize LED controller 
		Serial.println("\tERROR: PCA9634"); //Signal connection error
	}
	else { 
		led.setOutputMode(OpenDrain); //Set device to use open drain outputs
		led.setGroupMode(Blink); //Set system to blinking mode
		led.setOutputArray(Off); //Turn all off by default
		// led.setGroupBrightness(50); //Set to 50% brightness
		led.setBrightness(0, 50);

		led.setGroupBlinkPeriod(1000); //Set to blink every 8 seconds
		led.setGroupOnTime(500); //Set on period for each blink to 200ms

		Serial.print("\tNew state: ");
		if(LedState){
			led.setOutputArray(Group);
			Serial.println("ON");	
		} 
		else {
			led.setOutputArray(Off);
			Serial.println("OFF");
		}
		LedState = !LedState; //Toggle state each time called 
	}
	// led.sleep(true); //DEBUG!
	I2C_GlobalEn(true);

}

void TestBattery()
{
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	//Turn on external I2C port
	ioAlpha.pinMode(PinsIOAlpha::I2C_EXT_EN, OUTPUT);
	ioAlpha.digitalWrite(PinsIOAlpha::I2C_EXT_EN, HIGH);

	I2C_GlobalEn(true);
	I2C_OnBoardEn(false);
	// Wire.setClock(400000); //Confirm operation in fast mode

	// Wire.beginTransmission(0x36);
	// Wire.write(0x09); //Write to voltage register
	// int Error = Wire.endTransmission();
	int Error = 0; //DEBUG! 
	if(Error != 0) {
		Serial.print("\tERROR: I2C - ");
		Serial.println(Error);
	}
	else {
		// int Data = 0; 
		// Wire.requestFrom(0x36, 2);
		// Data = Wire.read();
		// Data = Data | (Wire.read() << 8);
		// Serial.print("\tCell Voltage: ");
		// Serial.println(float(Data)*0.078125); //Print voltage in mV
		Serial.print("\tCell Voltage: ");
		Serial.println(GetBatteryData(0x09));
		Serial.print("\tCell Voltage, Avg: ");
		Serial.println(GetBatteryData(0x19));
		Serial.print("\tCap Left: ");
		Serial.println(GetBatteryData(0x05));
		Serial.print("\tCap Total: ");
		Serial.println(GetBatteryData(0x10));
		Serial.print("\tTTF: ");
		Serial.println(GetBatteryData(0x20));
		Serial.print("\tSoC: ");
		Serial.println(GetBatteryData(0x06));


		Wire.beginTransmission(0x36);
		Wire.write(0x40); //Write to led register //DEBUG!
		// Wire.write(0b11100100); //Set for always on, 4 bars
		Wire.write(0b10100100); //Set for push button control, 4 bars
		Wire.write(0b01110010); //Default, but breathing LEDs
		Wire.endTransmission();
	}
}

uint16_t GetBatteryData(uint8_t Reg)
{
	const uint8_t ADR = 0x36;

	Wire.beginTransmission(ADR);
	Wire.write(Reg); //Write to voltage register
	int Error = Wire.endTransmission();
	delay(2);
	int Data = 0; 
	// Wire.requestFrom(ADR, 2);
	Wire.requestFrom(ADR, 2);
	Data = Wire.read();
	Data = Data | (Wire.read() << 8);

	return Data;
}

void TestUSB()
{
	// pinMode(BGPWR, INPUT);
	// Serial.print("\tUSB: ");
	// Serial.println(digitalRead(BGPWR));
	I2C_GlobalEn(false);
  	I2C_OnBoardEn(true);
	ioAlpha.pinMode(PinsIO::VUSB, INPUT); //Set to input to sense 
	uint8_t State = ioAlpha.digitalRead(PinsIO::VUSB); //Read state of port
	Serial.print("\tUSB Status: ");
	Serial.println(State);
}

void TestI2C() 
{
//   I2C_GlobalEn(true);
//   I2C_OnBoardEn(true);
	// Wire.begin();
	// Wire.setClock(10000); //Make sure all work in standard mode
	int Error = 0; 
	for(int i = 0; i < 128; i++) {
		
		Wire.beginTransmission(i);
		Wire.write(0x00);
		Error = Wire.endTransmission();

		if(Error != 3) {
			Serial.print("\t0x");
			Serial.print(i, HEX);
			Serial.print(" - ");
			if(Error == 0) Serial.println("PASS");
			else {
				Serial.print("ERROR - ");
				Serial.println(Error);
			}
		}
		delay(1); //DEBUG! 
	}
//   I2C_OnBoardEn(false);
}

void SensorDemo1() 
{
	// Serial.println(">Spector1");
	/////////////// SETUP /////////////////////
	bool isFault[4] = {false, false, false, false}; //Keep track which ports faulted
	const uint8_t POS_DETECT = 12; 
	const uint8_t SENSE_EN = 13;
	I2C_OnBoardEn(true);
	I2C_GlobalEn(false);
	// ioBeta.pinMode(PinsIOBeta::EN2, OUTPUT);
	// ioBeta.pinMode(PinsIOBeta::I2C_EN2, OUTPUT);
	// ioBeta.pinMode(PinsIOBeta::SEL2, OUTPUT);
	// ioBeta.pinMode(PinsIOBeta::FAULT2, INPUT);

	// ioBeta.digitalWrite(PinsIOBeta::EN2, HIGH);
	// ioBeta.digitalWrite(PinsIOBeta::I2C_EN2, HIGH);
	// ioBeta.digitalWrite(PinsIOBeta::SEL2, HIGH);

	ioBeta.pinMode(PinsIOBeta::EN2, OUTPUT);
	ioBeta.pinMode(PinsIOBeta::I2C_EN2, OUTPUT);
	ioBeta.pinMode(PinsIOBeta::SEL2, OUTPUT);
	ioBeta.pinMode(PinsIOBeta::FAULT2, INPUT_PULLUP);

	ioBeta.digitalWrite(PinsIOBeta::EN2, HIGH);
	ioBeta.digitalWrite(PinsIOBeta::I2C_EN2, HIGH);
	ioBeta.digitalWrite(PinsIOBeta::SEL2, LOW);
	int ErrorBeta = CSA_Beta.begin();
	CSA_Beta.SetCurrentDirection(CH1, UNIDIRECTIONAL); //Bulk voltage, unidirectional

	

	// ioBeta.pinMode(PinsIOBeta::EN1, OUTPUT);
	// ioBeta.pinMode(PinsIOBeta::I2C_EN1, OUTPUT);
	// ioBeta.pinMode(PinsIOBeta::SEL1, OUTPUT);
	// ioBeta.pinMode(PinsIOBeta::FAULT1, INPUT);

	// ioBeta.digitalWrite(PinsIOBeta::EN1, HIGH);
	// ioBeta.digitalWrite(PinsIOBeta::I2C_EN1, HIGH);
	// ioBeta.digitalWrite(PinsIOBeta::SEL1, HIGH);

	I2C_OnBoardEn(false);
	I2C_GlobalEn(true);
	// pinMode(Pins::TALON2_GPIOB, OUTPUT);
	// digitalWrite(Pins::TALON2_GPIOB, HIGH); //Connect to internal I2C
	pinMode(Pins::TALON2_GPIOB, OUTPUT);
	digitalWrite(Pins::TALON2_GPIOB, HIGH); //Connect to internal I2C
	Serial.println(ioTalonI2C.begin());

	ioTalonI2C.pinMode(POS_DETECT, INPUT); //Set position detect as normal pullup (has external pullup)
	ioTalonI2C.pinMode(SENSE_EN, OUTPUT); //Set sense control as output
	

	// for(int i = 8; i < 12; i++) { //Set FAULT lines as input pullups
	// 	ioTalonI2C.pinMode(i, INPUT_PULLUP);
	// }
	// BYPASS - START ////////////////////////////////////////////////////////////////////////////////////////
	// ioTalonI2C.digitalWrite(SENSE_EN, LOW); //Default sense to off
	// for(int i = 0; i < 4; i++) { //Default power off
	// 	ioTalonI2C.pinMode(i, OUTPUT);
	// 	ioTalonI2C.digitalWrite(i, LOW);
	// }
	
	// for(int i = 8; i < 12; i++) { //Set FAULT lines as output //DEBUG!
	// 	ioTalonI2C.pinMode(i, OUTPUT);
	// 	ioTalonI2C.digitalWrite(i, LOW);
	// }

	// for(int i = 0; i < 4; i++) { //Enable all power, release if fault
	// 	ioTalonI2C.pinMode(i, OUTPUT);
	// 	ioTalonI2C.digitalWrite(i, HIGH);
	// 	delay(1);
	// 	I2C_GlobalEn(false);
	// 	I2C_OnBoardEn(true);
	// 	float IBulk = CSA_Beta.GetCurrent(CH1, false);
	// 	I2C_OnBoardEn(false);
	// 	I2C_GlobalEn(true);
	// 	if(ioTalonI2C.digitalRead(i + 8) || IBulk > 500) { //If fault, turn back off
	// 		ioTalonI2C.digitalWrite(i, LOW);
	// 		isFault[i] = true;
	// 		Serial.print("\tPort ");
	// 		Serial.print(i);
	// 		Serial.print(" FAULT - ");
	// 		Serial.print(IBulk);
	// 		Serial.println("mA");
	// 	}
	// 	Serial.print("\tPort ");
	// 	Serial.print(i);
	// 	Serial.print(" - ");
	// 	Serial.print(IBulk);
	// 	Serial.println("mA");
	// } 
	// BYPASS - END ////////////////////////////////////////////////////////////////////////////////////////

	ioTalonI2C.digitalWrite(SENSE_EN, HIGH); //Turn sense back on
	for(int i = 0; i < 4; i++) { //Toggle enable lines to reset latches if tripped
		if(!isFault[i]) { //Only toggle back on if fault did not occour
			ioTalonI2C.pinMode(i, OUTPUT);
			ioTalonI2C.digitalWrite(i, HIGH);
			delayMicroseconds(10);
			ioTalonI2C.digitalWrite(i, LOW);
			delayMicroseconds(10);
			ioTalonI2C.digitalWrite(i, HIGH);
		}
	}
	
	for(int i = 8; i < 12; i++) { //Set FAULT lines as output //DEBUG!
		ioTalonI2C.pinMode(i, INPUT);
	}

	for(int i = 4; i < 8; i++) { //Enable all data
		ioTalonI2C.pinMode(i, OUTPUT);
		ioTalonI2C.digitalWrite(i, HIGH); 
		// ioTalonI2C.digitalWrite(i, LOW); //DEBUG! Disable data by default
	} 

	

	Serial.print(">Port Sense:\n");
	ioSense.begin(); //Initalize voltage sensor IO expander
	for(int i = 0; i < 4; i++) { //Set all pins to output
		ioSense.pinMode(i, OUTPUT); 
	}
	ioSense.digitalWrite(MUX_EN, LOW); //Turn MUX on 
	int SenseError = adcSense.Begin(); //Initialize ADC 
	if(SenseError == 0) { //Only proceed if ADC connects correctly
		adcSense.SetResolution(18); //Set to max resolution (we paid for it right?) 

		ioSense.digitalWrite(MUX_SEL2, LOW); //Read voltages
		for(int i = 0; i < 4; i++){ //Increment through 4 voltages
			ioSense.digitalWrite(MUX_SEL0, i & 0b01); //Set with lower bit
			ioSense.digitalWrite(MUX_SEL1, (i & 0b10) >> 1); //Set with high bit
			delay(1); //Wait for voltage to stabilize
	  		Serial.print("\tPort");
	  		Serial.print(i);
	  		Serial.print(":");
	  		Serial.print(adcSense.GetVoltage(true)*VoltageDiv, 6); //Print high resolution voltage
	  		Serial.print(" V\n");  
		}
		ioSense.digitalWrite(MUX_SEL2, HIGH); //Read currents
		for(int i = 0; i < 4; i++){ //Increment through 4 voltages
			ioSense.digitalWrite(MUX_SEL0, i & 0b01); //Set with lower bit
			ioSense.digitalWrite(MUX_SEL1, (i & 0b10) >> 1); //Set with high bit
			delay(1); //Wait for voltage to stabilize
	  		Serial.print("\tPort");
	  		Serial.print(i);
	  		Serial.print(":");
	  		Serial.print(adcSense.GetVoltage(true)*CurrentDiv*1000, 6); //Print high resolution current measure in mA
	  		Serial.print(" mA\n");  
		}
	}
	else Serial.print("\tFAIL!\n");
	// digitalWrite(I2C_EN, HIGH); //Turn on external I2C



	//FORCE BYPASS
	// ioTalonI2C.digitalWrite(SENSE_EN, LOW); //Turn off sense
	// ioTalonI2C.pinMode(8, OUTPUT); //Set FAULT1 to output
	// ioTalonI2C.digitalWrite(8, LOW); //Force fault
	// ioTalonI2C.digitalWrite(0, LOW);
	// ioTalonI2C.digitalWrite(0, HIGH); //Toggle port 1 on
	// ioTalonI2C.digitalWrite(4, HIGH); //Make sure signal is turned on

	// digitalWrite(Pins::TALON1_GPIOB, LOW); //Connect to external I2C
	// digitalWrite(Pins::TALON2_GPIOB, LOW); //Connect to external I2C

	// SWITCHED READING TEST - BEGIN /////////////////////////////////////////////////////////////////////////////////////////////////
	// ioTalonI2C.digitalWrite(SENSE_EN, LOW); //Default sense to off
	// for(int i = 0; i < 4; i++) {
	// 	ioTalonI2C.pinMode(i + 8, OUTPUT); //Setup to force fault
	// 	ioTalonI2C.digitalWrite(i + 8, LOW); //Force fault off
	// 	ioTalonI2C.digitalWrite(i + 4, HIGH); //Turn on all enable
	// 	ioTalonI2C.digitalWrite(i, LOW); //Turn off all power
	// }
	// digitalWrite(Pins::TALON2_GPIOB, LOW); //Connect to external I2C
	// for(int i = 0; i < 4; i++) {
	// 	digitalWrite(Pins::TALON2_GPIOB, HIGH); //Connect to internal I2C
	// 	// ioTalonI2C.digitalWrite(i + 4, HIGH); //Turn on specific data channel
	// 	ioTalonI2C.digitalWrite(i, HIGH); //Turn on specific power channel
	// 	digitalWrite(Pins::TALON2_GPIOB, LOW); //Connect to external I2C
	// 	uint8_t error_dps368 = 0; //DEBUG! FIX! Change library to return stuff properly
	// 	pressureSense.begin(Wire); //Initialize DPS368 sensor //DEBUG! FIX! Change library to return stuff properly
	// 	uint8_t error_sht31 = rhSense.begin(0x44); //Initialize SHT31 sensor
		
	// 	Serial.print(">Port ");
	// 	Serial.print(i);
	// 	Serial.println(" Status:");
	// 	// Serial.println(">Haar Status:"); //Print resulting sensor status 
	// 	Serial.print("\tDPS368: "); 
	// 	if(error_dps368 == 0) Serial.print("PASS\n"); //Print error grabbed from sensor
	// 	else Serial.print("FAIL!\n"); //Fail if result is not 0
	// 	Serial.print("\tSHT31: ");
	// 	if(error_sht31 == 1) Serial.print("PASS\n"); //Print error grabbed from sensor
	// 	else Serial.print("FAIL!\n"); //Fail if result is 0

	// 	///////////////// RUN ////////////////////
	// 	float temp_dps368; //Storage for temp measurement from DPS368 
	// 	float pressure; //Storage for pressure measurment from DPS368
		

	// 	int16_t error1 = pressureSense.measureTempOnce(temp_dps368, dps368_oversampling); //Grab new temp values [°C]
	// 	int16_t error2 = pressureSense.measurePressureOnce(pressure, dps368_oversampling); //Grab new pressure values [Pa]
	// 	pressure = pressure/100.0f; //Convert from Pa to mBar, because Pa is a stupid unit, and hPa is more stupid. 

	// 	float humidity = rhSense.readHumidity(); //Grab new humidity values [%]
	// 	float temp_sht31 = rhSense.readTemperature(); //Grab new temp values [°C]
	// 	// digitalWrite(I2C_EN, HIGH); //Turn off external I2C
	// 	// I2C_GlobalEn(false);

	// 	Serial.print("\tTemp (SHT31): "); Serial.print(temp_sht31, 4); Serial.print(" °C\t"); //Print temp to 4 decimal places
	// 	Serial.print("Temp (DPS368): "); Serial.print(temp_dps368, 2); Serial.print(" °C\t"); //Print temp to 2 decimal places
	// 	Serial.print("Pressure: "); Serial.print(pressure, 2); Serial.print(" mBar\t"); //Print pressure to 2 decimal places
	// 	Serial.print("Humidity: "); Serial.print(humidity, 2); Serial.print(" %\n"); //Print humidity to 2 decimal places
	// 	digitalWrite(Pins::TALON2_GPIOB, HIGH); //Connect to internal I2C
	// 	// ioTalonI2C.digitalWrite(i + 4, LOW); //Turn off specific data channel
	// 	ioTalonI2C.digitalWrite(i, LOW); //Turn off specific power channel
	// 	digitalWrite(Pins::TALON2_GPIOB, LOW); //Connect to external I2C
	// }
	// SWITCHED READING TEST - END /////////////////////////////////////////////////////////////////////////////////////////////////

	// BUS READING TEST - BEGIN /////////////////////////////////////////////////////////////////////////////////////////////////
	// ioTalonI2C.digitalWrite(4, LOW); //Disable port 0 data //DEBUG!
	// ioTalonI2C.digitalWrite(0, LOW); //Disable port 0 power //DEBUG!
	digitalWrite(Pins::TALON2_GPIOB, LOW); //Connect to external I2C
	uint8_t error_dps368A = 0; //DEBUG! FIX! Change library to return stuff properly
	pressureSenseA.begin(Wire, 0x77); //Initialize DPS368 sensor //DEBUG! FIX! Change library to return stuff properly
	uint8_t error_sht31A = rhSenseA.begin(0x44); //Initialize SHT31 sensor

	uint8_t error_dps368B = 0; //DEBUG! FIX! Change library to return stuff properly
	pressureSenseB.begin(Wire, 0x78); //Initialize DPS368 sensor //DEBUG! FIX! Change library to return stuff properly
	uint8_t error_sht31B = rhSenseB.begin(0x45); //Initialize SHT31 sensor

	uint8_t error_sgp30 = sgp.begin();
	uint8_t error_scd30 = airSensor.begin();
	Serial.println(">Haar Test - ALPHA:"); //Print resulting sensor status 
	Serial.print("\tDPS368: "); 
	if(error_dps368A == 0) Serial.print("PASS\n"); //Print error grabbed from sensor
	else Serial.print("FAIL!\n"); //Fail if result is not 0
	Serial.print("\tSHT31: ");
	if(error_sht31A == 1) Serial.print("PASS\n"); //Print error grabbed from sensor
	else Serial.print("FAIL!\n"); //Fail if result is 0

	Serial.println(">Haar Test - BETA:"); //Print resulting sensor status 
	Serial.print("\tDPS368: "); 
	if(error_dps368B == 0) Serial.print("PASS\n"); //Print error grabbed from sensor
	else Serial.print("FAIL!\n"); //Fail if result is not 0
	Serial.print("\tSHT31: ");
	if(error_sht31B == 1) Serial.print("PASS\n"); //Print error grabbed from sensor
	else Serial.print("FAIL!\n"); //Fail if result is 0

	Serial.println(">Hedorah Test:"); //Print resulting sensor status 
	Serial.print("\tSGP30: ");
	if(!error_sgp30) {
		Serial.println("FAIL!");
	}
	else {
		Serial.println("PASS");
	}

	Serial.println(">Hedorah-NDIR Test:"); //Print resulting sensor status 
	Serial.print("\tSCD30: ");
	if(!error_scd30) {
		Serial.println("FAIL!");
	}
	else {
		Serial.println("PASS");
	}

	

	///////////////// RUN ////////////////////
	float temp_dps368A; //Storage for temp measurement from DPS368 
	float pressureA; //Storage for pressure measurment from DPS368
	

	int16_t error1 = pressureSenseA.measureTempOnce(temp_dps368A, dps368_oversampling); //Grab new temp values [°C]
	int16_t error2 = pressureSenseA.measurePressureOnce(pressureA, dps368_oversampling); //Grab new pressure values [Pa]
	pressureA = pressureA/100.0f; //Convert from Pa to mBar, because Pa is a stupid unit, and hPa is more stupid. 

	float humidityA = rhSenseA.readHumidity(); //Grab new humidity values [%]
	float temp_sht31A = rhSenseA.readTemperature(); //Grab new temp values [°C]

	float temp_dps368B; //Storage for temp measurement from DPS368 
	float pressureB; //Storage for pressure measurment from DPS368
	

	error1 = pressureSenseB.measureTempOnce(temp_dps368B, dps368_oversampling); //Grab new temp values [°C]
	error2 = pressureSenseB.measurePressureOnce(pressureB, dps368_oversampling); //Grab new pressure values [Pa]
	pressureA = pressureB/100.0f; //Convert from Pa to mBar, because Pa is a stupid unit, and hPa is more stupid. 

	float humidityB = rhSenseB.readHumidity(); //Grab new humidity values [%]
	float temp_sht31B = rhSenseB.readTemperature(); //Grab new temp values [°C]
	// digitalWrite(I2C_EN, HIGH); //Turn off external I2C
	// I2C_GlobalEn(false);

	if (! sgp.IAQmeasure()) {
		// Serial.println("\tMeasurement failed");
		// return;
	}
	if (! sgp.IAQmeasureRaw()) {
		// Serial.println("\tRaw Measurement failed");
		// return;
	}

	float CO2_SCD30 = 0;
	float Temp_SCD30 = 0;
	float RH_SCD30 = 0;
	unsigned long LocalTime = millis();
	while((millis() - LocalTime) < 3000) {
		if (airSensor.dataAvailable()) {
			CO2_SCD30 = airSensor.getCO2();
			Temp_SCD30 = airSensor.getTemperature();
			RH_SCD30 = airSensor.getHumidity();
			continue;
		}
	}

	Serial.print("\tALPHA: Temp (SHT31): "); Serial.print(temp_sht31A, 4); Serial.print(" °C\t"); //Print temp to 4 decimal places
	Serial.print("Temp (DPS368): "); Serial.print(temp_dps368A, 2); Serial.print(" °C\t"); //Print temp to 2 decimal places
	Serial.print("Pressure: "); Serial.print(pressureA, 2); Serial.print(" mBar\t"); //Print pressure to 2 decimal places
	Serial.print("Humidity: "); Serial.print(humidityA, 2); Serial.print(" %\n"); //Print humidity to 2 decimal places

	Serial.print("\tBETA: Temp (SHT31): "); Serial.print(temp_sht31B, 4); Serial.print(" °C\t"); //Print temp to 4 decimal places
	Serial.print("Temp (DPS368): "); Serial.print(temp_dps368B, 2); Serial.print(" °C\t"); //Print temp to 2 decimal places
	Serial.print("Pressure: "); Serial.print(pressureB, 2); Serial.print(" mBar\t"); //Print pressure to 2 decimal places
	Serial.print("Humidity: "); Serial.print(humidityB, 2); Serial.print(" %\n"); //Print humidity to 2 decimal places

	Serial.print("\tTVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
	Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.print(" ppm");
	Serial.print("\tRaw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
	Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");

	Serial.print("\tCO2: "); Serial.print(CO2_SCD30); Serial.print(" ppm\tTemp: "); Serial.print(Temp_SCD30); Serial.print(" °C\tRH: "); Serial.print(RH_SCD30); Serial.println("%");

	// BUS READING TEST - END /////////////////////////////////////////////////////////////////////////////////////////////////

}

void SensorDemo2()
{
	const float VoltageDivSDI12 = 6;
	bool isFault[4] = {0}; //Keep track which ports faulted
	I2C_OnBoardEn(true);
	I2C_GlobalEn(false);
	ioBeta.pinMode(PinsIOBeta::EN2, OUTPUT);
	ioBeta.pinMode(PinsIOBeta::I2C_EN2, OUTPUT);
	ioBeta.pinMode(PinsIOBeta::SEL2, OUTPUT);
	ioBeta.pinMode(PinsIOBeta::FAULT2, INPUT_PULLUP);

	ioBeta.digitalWrite(PinsIOBeta::EN2, HIGH); //Toggle output in case tripped
	ioBeta.digitalWrite(PinsIOBeta::EN2, LOW);
	ioBeta.digitalWrite(PinsIOBeta::EN2, HIGH);
	ioBeta.digitalWrite(PinsIOBeta::I2C_EN2, HIGH);
	ioBeta.digitalWrite(PinsIOBeta::SEL2, HIGH);
	int ErrorBeta = CSA_Beta.begin();
	CSA_Beta.SetCurrentDirection(CH1, UNIDIRECTIONAL); //Bulk voltage, unidirectional

	I2C_OnBoardEn(false);
	I2C_GlobalEn(true);
	Serial.print("\tIO Status: ");
	Serial.println(ioTalonSDI12.begin()); //DEBUG!

	ioTalonSDI12.pinMode(SENSE_EN, OUTPUT); //Set sense control as output
	ioTalonSDI12.digitalWrite(SENSE_EN, HIGH); //Default sense to on - NOTE: For proper sequencing, this must be done BEFORE each enable line is driven high
	
	ioTalonSDI12.pinMode(Dir, OUTPUT); //Set to output
	ioTalonSDI12.pinMode(FOut, OUTPUT); //Set to output
	ioTalonSDI12.digitalWrite( Dir, HIGH); //Set to transmit 
	// ioTalonSDI12.digitalWrite(SENSE_EN, LOW); //Default sense to off before enabling the line //DEBUG!
	
	// ioSense.begin(); //Initalize voltage sensor IO expander
	// for(int i = 0; i < 4; i++) { //Set all pins to output
	// 	ioSense.pinMode(i, OUTPUT); 
	// }
	// ioSense.digitalWrite(MUX_EN, LOW); //Turn MUX on 
	// int SenseError = adcSense.Begin(); //Initialize ADC 
	// adcSense.SetResolution(12); //Set to fast mode
	// ioSense.digitalWrite(MUX_SEL2, HIGH); //Read currents
	// ioSense.digitalWrite(MUX_SEL0, LOW); //Set to port 1
	// ioSense.digitalWrite(MUX_SEL1, LOW); 
	// for(int i = 0; i < 4; i++) { //Enable all power
	// 	ioTalonSDI12.pinMode(i, OUTPUT); 
	// 	ioTalonSDI12.digitalWrite(i, HIGH); 
	// 	ioSense.digitalWrite(MUX_SEL0, i & 0b01); //Set with lower bit
	// 	ioSense.digitalWrite(MUX_SEL1, (i & 0b10) >> 1); //Set with high bit
	// 	I2C_GlobalEn(false);
	// 	I2C_OnBoardEn(true);
	// 	float IBulk = CSA_Beta.GetCurrent(CH1, false);
	// 	I2C_OnBoardEn(false);
	// 	I2C_GlobalEn(true);

	// 	// delay(2);
	// 	// // ioTalonSDI12.digitalWrite(i, LOW);
	// 	// // delay(1);
	// 	// // ioTalonSDI12.digitalWrite(i, HIGH); 
	// 	// // delayMicroseconds(100);
	// 	// delay(10); //Wait for voltage to stabilize
	// 	// float Current = adcSense.GetVoltage(true)*CurrentDiv*1000;
	// 	// if(ioTalonSDI12.digitalRead(i + 8) || Current > 450) { //If fault, turn back off
	// 	if(ioTalonSDI12.digitalRead(i + 8) || IBulk > 2500) { //If fault, turn back off
	// 		ioTalonSDI12.digitalWrite(i, LOW);
	// 		Serial.print("\tPort ");
	// 		Serial.print(i);
	// 		Serial.println(" FAULT");
	// 	}
		
	// 	Serial.print("\tPort");
	// 	Serial.print(i);
	// 	Serial.print(":");
	// 	Serial.print(IBulk, 6); //Print high resolution current measure in mA
	// 	Serial.print(" mA\n");  

	// } 
	// //DEBUG! - BEGIN
	ioTalonSDI12.digitalWrite(SENSE_EN, LOW); //Turn sense off
	// ioTalonSDI12.digitalWrite(SENSE_EN, HIGH); //Turn sense on
	for(int i = 8; i < 12; i++) { 
		ioTalonSDI12.pinMode(i, OUTPUT); //Set FAULT lines as output //DEBUG!
		ioTalonSDI12.digitalWrite(i, LOW);
		// ioTalonI2C.pinMode(i, INPUT_PULLUP); //Set FAULT lines as input pullup
	}

	for(int i = 0; i < 4; i++) { //Enable all power, release if fault
		ioTalonSDI12.pinMode(i, OUTPUT);
		ioTalonSDI12.digitalWrite(i, HIGH); 
		delayMicroseconds(1500);
		I2C_GlobalEn(false);
		I2C_OnBoardEn(true);
		float IBulk = CSA_Beta.GetCurrent(CH1, false);
		I2C_OnBoardEn(false);
		I2C_GlobalEn(true);

		// delay(2);
		// // ioTalonSDI12.digitalWrite(i, LOW);
		// // delay(1);
		// // ioTalonSDI12.digitalWrite(i, HIGH); 
		// // delayMicroseconds(100);
		// delay(10); //Wait for voltage to stabilize
		// float Current = adcSense.GetVoltage(true)*CurrentDiv*1000;
		// if(ioTalonSDI12.digitalRead(i + 8) || Current > 450) { //If fault, turn back off
		if(ioTalonSDI12.digitalRead(i + 8) || IBulk > 2050) { //If fault, turn back off - Min trip current val
			ioTalonSDI12.digitalWrite(i, LOW);
			isFault[i] = true;
			Serial.print("\tPort ");
			Serial.print(i);
			Serial.println(" FAULT");
		}
		
		Serial.print("\tPort");
		Serial.print(i);
		Serial.print(":");
		Serial.print(IBulk, 6); //Print high resolution current measure in mA
		Serial.print(" mA\n");  
		// delay(1);
		// if(ioTalonSDI12.digitalRead(i + 8)) { //If fault, turn back off
		// 	// ioTalonI2C.digitalWrite(i, LOW);
		// 	Serial.print("\tPort ");
		// 	Serial.print(i);
		// 	Serial.println(" FAULT");
		// }
	} 
	
	ioTalonSDI12.digitalWrite(SENSE_EN, HIGH); //Turn sense back on
	for(int i = 0; i < 4; i++) { //Toggle enable lines to reset latches if tripped
		if(!isFault[i]) { //Only toggle back on if fault did not occour
			ioTalonSDI12.digitalWrite(i, HIGH);
			delayMicroseconds(10);
			ioTalonSDI12.digitalWrite(i, LOW);
			delayMicroseconds(10);
			ioTalonSDI12.digitalWrite(i, HIGH);
		}
	}
	
	for(int i = 8; i < 12; i++) { //Set FAULT lines as input //DEBUG!
		ioTalonSDI12.pinMode(i, INPUT);
	}
	// DEBUG! - END

	for(int i = 4; i < 8; i++) { //Default data to disabled
		ioTalonSDI12.pinMode(i, OUTPUT); 
		ioTalonSDI12.digitalWrite(i, LOW); 
	} 

	// for(int i = 8; i < 12; i++) { //Set FAULT lines as input pullups
	// 	ioTalonSDI12.pinMode(i, INPUT_PULLUP);
	// }

	ioTalonSDI12.pinMode(POS_DETECT, INPUT); //Set position detect as normal pullup (has external pullup)

	Serial.print(">Port Sense:\n");

	ioSense.begin(); //Initalize voltage sensor IO expander
	for(int i = 0; i < 4; i++) { //Set all pins to output
		ioSense.pinMode(i, OUTPUT); 
	}
	ioSense.digitalWrite(MUX_EN, LOW); //Enable MUX
	int SenseError = adcSense.Begin(); //Initialize ADC 
	int ApogeeError = apogeeADC.begin();
	if(SenseError == 0) { //Only proceed if ADC connects correctly
		adcSense.SetResolution(18); //Set to max resolution (we paid for it right?) 

		ioSense.digitalWrite(MUX_SEL2, LOW); //Read voltages
		for(int i = 0; i < 4; i++){ //Increment through 4 voltages
			ioSense.digitalWrite(MUX_SEL0, i & 0b01); //Set with lower bit
			ioSense.digitalWrite(MUX_SEL1, (i & 0b10) >> 1); //Set with high bit
			delay(1); //Wait for voltage to stabilize
	  		Serial.print("\tPort");
	  		Serial.print(i);
	  		Serial.print(":");
	  		Serial.print(adcSense.GetVoltage(true)*VoltageDivSDI12, 6); //Print high resolution voltage
	  		Serial.print(" V\n");  
		}
		ioSense.digitalWrite(MUX_SEL2, HIGH); //Read currents
		for(int i = 0; i < 4; i++){ //Increment through 4 voltages
			ioSense.digitalWrite(MUX_SEL0, i & 0b01); //Set with lower bit
			ioSense.digitalWrite(MUX_SEL1, (i & 0b10) >> 1); //Set with high bit
			delay(1); //Wait for voltage to stabilize
	  		Serial.print("\tPort");
	  		Serial.print(i);
	  		Serial.print(":");
	  		Serial.print(adcSense.GetVoltage(true)*CurrentDiv*1000, 6); //Print high resolution current measure in mA
	  		Serial.print(" mA\n");  
		}
	}
	else Serial.print("\tFAIL!\n");

	ioSense.digitalWrite(MUX_EN, HIGH); //Disable MUX
	Serial.println("Apogee Port:");
	if(ApogeeError == 0) {
		apogeeADC.getVoltageRaw(); //Clear POR value of register 
		for(int i = 0; i < 10; i++) {
			Serial.print("\t");
			Serial.print(apogeeADC.getVoltage(4.9477)*1000.0);
			Serial.println(" mV");
		}
	}
	else Serial.println("FAIL!");
	

	for(int i = 0; i < 4; i++) {
		ioTalonSDI12.pinMode(i + 4, OUTPUT); 
		ioTalonSDI12.digitalWrite(i + 4, HIGH); //Turn port data on
		// delay(25); //DEBUG!
		// SendCommand("?"); //DEBUG!
		String Data = SendCommand("?!");
		// Serial.println(Data); //DEBUG!
		int Address = (Data.substring(0, 1)).toInt(); //Get address detected 
		String ID = Command("I", Address); //Get ID string from device
		Serial.print("\tPort ");
		Serial.print(i);
		Serial.println(":");
		Serial.print("\t\tAddress: ");
		Serial.println(Address);
		Serial.print("\t\tID: ");
		Serial.println(ID);
		ioTalonSDI12.digitalWrite(i + 4, LOW); //Turn port data back off
	}

	for(int i = 4; i < 8; i++) { //Default data to Enabled
		ioTalonSDI12.pinMode(i, OUTPUT); 
		ioTalonSDI12.digitalWrite(i, HIGH); 
	} 

	//Enable all but channel 4
	// for(int i = 4; i < 7; i++) { 
	// 	ioTalonSDI12.pinMode(i, OUTPUT); 
	// 	ioTalonSDI12.digitalWrite(i, HIGH); 
	// } 
	// ioTalonSDI12.pinMode(7, OUTPUT); 
	// ioTalonSDI12.digitalWrite(7, LOW); 
	///////////////////////////
}

void SensorDemo3()
{
	const uint8_t MUX_EN = 11;
	const uint8_t MUX_SEL2 = 10;
	const uint8_t MUX_SEL1 = 9;
	const uint8_t MUX_SEL0 = 8;
	I2C_OnBoardEn(true);
	I2C_GlobalEn(false);
	ioBeta.pinMode(PinsIOBeta::EN4, OUTPUT);
	ioBeta.pinMode(PinsIOBeta::I2C_EN4, OUTPUT);
	ioBeta.pinMode(PinsIOBeta::SEL4, OUTPUT);
	ioBeta.pinMode(PinsIOBeta::FAULT4, INPUT);

	ioBeta.digitalWrite(PinsIOBeta::EN4, HIGH);
	ioBeta.digitalWrite(PinsIOBeta::I2C_EN4, HIGH);
	ioBeta.digitalWrite(PinsIOBeta::SEL4, LOW);

	I2C_OnBoardEn(false);
	I2C_GlobalEn(true);
	ioAUXAlpha.begin();
	ioAUXBeta.begin();
	ioAUXGamma.begin();
	ioAUXBeta.setIntConfig(false, false, true, false); //Setup individual inerrupts with push pull output, active high, cleared by reading GPIO regs 
	//SETUP DEFAULT PINS
	for(int i = 0; i < 16; i++) { //Set all Gamma pins to input
		ioAUXGamma.pinMode(i, INPUT_PULLUP);
	} 

	for(int i = 12; i < 16; i++) { //Set all Alpha (not MUX_SEL) pins to input
		ioAUXAlpha.pinMode(i, INPUT_PULLUP);
		// ioAlpha.pinMode(i, OUTPUT);
		// ioAlpha.digitalWrite(i, LOW);
	} 

	for(int i = 11; i < 16; i++) { //Set all Beta pins to input
		ioAUXBeta.pinMode(i, INPUT_PULLUP);
		// ioBeta.pinMode(i, OUTPUT);
		// ioBeta.digitalWrite(i, LOW);
	} 


	for(int i = 0; i < 3; i++) { //Set each group of IO pins seperately
		ioAUXBeta.pinMode(OUT1 + i, INPUT); //Set all outputs from counters as inputs to I/O expander
		ioAUXBeta.pinMode(COUNT_EN1 + i, OUTPUT); //Set enable control as output
		ioAUXBeta.pinMode(OVF1 + i, INPUT); //Set overflow pins as inputs
		ioAUXBeta.digitalWrite(COUNT_EN1 + i, HIGH); //Default to enable all counters
		ioAUXAlpha.pinMode(FAULT1 + 2*i, INPUT_PULLUP);
		ioAUXAlpha.pinMode(EN1 + 2*i, OUTPUT);
		ioAUXAlpha.digitalWrite(EN1 + 2*i, HIGH); //Toggle enable to reset if needed
		ioAUXAlpha.digitalWrite(EN1 + 2*i, LOW);
		ioAUXAlpha.digitalWrite(EN1 + 2*i, HIGH);
	}
	ioAUXBeta.pinMode(LOAD, OUTPUT);
	ioAUXBeta.pinMode(RST, OUTPUT);
	ioAUXBeta.digitalWrite(LOAD, LOW); //Start with load in low position, will load on high going edge
	ioAUXBeta.digitalWrite(RST, HIGH); //Negate reset by default
	ioAUXBeta.setIntPinConfig(OVF1, false, HIGH); //Interrupt if OVF1 goes low
	ioAUXBeta.setIntPinConfig(OUT1, true); //Interrupt if OUT1 changes
	ioAUXBeta.setInterrupt(OVF1, true); //turn on interrupt for OVF1 pin
	ioAUXBeta.setInterrupt(OUT1, true); //turn on interrupt for OUT1 pin
	ioAUXBeta.clearInterrupt(BOTH); //Clean both interrupts 

	ioAUXAlpha.pinMode(REG_EN, OUTPUT);
	ioAUXAlpha.digitalWrite(REG_EN, HIGH); //Default 5V to on
	// ioAlpha.pinMode(ADC_INT, INPUT_PULLUP); //Setup ADC int as an input pullup for the open drain
	ioAUXAlpha.pinMode(ADC_INT, INPUT); //Setup ADC int as an input pullup for the open drain

	Serial.print("\tPort Sense:\n");
	for(int i = 8; i < 12; i++) { //Set all MUX select pins to output
		ioAUXAlpha.pinMode(i, OUTPUT); 
	}
	ioAUXAlpha.digitalWrite(MUX_EN, LOW); //Turn MUX on 
	ads.begin();

	adcSense.SetResolution(18); //Set to max resolution (we paid for it right?) 

	ioAUXAlpha.digitalWrite(MUX_SEL2, LOW); //Read voltages
	for(int i = 0; i < 3; i++){ //Increment through 4 voltages
		ioAUXAlpha.digitalWrite(MUX_SEL0, i & 0b01); //Set with lower bit
		ioAUXAlpha.digitalWrite(MUX_SEL1, (i & 0b10) >> 1); //Set with high bit
		delay(1); //Wait for voltage to stabilize
		Serial.print("\tPort");
		Serial.print(i);
		Serial.print(" Input:");
		Serial.print(ads.readADC_SingleEnded(3)*0.1875, 4); //Print high resolution voltage
		Serial.print(" mV\n");  
	}
	ioAUXAlpha.digitalWrite(MUX_SEL0, HIGH); //Set with lower bit
	ioAUXAlpha.digitalWrite(MUX_SEL1, HIGH); //Set with high bit
	delay(1); //Wait for voltage to stabilize
	Serial.print("\t5V Rail:");
	Serial.print(ads.readADC_SingleEnded(3)*0.1875, 4); //Print high resolution voltage
	Serial.print(" mV\n");  
	ioAUXAlpha.digitalWrite(MUX_SEL2, HIGH); //Read currents
	for(int i = 0; i < 3; i++){ //Increment through 4 voltages
		ioAUXAlpha.digitalWrite(MUX_SEL0, i & 0b01); //Set with lower bit
		ioAUXAlpha.digitalWrite(MUX_SEL1, (i & 0b10) >> 1); //Set with high bit
		delay(1); //Wait for voltage to stabilize
		Serial.print("\tPort");
		Serial.print(i);
		Serial.print(" Output:");
		Serial.print(ads.readADC_SingleEnded(3)*0.1875, 4); //Print high resolution voltage
		Serial.print(" mV\n");  
	}

	//SENSE VOLTAGES
	Serial.print("\tVoltage Sense:\n");
	

	for(int i = 0; i < 3; i++){ //Increment through 4 voltages
  		Serial.print("\t\tPort");
  		Serial.print(i);
  		Serial.print(": ");
  		Serial.print(ads.readADC_SingleEnded(i)*0.1875, 4); //Print high resolution voltage, Convert by standard gain 
  		Serial.print(" mV\n");  
	}

	//Read counters
	static uint16_t Count[3] = {0}; //Keep all counters as local statics to be returned at request 

	ioAUXBeta.digitalWrite(LOAD, HIGH); //Load all values
	delayMicroseconds(1); //Delay min number of microseconds to ensure a pulse
	ioAUXBeta.digitalWrite(LOAD, LOW);

	for(int i = 0; i < 3; i++) { //Get count n
		ioAUXBeta.digitalWrite(COUNT_EN1 + i, LOW); //Turn on outputs of counter n 
		Count[i] = ioAUXGamma.readBus(); //Grab bus
		ioAUXBeta.digitalWrite(COUNT_EN1 + i, HIGH); //Turn off outputs of countern
	}

	ioAUXBeta.digitalWrite(RST, LOW); //Reset counters
	delay(1);
	ioAUXBeta.digitalWrite(RST, HIGH); //Negate reset by default

	Serial.print("\tCounts:\n");

	for(int i = 0; i < 3; i++) {
		Serial.print("\t\tPort");
		Serial.print(i);
		Serial.print(": ");
		Serial.print(Count[i]);
		Serial.print(" Counts\n");
	}

	// delay(5000);
	// for(int i = 0; i < 3; i++) { //Shut down output power //DEBUG!
	// 	ioAUXAlpha.digitalWrite(EN1 + 2*i, LOW);
	// }
	// delay(5000);
	// ioAUXAlpha.digitalWrite(REG_EN, LOW); //Turn 5V off
}

void SendBreak()
{
	// io.pinMode(0, Dir, OUTPUT); //Make sure direction is set to output
	// io.pinMode(0, FOut, OUTPUT); //Make sure the force out pin is in output mode
	ioTalonSDI12.digitalWrite( Dir, HIGH); //Set direction to output
	ioTalonSDI12.digitalWrite( FOut, LOW); //Set low to force output high
	delay(13); //Wait for bus to acnowledge action
	ioTalonSDI12.digitalWrite( FOut, HIGH); //Stop forcing output
}

void Mark(unsigned long Period)
{
	// io.pinMode(0, Dir, OUTPUT); //Make sure direction control pin is set as an output
	digitalWrite(TX_SDI12, 1); //Preempt value before turning output on
	delay(1);
	pinMode(TX_SDI12, OUTPUT); //Make sure transmit pin is set as output
	ioTalonSDI12.digitalWrite( Dir, HIGH); //Set direction to output
	digitalWrite(TX_SDI12, 1); //Begin marking condition
	delay(Period); //Wait for a given marking period
	// digitalWrite(TX, 0); //Stop marking  
}

void Space(unsigned long Period)
{
	// io.pinMode(0, Dir, OUTPUT); //Make sure direction control pin is set as an output
	pinMode(TX_SDI12, OUTPUT); //Make sure transmit pin is set as output
	ioTalonSDI12.digitalWrite( Dir, HIGH); //Set direction to output
	digitalWrite(TX_SDI12, 0); //Begin spacing condition
	delay(Period); //Wait for a given marking period
	digitalWrite(TX_SDI12, 1); //Stop spacing
}

void ReleaseBus() 
{
	// io.pinMode(0, Dir, OUTPUT); //Make sure direction pin is set as an output
	ioTalonSDI12.digitalWrite( Dir, LOW); //Set direction to input
}

String Command(String Command, int Address) //Correctly place address and line end into string to pass to SendCommand
{
  Command = String(Address) + Command + "!";
  return SendCommand(Command); //Return results
}
String SendCommand(String Command) 
{
	SendBreak(); //Send break to start message
	Mark(MARKING_PERIOD); //Mark for standard period before command
	Serial1.begin(1200, SERIAL_8N1);
	Serial1.print(SerialConvert8N1to7E1(Command)); //Send converted value
	Serial1.flush(); //Make sure data is transmitted before releasing the bus
	delay(1); //DEBUG! Return to 1ms??
	ReleaseBus(); //Switch bus to recieve 

	unsigned long LocalTime = millis();
	char Data[100] = {0}; //Make data array for storage FIX! Change length to not be arbitrary
	bool GotData = false; //Used to keep track of terminating character has been recieved 
	int Pos = 0; //Keep track of position in data array
	while(!GotData && (millis() - LocalTime) <  TimeoutStandard) {
		if(Serial1.available() > 0) {
			Data[Pos] = SerialConvert7E1to8N1(Serial1.read()); //If byte is availiable, read converted version in
			Pos++; //Increment position

		}
    if(Data[Pos] == '\n') GotData = true; //Set flag if end character is read in //DEBUG!
	}
	String Val = String(Data); //Convert to String
	Val.trim(); //Get rid of any trailing characters 
	return Val; 
}

///////////////////SERIAL CONVERSION PATCH//////////////////////////////////
String SerialConvert8N1to7E1(String Msg) //Take input message in 8N1 format and convert to 7E1 format 
{
	char MsgBuf[25] = {0}; //Initalize a char buffer to break apart string message //FIX! Make variable? Or just larger?
	uint8_t BufLength = sizeof(MsgBuf); //Keep track of size of buffer being used
	Msg.toCharArray(MsgBuf, BufLength); //Break message string into an array of chars 
	for(int i = 0; i < BufLength; i++) {
		MsgBuf[i] = MsgBuf[i] | (GetParity(MsgBuf[i]) << 7); //Add parity bit to left most bit
	}
	// Serial.println(String(MsgBuf)); //DEBUG!
	return String(MsgBuf); //Convert array back to string and return this 
}

char SerialConvert7E1to8N1(char Msg) //Take input message in 8N1 format and convert to 7E1 format 
{
	return Msg & 0x7F; //Clear parity bit
}

bool GetParity(char Data) //Calculate partiy of a char of data
{
	bool Parity = 0;
	while(Data != 0) {
		Parity = !Parity; //Invert parity each time a '1' is encountered
		Data = Data & (Data - 1); //Clear next '1' in message
	}
	return Parity; 
}

// String SendCommand(String Command) 
// {
// 	SendBreak(); //Send break to start message
// 	Mark(MARKING_PERIOD); //Mark for standard period before command
// 	// pinMode(TX, INPUT);
// 	// delay(1);
// 	Serial1.begin(1200, SERIAL_7E1);
// 	Serial1.print(Command); 
// 	Serial1.flush(); //Make sure data is transmitted before releasing the bus
// 	delay(1); //DEBUG! Return to 1ms??
// 	ReleaseBus(); //Switch bus to recieve 
// //	io.digitalWrite(4, HIGH); //DEBUG! Turn off output buffer!
// 	// char Var = Serial.re

// 	unsigned long LocalTime = millis();
// 	char Data[100] = {0}; //Make data array for storage FIX! Change length to not be arbitrary
// 	bool GotData = false; //Used to keep track of terminating character has been recieved 
// 	int Pos = 0; //Keep track of position in data array
// 	while(!GotData && (millis() - LocalTime) <  TimeoutStandard) {
// 		if(Serial1.available() > 0) {
// 			Data[Pos] = Serial1.read(); //If byte is available, read it in
// 			Pos++; //Increment position
// 		}
// 		if(Data[Pos] == '\n') GotData = true; //Set flag if end character is read in
// 	}
// 	String Val = String(Data); //Convert to String
// 	Val.trim(); //Get rid of any trailing characters 
// 	// while(Serial1.available() < 3 && (millis() - LocalTime) <  TimeoutStandard);
// 	// String Val = Serial1.readStringUntil('\r');
// 	// Serial.println(Val);
// //	io.digitalWrite(4, LOW); //DEBUG! Turn on output buffer!
// 	return Val; 
// }

void I2C_GlobalEn(bool State)
{
	pinMode(Pins::I2C_GLOBAL_EN, OUTPUT);
	digitalWrite(Pins::I2C_GLOBAL_EN, State);
}

void I2C_OnBoardEn(bool State)
{
	pinMode(Pins::I2C_OB_EN, OUTPUT);
	digitalWrite(Pins::I2C_OB_EN, State);
}

void SetDefaultPins()
{
	//FIX! Update for Kestrel! 
	// digitalWrite(I2C_EN, LOW); //Turn off external I2C
	// for(int i = 0; i < 8; i++) { //Enable all power, then all data
	// 	io.pinMode(i, OUTPUT);
	// 	io.digitalWrite(i, HIGH);
	// } 

	// for(int i = 8; i < 12; i++) { //Set FAULT lines as input pullups
	// 	io.pinMode(i, INPUT_PULLUP);
	// }

	// io.pinMode(POS_DETECT, INPUT); //Set position detect as normal pullup (has external pullup)
	// io.pinMode(SENSE_EN, OUTPUT); //Set sense control as output
	// io.digitalWrite(SENSE_EN, HIGH); //Default sense to on 
	// digitalWrite(I2C_EN, HIGH); //Turn on external I2C
}



