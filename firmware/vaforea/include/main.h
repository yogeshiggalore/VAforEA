#ifndef main_h
#define main_h

#include <Arduino.h>

#define ADC_READ_INTERVAL   100
#define MONITOR_INTERVAL   500

void ADC_Read_Task(void *parameters);
void Monitor_Task(void *parameters);

#endif