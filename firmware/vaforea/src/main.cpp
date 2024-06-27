#include <Arduino.h>
#include <main.h>
#include "app_vaforea.h"
#include "voice_ctrl.h"

struct voice_config vc_cfg;

TaskHandle_t th_ADC_Read;
TaskHandle_t th_Display;

void setup()
{
	app_begin();
	app_scan_i2c_devices();
	xTaskCreatePinnedToCore( ADC_Read_Task, " Task ADC read", 10000, NULL, 0,  &th_ADC_Read, 0);
	xTaskCreatePinnedToCore( OLED_Display_Task, " Task display", 10000, NULL, 0,  &th_Display, 0); 
}

void loop()
{
	app_read();
	app_process();
	app_write();
	delay(5);
}

void ADC_Read_Task(void *parameters)
{
	for(;;)
	{
		app_ads_read();
		vTaskDelay( ADC_READ_INTERVAL / portTICK_PERIOD_MS);
	}
}

void OLED_Display_Task(void *parameters)
{
	for(;;)
	{
		app_display_test();
		vTaskDelay( OLED_DISPLAY_INTERVAL / portTICK_PERIOD_MS);
	}
}