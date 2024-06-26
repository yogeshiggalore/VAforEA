#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#define MAX_I2C_ADDRESS 127

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

void scan_i2c_devices(void);

uint8_t value=0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("code started....");
	Wire.begin();

	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
	{ 
    	for(;;)
		{
			Serial.println(F("SSD1306 allocation failed"));
			delay(1000);
		}
  	}
}

void loop()
{
	scan_i2c_devices();
	//delay(5000);

	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0, 2);
	display.println("hello");
	
	display.setTextSize(3);
	display.setTextColor(WHITE);
	display.setCursor(0, 20);
	display.print((int) value++);
	
	display.display();

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

		/* 100 ms sleep */
		delay(100);
	}
	Serial.print("\n\nstopped scanning i2c devices\n");
}