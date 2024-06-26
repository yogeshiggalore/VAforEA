#ifndef main_h
#define main_h

#include <Arduino.h>

#define ADC_READ_INTERVAL   1000

void ADC_Read_Task(void *parameters);

#endif