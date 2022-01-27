#include <Wire.h>
 // #include "Adafruit_MCP23008.h"
// #include <Adafruit_MCP23X17.h>
// #include <MCP2301x.h>
#include <Arduino.h>
#include <MCP23018.h>

// #define NUM_MCP2301x_ICs   1

// const uint8_t I2C_GLOBAL_EN = 0; //FIX!

namespace Pins { //Use for Feather M0 Express
	// constexpr uint16_t WD_HOLD  = 12;
	// constexpr uint16_t EXT_EN   = 13;
	constexpr uint16_t SD_EN    = 1;
	constexpr uint16_t SD_CS    = 19;
	constexpr uint16_t SD_CD    = 18;
	// constexpr uint16_t SDA_CTRL = 5;
	// constexpr uint16_t TIP      = 18;
	// constexpr uint16_t GPIOXA 	= 17;
	constexpr uint16_t Clock_INT 	= 6;
	// constexpr uint16_t Port1D 	= 11;
	// constexpr uint16_t Port2D 	= 10;
	// constexpr uint16_t Port3D  	= 9;
	// constexpr uint16_t Port1A 	= A2;
	// constexpr uint16_t Port2A 	= A1;
	// constexpr uint16_t Port3A 	= A0;
	constexpr uint16_t I2C_GLOBAL_EN = 0; //FIX!
	constexpr uint16_t I2C_OB_EN = 5; //FIX!
}

const uint8_t TX = 1;
const uint8_t RX = 0;
const uint8_t FOut = 1;
const uint8_t Dir = 0;

//IO Expander pins
const uint8_t POS_DETECT = 12; 
const uint8_t SENSE_EN = 13;

//IO Sense pins
const uint8_t MUX_EN = 3;
const uint8_t MUX_SEL0 = 0;
const uint8_t MUX_SEL1 = 1;
const uint8_t MUX_SEL2 = 2;

const float VoltageDiv = 3; //Program voltage divider
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
PAC1934 CSA_Beta(2, 2, 2, 2, 0x14); //Initalize with R6 = 2m, R15 = 10m, R7 = 2m, R8 = 2m, ADR = 0x14

/////////// SD /////////////////
#include <SD.h>
#include <SPI.h>
char FileNameC[11]; //Used for file handling
boolean SD_Init = false;

/////////// FRAM /////////////

/////////// RTC /////////////
#include "MCP7940.h" //FIX! Update
MCP7940 Clock; // Instantiate Clock


//////////// LED ////////////
#include <PCA9634.h>

PCA9634 led(0x50); //Instatiate LED driver with 0x50 address





