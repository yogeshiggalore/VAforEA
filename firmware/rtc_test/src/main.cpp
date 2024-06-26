/*
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  Serial.begin(115200);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void loop () {
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours, 30 minutes, and 6 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();

    Serial.println();
    delay(3000);
}
*/

#include <Arduino.h>
#include <DS1307.h>
#include <Wire.h>

#define MAX_I2C_ADDRESS 127

uint8_t set_Sec = 0;	  
uint8_t set_Minute = 47;  
uint8_t set_Hour = 3;	  
uint8_t set_Day = 13;	  
uint8_t set_Month = 05;	  
uint16_t set_Year = 2022; 

uint8_t sec, minute, hour, day, month;
uint16_t year;
DS1307 rtc;

void scan_i2c_devices(void);

void setup(void)
{
	Serial.begin(115200); 
	Wire.begin();
	
	delay(3000);
	
	rtc.begin();
	//03:47:00 13.05.2022 //sec, min, hour, day, month, year
	rtc.set(set_Sec, set_Minute, set_Hour, set_Day, set_Month, set_Year);
	rtc.stop();

	rtc.start();
	delay(1000); 
	Serial.print("You have set: ");
	Serial.print("\nTime: ");
	Serial.print(set_Hour, DEC);
	Serial.print(":");
	Serial.print(set_Minute, DEC);
	Serial.print(":");
	Serial.print(set_Sec, DEC);

	Serial.print("\nDate: ");
	Serial.print(set_Day, DEC);
	Serial.print(".");
	Serial.print(set_Month, DEC);
	Serial.print(".");
	Serial.print(set_Year, DEC);
	Serial.println("");
	
}

void loop(void)
{
	rtc.get(&sec, &minute, &hour, &day, &month, &year);
	printf("%d:%d:%d %d-%d-%d\n", sec, minute, hour, day, month, year);
	//scan_i2c_devices();
	delay(5000);

}

void scan_i2c_devices(void)
{
	uint8_t *dataptr;
	uint8_t I2CAddr;
	uint8_t status;
	uint8_t numberOfbytes;

	Serial.print("started scanning i2c devices\n\n");
	Serial.print("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n00:    ");
	for (I2CAddr = 1; I2CAddr <= MAX_I2C_ADDRESS; I2CAddr++)
	{
		if ((I2CAddr % 0x10) == 0)
		{
			Serial.printf("\n%2X: ", I2CAddr);
		}

		Wire.beginTransmission(I2CAddr);
    	status = Wire.endTransmission();
		if (status == 0)
		{
			Serial.printf("%2X ", I2CAddr);
		}
		else if( status == 4 )
		{
			Serial.print("XX ");
		}
		else
		{
			Serial.print("-- ");
		}

		
		delay(100);
	}
	Serial.print("\n\nstopped scanning i2c devices\n");
}
