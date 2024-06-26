/*******************************************************************************
* File Name: app.h
*
* Version: 1.0
*
* Description:
* This is header file for application module for VAforEA project
*
********************************************************************************
* VAforEA (2024-25)
********************************************************************************/

#ifndef app_vaforea_h
#define app_vaforea_h
#include <Arduino.h>
#include <voice_ctrl.h>
#include <ads1115.h>
#include <mcp4725.h>

#define MAX_I2C_ADDRESS	127
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define APP_MEMORY_SIZE		9

#define DAC_100_PERCENT		100

struct mem_config
{
	uint16_t voltage;
	uint16_t current;
	uint16_t input_voltage;
	uint16_t input_current;
	uint8_t  max_percent;
};

struct app_config
{
	struct voice_config   vc;
	struct ads1115_config ads;
	struct mcp4725_config mcpVolt;
	struct mcp4725_config mcpCurr;
	struct mem_config mem;
};

struct re_data
{
	long prv;
	int8_t cnt;
};

struct xl4015
{
	int8_t  curr_volt_per;
	int16_t curr_voltage;
	int16_t voltage_backup;
	int16_t curr_current;
	uint8_t  exp_volt_per;
	uint16_t exp_voltage;
	uint16_t exp_current;
	uint16_t meas_volt;
	uint16_t meas_curr;
	uint16_t input_volt;
};

struct app_data
{
	struct voice_data vc;
	struct ads1115_data ads;
	struct mcp4725_data mcpVolt;
	struct mcp4725_data mcpCurr;
	struct re_data reVolt;
	struct re_data reCurr;
	struct xl4015  xl;
	bool vc_rqt;
	bool vc_process;
	bool vc_complete;
};

int app_begin( void );
int app_hardware_init(void);
void app_read(void);
void app_process(void);
void app_write(void);
void app_scan_i2c_devices(void);

void app_vc_init(void);
void app_vc_cb_on_cmd_rx(uint8_t cmd, uint16_t value);
void app_vc_cb_on_err(uint8_t err);
void app_vc_take_action(void);
void app_vc_process_command(void);

void app_re_init(void);
void app_re_volt_cb_trig(long val);
void app_re_curr_cb_trig(long val);

void app_ads_init(void);
void app_ads_read(void);

void app_mcp_init(void);
void app_mcp_set_voltage( uint8_t cmd, uint16_t value );

void app_display_init(void);
void app_display_test(void);

void app_eeprom_init(void);

void app_test_dac_with_percent( void );

#endif