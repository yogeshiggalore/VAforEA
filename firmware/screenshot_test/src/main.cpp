#include <Arduino.h>

/* ss buffer */
uint8_t ss_buf[9] = {0xAA, 0x55, 0x80, 0x00, 0x00, 0xA5, 0x5A, 0x0D, 0x0A};
uint8_t volt_buf[9] = {0xAA, 0x55, 0x81, 0x00, 0x00, 0xA5, 0x5A, 0x0D, 0x0A};
uint16_t voltage=0;

#define RELAY_ON	1
#define RELAY_OFF	0

#define RELAY_PIN	4

void app_relay_init(void)
{
	pinMode( RELAY_PIN, OUTPUT );
	digitalWrite( RELAY_PIN, RELAY_OFF );
}

void app_relay_control(bool val)
{
	if( val == RELAY_ON )
	{
		digitalWrite( RELAY_PIN, RELAY_ON );
	}
	else
	{
		digitalWrite( RELAY_PIN, RELAY_OFF );
	}
}

void setup()
{
	Serial.begin(115200);
	Serial.println("code started\n");
	app_relay_init();
}

void loop()
{
	Serial.println("random");
	Serial.write(ss_buf, sizeof(ss_buf));
	Serial.println("wrongdata");
	Serial.println("jkjlkj\r");
	for(int i=0; i<10; i++)
	{
		voltage += 500;
		if( voltage > 12000 )
		{
			voltage = 0;
		}
		volt_buf[3] = voltage >> 8;
		volt_buf[4] = voltage;
		Serial.write(volt_buf, sizeof(volt_buf));
		delay(1000);
	}
}
