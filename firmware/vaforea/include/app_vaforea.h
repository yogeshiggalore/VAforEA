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
#include <mcp4725.h>

#define MAX_I2C_ADDRESS	127
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define APP_MEMORY_SIZE		9

#define DAC_100_PERCENT		100

#define RELAY_ON	1
#define RELAY_OFF	0
#define RELAY_UNKNW	0xFF

#define RELAY_PIN	4

#define USE_RELAY_CONTROL	1

enum VOICE_PC_ACT_CMD
{
	VOICE_PC_ACT_CMD_SCRNSHT=0x80,
	VOICE_PC_ACT_CMD_VLTRED=0x81,
	VOICE_PC_ACT_CMD_CURRED=0x82,
	VOICE_PC_ACT_CMD_RLYSTS=0x83,
	VOICE_PC_ACT_CMD_ERR = 0xFF,
};

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

	int16_t curr_curr_per;
	int16_t curr_current;
	int16_t current_backup;

	uint8_t  exp_volt_per;
	uint16_t exp_voltage;
	
	uint8_t  exp_curr_per;
	uint16_t exp_current;
	
	float meas_volt;
	float meas_curr;
	uint16_t input_volt;

	uint8_t relay_status;
};

struct app_data
{
	struct voice_data vc;
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
uint8_t app_vc_process_command(void);

void app_re_init(void);
void app_re_volt_cb_trig(long val);
void app_re_curr_cb_trig(long val);

void app_ads_init(void);
void app_ads_read(void);

void app_mcp_init(void);
void app_mcp_set_voltage( uint8_t cmd, uint16_t value );
void app_mcp_set_current( uint8_t cmd, uint16_t value );

void app_display_init(void);
void app_display_test(void);

void app_relay_init(void);
void app_relay_control(bool val);

void app_pc_act_send(void);

void app_eeprom_init(void);

void app_test_dac_with_percent( void );

#endif