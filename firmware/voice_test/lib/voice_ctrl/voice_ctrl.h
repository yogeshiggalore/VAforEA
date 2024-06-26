/*******************************************************************************
* File Name: voice_ctrl.h
*
* Version: 1.0
*
* Description:
* This is header file for voice control module in VAforEA project
*
********************************************************************************
* VAforEA (2024-25)
********************************************************************************/

/*

sl no	behaviour	    command word	                    reply word	                            uart data

1	    PwrTurnOn	    turn on power supply	            turning on power supply	                AA 55 01 00 01 A5 5A 0D 0A
2	    PwrTurnOff	    turn off power supply	            turning off power supply	            AA 55 01 00 00 A5 5A 0D 0A
3	    PwrSVFive	    set voltage to five	                setting voltage to five	                AA 55 02 13 88 A5 5A 0D 0A
4	    PwrSVThree	    set voltage to three point three	setting voltage to three point three	AA 55 02 0C E4 A5 5A 0D 0A
5	    PwrSVTwelve	    set voltage to twelve	            setting voltage to twelve	            AA 55 02 2E E0 A5 5A 0D 0A
6	    PwrVIOne	    up voltage by one	                voltage up by one	                    AA 55 03 03 E8 A5 5A 0D 0A
7	    PwrVIPFive	    up voltage by point five	        voltage up by point five	            AA 55 03 01 F4 A5 5A 0D 0A
8	    PwrVIPOne	    up voltage by point one	            voltage up by point one	                AA 55 03 00 64 A5 5A 0D 0A
9	    PwrVDOne	    down voltage by one	                voltage down by one	                    AA 55 04 03 E8 A5 5A 0D 0A
10	    PwrVDPFive	    down voltage by point five	        voltage down by point five	            AA 55 04 01 F4 A5 5A 0D 0A
11	    PwrVDPOne	    down voltage by point one	        voltage down by point one	            AA 55 04 00 64 A5 5A 0D 0A
12	    PwrSCOne	    set current to one	                setting current to one	                AA 55 05 03 E8 A5 5A 0D 0A
13	    PwrSCTwo	    set current to two	                setting current to two	                AA 55 05 07 D0 A5 5A 0D 0A
14	    PwrSCThree	    set current to three	            setting current to three	            AA 55 05 0B B8 A5 5A 0D 0A
15	    PwrSCFour	    set current to four	                setting current to four	                AA 55 05 FA 00 A5 5A 0D 0A
16	    PwrCIPFive	    up current by point five	        current up by point five	            AA 55 06 01 F4 A5 5A 0D 0A
17	    PwrCIPOne	    up current by point one	            current up by point one	                AA 55 06 00 64 A5 5A 0D 0A
18	    PwrCDPFive	    down current by point five	        current down by point five	            AA 55 07 01 F4 A5 5A 0D 0A
19	    PwrCDPOne	    down current by point one	        current down by point one	            AA 55 07 00 64 A5 5A 0D 0A
20		PwrScrShot		take screenshot						taking screenshot						AA 55 08 00 00 A5 5A 0D 0A

*/

#ifndef voice_ctrl_h
#define voice_ctrl_h
#include <Arduino.h>

#define RX_BUFFER_SIZE	30
#define MAX_PKT_SIZE	9

typedef void (*vc_cb_on_cmd_rx)(uint8_t cmd, uint16_t value);
typedef void (*vc_cb_on_err)(uint8_t err);

enum SELECT_UART_PORT
{
	SELECT_UART_PORT_NONE=0,
	SELECT_UART_PORT_ZERO,
	SELECT_UART_PORT_ONE,
	SELECT_UART_PORT_TWO,
};

enum VOICE_CTRL_ERR
{
	VOICE_CTRL_ERR_NONE=0,
	VOICE_CTRL_ERR_PORT,
	VOICE_CTRL_ERR_PKT_FORMAT,
	VOICE_CTRL_ERR_CMD,
	VOICE_CTRL_ERR_MAX,
};

enum VOICE_CTRL_CMD
{
	VOICE_CTRL_CMD_NONE=0,
	VOICE_CTRL_CMD_ONOFF,
	VOICE_CTRL_CMD_SET_VOLT,
	VOICE_CTRL_CMD_UP_VOLT,
	VOICE_CTRL_CMD_DOWN_VOLT,
	VOICE_CTRL_CMD_SET_CURR,
	VOICE_CTRL_CMD_UP_CURR,
	VOICE_CTRL_CMD_DOWN_CURR,
	VOICE_CTRL_CMD_ACTION,
	VOICE_CTRL_CMD_ERR,
};

enum VOICE_CTRL_BYTE_CODE
{
	VOICE_CTRL_BYTE_CODE_AA = 0xAA,
	VOICE_CTRL_BYTE_CODE_55 = 0x55,
	VOICE_CTRL_BYTE_CODE_A5 = 0xA5,
	VOICE_CTRL_BYTE_CODE_5A = 0x5A,
	VOICE_CTRL_BYTE_CODE_CR = 0x0D,
	VOICE_CTRL_BYTE_CODE_LF = 0x0A,
};

enum VOICE_CTRL_BYTE_IDX
{
	VOICE_CTRL_BYTE_IDX_AA = 0,
	VOICE_CTRL_BYTE_IDX_55 = 1,
	VOICE_CTRL_BYTE_IDX_CC = 2,
	VOICE_CTRL_BYTE_IDX_D1 = 3,
	VOICE_CTRL_BYTE_IDX_D2 = 4,
	VOICE_CTRL_BYTE_IDX_A5 = 5,
	VOICE_CTRL_BYTE_IDX_5A = 6,
	VOICE_CTRL_BYTE_IDX_CR = 7,
	VOICE_CTRL_BYTE_IDX_LF = 8,
};

struct voice_config
{
	enum SELECT_UART_PORT port;
	vc_cb_on_cmd_rx cb_on_cmd_rx;
	vc_cb_on_err cb_on_err;
};

struct voice_data
{
	uint8_t err;
	uint8_t rx_buf[RX_BUFFER_SIZE];
	uint8_t rx_cntr;
	uint8_t cmd;
	uint16_t value;
};

class voice_ctrl
{
	public:
		voice_ctrl();
		int begin(struct voice_config *config);
		int read(void);
		int process(void);
		void check(void);
		void print_cfg(void);
		void print_data(void);
		const char *get_cmd(void);
		const char *get_err(void);
	private:
		struct voice_config *cfg;
		struct voice_data data;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_VOICE_CTRL)
extern voice_ctrl vc;
#endif
#endif

