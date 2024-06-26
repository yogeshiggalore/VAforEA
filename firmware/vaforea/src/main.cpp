#include <Arduino.h>
#include <main.h>
#include "app_vaforea.h"
#include "voice_ctrl.h"

struct voice_config vc_cfg;

TaskHandle_t th_ADC_Read;

void setup()
{
	app_begin();
	app_scan_i2c_devices();
	xTaskCreatePinnedToCore( ADC_Read_Task, " Task ADC read", 10000, NULL, 0,  &th_ADC_Read, 0); 
}

void loop()
{
	app_read();
	app_process();
	app_write();
	delay(500);
}

void ADC_Read_Task(void *parameters)
{
	for(;;)
	{
		app_ads_read();
		vTaskDelay( ADC_READ_INTERVAL / portTICK_PERIOD_MS);
	}
}