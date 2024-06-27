#ifndef main_h
#define main_h

#include <Arduino.h>

#define ADC_READ_INTERVAL   100
#define OLED_DISPLAY_INTERVAL   500

void ADC_Read_Task(void *parameters);
void OLED_Display_Task(void *parameters);

#endif