void setup() 
{
	pinMode(I2C_EN, OUTPUT); 
	digitalWrite(I2C_EN, HIGH); //Turn on external port connection
	Serial.begin(115200);

	Wire.begin();
	SetDefaultPins();
	// SetPinMode();
	// SetPullups();
	// io.digitalWrite(6, HIGH); //Turn on 5V
	// io.digitalWrite(7, HIGH); //Turn on 12V
	// io.digitalWrite(2, LOW); //Enable Data 1
	// io.digitalWrite(3, LOW); //Enable Data 2
	// io.digitalWrite(4, LOW); //Enable Data 3

	
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
		
	}

	if(Input == '*') {
		ReadString = String(ReadArray);
		ReadString.trim();
		memset(ReadArray, 0, sizeof(ReadArray));
		ReadLength = 0;

		Serial.print(">");
		Serial.println(ReadString); //Echo back to serial monitor

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

		if(ReadString.equalsIgnoreCase("FRAM")) {
			TestFRAM(); //Call FRAM test
		}

		if(ReadString.equalsIgnoreCase("RTC")) {
			TestRTC(); //Call RTC test
			//Include EEPROM??
		}

		if(ReadString.equalsIgnoreCase("LED")) {
			TestLED(); //Call LED test
		}

		if(ReadString.equalsIgnoreCase("Battery")) {
			TestBattery(); //Read battery state from Gonk 
		}

		if(ReadString.equalsIgnoreCase("I2C")) {
			TestI2C(); //Run I2C scan 
		}

		if(ReadString.equalsIgnoreCase("Sensor1")) {
			SensorDemo1(); //Run sensor 1 demo  
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
		

		digitalWrite(I2C_EN, LOW); //Turn off external port connection
		if(Operation.equals("M")) { //if call is for pinmode setting
			switch(Device) {
				case 'D':
					pinMode(Pin, State); //Use normal state logic 
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
					digitalWrite(Pin, State);
					Result = 0; //Set manually
					break;
				case 'A':
					Result = ioAlpha.digitalWrite(Pin, State);
					break;
				case 'B':
					Result = ioBeta.digitalWrite(Pin, State);
					break;
			}
		}
		digitalWrite(I2C_EN, HIGH); //Turn on external port connection
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

}

void TestAtmos() 
{
	I2C_GlobalEn(false); //Turn off external I2C
	I2C_OnBoardEn(true); //Turn on local I2C
	if(RH.begin() == 0) { //Setup RH with defaults
		Wire.begin();
		Wire.setClock(400000); //Confirm operation in fast mode
		float AtmosData[2] = {0}; //Create array for envrio data
		RH.Wake(); //Wake RH sensor
		RH.GetData(AtmosData); //Read data from sensor into array
		RH.Sleep(); //Sleep sensor again to reduce power

		Serial.print("\tHumidity = ");
		Serial.print(AtmosData[0]); //Print RH
		Serial.print(" %\t");
		Serial.print("\tTemperature = ");
		Serial.print(AtmosData[1]); //Print temp
		Serial.print(" ºC\n"); 
	}
	else Serial.println("\tERROR: SHTC3");
	I2C_GlobalEn(true); //Turn back on external I2C
}

void TestMag(bool Set, bool Reset) 
{
	const uint8_t ADR = 0x30;
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	// Wire.begin();
	// Wire.setClock(100000); //Confirm operation in standard mode

	uint16_t DataRaw[3] = {0}; //X, Y, Z accel data, raw
	float Data[3] = {0}; //Converted X, Y, Z accel data [g]

	Wire.beginTransmission(ADR);
	Wire.write(0x1C); //Write to Control Register 1
	// Wire.write(0x80); //Call for chip reset
	Wire.write(0x00); //Clear reg
	int Error = Wire.endTransmission(true);

	// delay(500);

	if(Set && !Reset) {
		Wire.beginTransmission(ADR);
		Wire.write(0x1B); //Write to Control Register 0
		// Wire.write(0x21); //Set both magnetic measurment bit and periodic measurment bit
		Wire.write(0x80); //Trigger SET
		// Wire.write(0x23); //Set both magnetic and temp measurment bits and periodic measurment bit
		Error = Wire.endTransmission(true);
		Serial.println("\tSET");
	}

	if(Reset && !Set) {
		Wire.beginTransmission(ADR);
		Wire.write(0x1B); //Write to Control Register 0
		// Wire.write(0x21); //Set both magnetic measurment bit and periodic measurment bit
		Wire.write(0x10); //Trigger RESET
		// Wire.write(0x23); //Set both magnetic and temp measurment bits and periodic measurment bit
		Error = Wire.endTransmission(true);
		Serial.println("\tRESET");
	}

	Wire.beginTransmission(ADR);
	Wire.write(0x1B); //Write to Control Register 0
	// Wire.write(0x21); //Set both magnetic measurment bit and periodic measurment bit
	Wire.write(0x21); //Set both magnetic measurment bit and reset bit
	// Wire.write(0x23); //Set both magnetic and temp measurment bits and periodic measurment bit
	Error = Wire.endTransmission(true);

	if(Error != 0) {
		Serial.print("\tERROR: MMC5633 - ");
		Serial.println(Error);
	}
	else {
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
				Wire.endTransmission(true);

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
	}


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
	pinMode(Pins::SD_EN, OUTPUT); //Ensure EN pin is setup as output
	digitalWrite(Pins::SD_EN, HIGH); //Set enable pin to turn on power

	boolean SDError = false;
	pinMode(Pins::SD_CS, OUTPUT);
	SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
	// SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0)); //Test max speed mode
	// SPI.setDataMode(SPI_MODE0);
	// SPI.setClockDivider(SPI_CLOCK_DIV2); //Sts SPI clock to 4 MHz for an 8 MHz system clock

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

	SD.remove(FileNameC); //Remove dummy file created 
	Serial.print("\tResult: ");
	Serial.println(SDError);
	return SDError;
}

void TestFRAM() 
{
	const uint8_t ADR = 0x57; //Address for FRAM
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	Wire.setClock(400000); //Confirm operation in fast mode
	randomSeed(millis()); //Seed with a random number to try to endsure randomness
	uint8_t RandVal = random(231); //Generate a random number between 0 and 231 (pages in Soul of a New Machine)
	int RandPos[2] = {0};
	RandPos[0] = random(255); //Generate a random offset
	RandPos[1] = random(255); //Generate a random offset

	//Wake up from sleep
	Wire.beginTransmission(ADR);
	Wire.endTransmission();
	delayMicroseconds(500); //Delay at least t_Rec (400us)

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

	//Set device to sleep mode - Hacky but tested and it works. See Page 12 of datasheet for details 
	Wire.beginTransmission(0x7C);
	Wire.write((ADR << 1) | 0x01); //Shift to add "r/w" bit
	Wire.endTransmission(false);
	Wire.beginTransmission(0x43);
	Wire.endTransmission();

	I2C_GlobalEn(true);
}

void TestRTC() 
{
	bool Clock_Error = 1; //Default error
	// pinMode(Pins::SDA_CTRL, OUTPUT); //Setup SDA control as an output
	// digitalWrite(Pins::SDA_CTRL, HIGH); //Set turn on on board SDA
	I2C_GlobalEn(false);
	I2C_OnBoardEn(true);
	Wire.setClock(400000); //Confirm operation in fast mode
	pinMode(Pins::Clock_INT, INPUT); //Setup Clock pin as input
	Serial.print("\tCurrent Time: ");
	Serial.println(Clock.GetTime(MCP7940::Format::ISO_8601)); //FIX! //Grab time from Clock)
	Clock.Begin();
	Clock.ClearAlarm(0);
	Clock.ClearAlarm(1);
	Clock.SetMode(MCP7940::Mode::Inverted); //Use dual alarm setup 
	Clock.SetTime(2021, 06, 10, 2, 12, 0, 0); //Set arbitrary time (incept date) //DEBUG!
	Clock.SetAlarm(5); //Set an alarm for 5 seconds from now

	unsigned long LocalTime = millis();
	Serial.print("\tAlarm State = "); 
	Serial.println(Clock.ReadAlarm());
	Serial.print("\tWaiting for alarm...\n");
	while(digitalRead(Pins::Clock_INT) != HIGH && (millis() - LocalTime < 7000)); //Wait for Clock to trigger, or 7 seconds to go by 
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

	//////////// WRITE/READ SRAM //////////////
	randomSeed(millis()); //Seed with a random number to try to endsure randomness
	uint8_t RandVal = random(231); //Generate a random number between 0 and 231 (pages in Soul of a New Machine)
	int RandPos = random(63); //Generate a random offset
	Wire.beginTransmission(0x6F);
	Wire.write(RandPos + 0x20); //Command write to random position 
	Wire.write(RandVal);
	uint8_t Error = Wire.endTransmission();

	Serial.print("\tSRAM Write:\n");
	Serial.print("\t\tData: ");
	Serial.println(RandVal);
	Serial.print("\t\tPos: ");
	Serial.println(RandPos);
	Serial.print("\t\tError: ");
	Serial.println(Error);

	Wire.beginTransmission(0x6F);
	Wire.write(RandPos + 0x20); //Command write to random position 
	Error = Wire.endTransmission();

	Wire.requestFrom(0x6F, 1);
	RandVal = Wire.read(); //Read single byte back

	Serial.print("\tSRAM Read:\n");
	Serial.print("\t\tData: ");
	Serial.println(RandVal);
	Serial.print("\t\tPos: ");
	Serial.println(RandPos);
	Serial.print("\t\tError: ");
	Serial.println(Error);

	//////////// WRITE/READ EEPROM /////////////
	randomSeed(millis()); //Seed with a random number to try to endsure randomness
	RandVal = random(231); //Generate a random number between 0 and 231 (pages in Soul of a New Machine)
	RandPos = random(127); //Generate a random offset

	Wire.beginTransmission(0x57); //EEPROM address
	Wire.write(RandPos + 0x20); //Command write to random position 
	Wire.write(RandVal);
	Error = Wire.endTransmission();

	Serial.print("\tEEPROM Write:\n");
	Serial.print("\t\tData: ");
	Serial.println(RandVal);
	Serial.print("\t\tPos: ");
	Serial.println(RandPos);
	Serial.print("\t\tError: ");
	Serial.println(Error);

	LocalTime = millis();
	//Poll for write cycle to complete (see page 41 of datasheet)
	do { //Max EEPROM write time is 5ms, wait for cycle to finish or to timeout
		Wire.beginTransmission(0x57); //EEPROM address
		Wire.write(RandPos + 0x20); //Command write to random position 
		Error = Wire.endTransmission();
	} while(Error != 0 && (millis() - LocalTime) < 6);

	if(Error != 0) Serial.print("\tERROR: Write Cycle Timeout"); //Throw error if timeout occoured 

	Wire.beginTransmission(0x57); //EEPROM address
	Wire.write(RandPos + 0x20); //Command write to random position 
	Error = Wire.endTransmission();

	Wire.requestFrom(0x57, 1);
	RandVal = Wire.read(); //Read single byte back

	Serial.print("\tEEPROM Read:\n");
	Serial.print("\t\tData: ");
	Serial.println(RandVal);
	Serial.print("\t\tPos: ");
	Serial.println(RandPos);
	Serial.print("\t\tError: ");
	Serial.println(Error);

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
	Wire.setClock(400000); //Confirm operation in fast mode

	Wire.beginTransmission(0x36);
	Wire.write(0x09); //Write to voltage register
	Wire.endTransmission();

	int Data = 0; 
	Wire.requestFrom(0x36, 2);
	Data = Wire.read();
	Data = Data | (Wire.read() << 8);
	Serial.print("\tCell Voltage: ");
	Serial.println(float(Data)*0.078125); //Print voltage in mV

	Wire.beginTransmission(0x36);
	Wire.write(0x40); //Write to led register //DEBUG!
	Wire.write(0b11100100); //Set for always on, 4 bars
	// Wire.write(0b10100100); //Set for push button control, 4 bars
	Wire.write(0b01110010); //Default, but breathing LEDs
	
	
	Wire.endTransmission();
}

void TestI2C() 
{
	// Wire.begin();
	// Wire.setClock(100000); //Make sure all work in standard mode
	int Error = 0; 
	for(int i = 0; i < 128; i++) {
		
		Wire.beginTransmission(i);
		Wire.write(0x00);
		Error = Wire.endTransmission();

		if(Error != 2) {
			Serial.print("\t0x");
			Serial.print(i, HEX);
			Serial.print(" - ");
			if(Error == 0) Serial.println("PASS");
			if(Error == 4 || Error == 1 || Error == 3) {
				Serial.print("ERROR - ");
				Serial.println(Error);
			}
		}
		delay(1); //DEBUG! 
	}
}

void SensorDemo1() 
{
	// // Serial.println(">Spector1");
	// /////////////// SETUP /////////////////////
	// digitalWrite(I2C_EN, LOW); //Turn on external I2C
	// uint8_t error_dps368 = 0; //DEBUG! FIX! Change library to return stuff properly
	// pressureSense.begin(Wire); //Initialize DPS368 sensor //DEBUG! FIX! Change library to return stuff properly
	// uint8_t error_sht31 = rhSense.begin(0x44); //Initialize SHT31 sensor
	
	// Serial.println(">Haar Status:"); //Print resulting sensor status 
	// Serial.print("\tDPS368: "); 
	// if(error_dps368 == 0) Serial.print("PASS\n"); //Print error grabbed from sensor
	// else Serial.print("FAIL!\n"); //Fail if result is not 0
	// Serial.print("\tSHT31: ");
	// if(error_sht31 == 1) Serial.print("PASS\n"); //Print error grabbed from sensor
	// else Serial.print("FAIL!\n"); //Fail if result is 0

	// ///////////////// RUN ////////////////////
	// float temp_dps368; //Storage for temp measurement from DPS368 
 //  	float pressure; //Storage for pressure measurment from DPS368
  	

	// int16_t error1 = pressureSense.measureTempOnce(temp_dps368, dps368_oversampling); //Grab new temp values [°C]
	// int16_t error2 = pressureSense.measurePressureOnce(pressure, dps368_oversampling); //Grab new pressure values [Pa]
	// pressure = pressure/100.0f; //Convert from Pa to mBar, because Pa is a stupid unit, and hPa is more stupid. 

	// float humidity = rhSense.readHumidity(); //Grab new humidity values [%]
	// float temp_sht31 = rhSense.readTemperature(); //Grab new temp values [°C]
	// digitalWrite(I2C_EN, HIGH); //Turn off external I2C

	// Serial.print("\tTemp (SHT31): "); Serial.print(temp_sht31, 4); Serial.print(" °C\t"); //Print temp to 4 decimal places
	// Serial.print("Temp (DPS368): "); Serial.print(temp_dps368, 2); Serial.print(" °C\t"); //Print temp to 2 decimal places
	// Serial.print("Pressure: "); Serial.print(pressure, 2); Serial.print(" mBar\t"); //Print pressure to 2 decimal places
	// Serial.print("Humidity: "); Serial.print(humidity, 2); Serial.print(" %\n"); //Print humidity to 2 decimal places
}

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



