/*******************************************************************************
 * File Name: app_vaforea.cpp
 *
 * Version: 1.0
 *
 * Description:
 * This is header file for application module for VAforEA project
 *
 *
 *
 ********************************************************************************
 * VAforEA (2024-25)
 ********************************************************************************/

#include <Arduino.h>
#include <app_vaforea.h>
#include <voice_ctrl.h>
#include <RotaryEncoder.h>
#include <mcp4725.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>
#include <EEPROM.h>

struct app_config app_cfg;
struct app_data   app_values;

RotaryEncoder re_volt( 36, 39 );
RotaryEncoder re_curr( 34, 35 );

Adafruit_ADS1115 ads;
mcp4725 mcpVolt;
mcp4725 mcpCurr;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define ADC_V_SLOPE		0
#define ADC_V_OFFSET	1
#define ADC_A_SLOPE		2
#define ADC_A_OFFSET	3

int app_begin( void )
{
	int status = 0;

	app_hardware_init();
	printf("Hardware init done\n");

	app_eeprom_init();
	printf("mem init done\n");

	app_vc_init();
	printf("voice ctrl init done\n");

	app_re_init();
	printf("rotary encode init done\n");

	app_ads_init();
	printf("ads1115 init done\n");

	app_mcp_init();
	printf("mcp4725 init done\n");

	app_display_init();
	printf("display init done\n");

	app_relay_init();
	printf("relay init done\n");

	return status;
}

int app_hardware_init(void)
{
	Serial.begin(115200);
	Wire.begin();
	pinMode( LED_BUILTIN, OUTPUT );
	digitalWrite(LED_BUILTIN, LOW);
	return 0;
}

void app_read(void)
{
	vc.check();
}

void app_process(void)
{

}

void app_write(void)
{

}

void app_vc_init(void)
{
    app_cfg.vc.port = SELECT_UART_PORT_TWO;
	app_cfg.vc.cb_on_cmd_rx = app_vc_cb_on_cmd_rx;
	app_cfg.vc.cb_on_err = app_vc_cb_on_err;

	printf("vc cfg port:%d cb_on_cmd_rx:%p, cb_on_err:%p\n", app_cfg.vc.port, app_cfg.vc.cb_on_cmd_rx, app_cfg.vc.cb_on_err);
	vc.begin(&app_cfg.vc, &app_values.vc);
}

void app_vc_cb_on_cmd_rx(uint8_t cmd, uint16_t value)
{
	uint8_t cmd_act;

	printf("voice cmd received cmd:%s value:%d\n", vc.get_cmd(), value);
	cmd_act = app_vc_process_command();
	
	if( cmd_act == VOICE_CTRL_CMD_ACTION )
	{
		printf("take action command\n");
	}
	else
	{
		printf("set voltage:%d and current:%d per:%d\n", app_values.xl.curr_voltage, app_values.xl.curr_current, app_values.xl.curr_volt_per);
	}
	
	app_values.vc_rqt = true;
}

void app_vc_cb_on_err(uint8_t err)
{
	printf("voice cmd err:%s\n", vc.get_err());
	app_values.vc_rqt = true;
}

uint8_t app_vc_process_command(void)
{
	uint8_t set_per=0;
	uint8_t ss_buf[9] = {0xAA, 0x55, VOICE_PC_ACT_CMD_SCRNSHT, 0x00, 0x00, 0xA5, 0x5A, 0x0D, 0x0A};
	uint8_t cmd = VOICE_CTRL_CMD_ERR;

	cmd = app_values.vc.cmd;

	switch (app_values.vc.cmd)
	{
	case VOICE_CTRL_CMD_NONE:
		
		break;
	
	case VOICE_CTRL_CMD_ONOFF:
	case VOICE_CTRL_CMD_SET_VOLT:
	case VOICE_CTRL_CMD_UP_VOLT:
	case VOICE_CTRL_CMD_DOWN_VOLT:
		app_mcp_set_voltage(app_values.vc.cmd, app_values.vc.value);
		break;

	case VOICE_CTRL_CMD_SET_CURR:
	case VOICE_CTRL_CMD_UP_CURR:
	case VOICE_CTRL_CMD_DOWN_CURR:
		//app_mcp_set_current(app_values.vc.cmd, app_values.vc.value);
		break;

	case VOICE_CTRL_CMD_ACTION:
		Serial.write(ss_buf, sizeof(ss_buf));
		break;

	default:

		break;
	}

	app_values.vc.cmd = VOICE_CTRL_CMD_NONE;
	app_values.vc.value = 0;

	return cmd;
}

void app_vc_take_action(void)
{

}

void app_re_init(void)
{
	re_volt.setEncoderType( EncoderType::HAS_PULLUP );
	re_volt.setBoundaries( 0, app_cfg.mem.input_voltage, true );
	re_volt.onTurned( &app_re_volt_cb_trig );
	re_volt.begin();

	re_curr.setEncoderType( EncoderType::HAS_PULLUP );
	re_curr.setBoundaries( 0, app_cfg.mem.input_current, true );
	re_curr.onTurned( &app_re_curr_cb_trig );
	re_curr.begin();
}

void app_re_volt_cb_trig(long val)
{
	uint16_t volt_diff=0;

	printf( "RE Volt value: %ld\n", val );
	app_values.reVolt.cnt = val - app_values.reVolt.prv;
	app_values.reVolt.prv = val;

	printf( "RE Volt value: %ld %d\n", val, app_values.reVolt.cnt );

	if( app_values.reVolt.cnt < 0 )
	{
		volt_diff = app_values.reVolt.cnt * 100 * -1;
		/* multiplying -1 make it positive value so < 1000 works */
		if( volt_diff < 1000 )
		{
			app_mcp_set_voltage(VOICE_CTRL_CMD_DOWN_VOLT, volt_diff);
		}
	}

	if(app_values.reVolt.cnt > 0)
	{
		volt_diff = app_values.reVolt.cnt * 100;
		if( volt_diff < 1000 )
		{
			app_mcp_set_voltage(VOICE_CTRL_CMD_UP_VOLT, volt_diff);
		}
	}
}

void app_re_curr_cb_trig(long val)
{
	uint16_t curr_diff=0;

	app_values.reCurr.cnt = app_values.reCurr.prv - val;
	app_values.reCurr.prv = val;

	printf( "RE Curr value: %ld %d\n", val, app_values.reCurr.cnt );

	if( app_values.reCurr.cnt < 0 )
	{
		curr_diff = app_values.reCurr.cnt * 100 * -1;
		/* multiplying -1 make it positive value so < 1000 works */
		if( curr_diff < 1000 )
		{
			app_mcp_set_current(VOICE_CTRL_CMD_DOWN_CURR, curr_diff);
		}
	}

	if(app_values.reCurr.cnt > 0)
	{
		curr_diff = app_values.reCurr.cnt * 100;
		if( curr_diff < 1000 )
		{
			app_mcp_set_voltage(VOICE_CTRL_CMD_UP_CURR, curr_diff);
		}
	}
}

void app_ads_init(void)
{
	ads.begin();
}

void app_ads_read(void)
{
	int16_t raw_volt;
	int16_t raw_curr;

	ads.setGain(GAIN_TWO);
    raw_volt = ads.readADC_SingleEnded(0);
    app_values.xl.meas_volt = ads.computeVolts(raw_volt) * 10;
	raw_curr = ads.readADC_SingleEnded(2);
    app_values.xl.meas_curr = ads.computeVolts(raw_curr) * 10;

    //printf("ADS volt= r:%d c:%f curr= r:%d c:%d Vper:%d\n", raw_volt, app_values.xl.meas_volt, raw_curr, app_values.xl.meas_curr, app_values.xl.curr_volt_per);
}

void app_mcp_init(void)
{
	app_cfg.mcpVolt.addr = 0x60;
	app_cfg.mcpVolt.max_volt = 5.1;
	app_cfg.mcpVolt.pwr_dwn_mode = 0;
	app_cfg.mcpVolt.last_wrt_eeprom = 0;
	mcpVolt.begin(&app_cfg.mcpVolt, &app_values.mcpVolt);

	app_cfg.mcpCurr.addr = 0x61;
	app_cfg.mcpCurr.max_volt = 5.1;
	app_cfg.mcpCurr.pwr_dwn_mode = 0;
	app_cfg.mcpCurr.last_wrt_eeprom = 0;
	mcpCurr.begin(&app_cfg.mcpCurr, &app_values.mcpCurr);

	/* turn off voltage */
	mcpVolt.setPercentage(app_cfg.mem.max_percent);
	app_values.xl.curr_volt_per = app_cfg.mem.max_percent;
	app_values.xl.curr_voltage = 0;

	/* turn off current */
	mcpCurr.setPercentage(app_cfg.mem.max_percent);
	app_values.xl.curr_curr_per = app_cfg.mem.max_percent;
	app_values.xl.curr_current = 0;

}

void app_mcp_set_voltage( uint8_t cmd, uint16_t value )
{
	bool process_flag=false;
	int8_t set_percent=0;

	switch (cmd)
	{
	case VOICE_CTRL_CMD_ONOFF:
		#ifdef USE_RELAY_CONTROL
			if( value == 0 )
			{
				app_relay_control(RELAY_OFF);
			}
			else
			{
				app_relay_control(RELAY_ON);
			}
		#else
			process_flag = true;
			if( value == 0 )
			{
				app_values.xl.voltage_backup = app_values.xl.curr_voltage;
				app_values.xl.curr_voltage = 0;
			}
			else
			{
				app_values.xl.curr_voltage = app_values.xl.voltage_backup;
			}

		#endif
		break;

	case VOICE_CTRL_CMD_SET_VOLT:
		process_flag = true;
		app_values.xl.curr_voltage = value;
		break;
	
	case VOICE_CTRL_CMD_UP_VOLT:
		process_flag = true;
		app_values.xl.curr_voltage += value;
		if( app_values.xl.curr_voltage > app_cfg.mem.input_voltage )
		{
			app_values.xl.curr_voltage = app_cfg.mem.input_voltage;
		}
		break;

	case VOICE_CTRL_CMD_DOWN_VOLT:
		process_flag = true;
		app_values.xl.curr_voltage -= value;
		if( app_values.xl.curr_voltage < 0 )
		{
			app_values.xl.curr_voltage = 0;
		}
		break;

	default:
		break;
	}

	if( process_flag == true )
	{
		set_percent = map( app_values.xl.curr_voltage, 0, app_cfg.mem.input_voltage, 0, app_cfg.mem.max_percent);
		app_values.xl.curr_volt_per = DAC_100_PERCENT - set_percent;
		if( app_values.xl.curr_volt_per < 0 )
		{
			app_values.xl.curr_volt_per = 0;
		}

		if( app_values.xl.curr_volt_per > app_cfg.mem.max_percent )
		{
			app_values.xl.curr_volt_per = app_cfg.mem.max_percent;
		}

		mcpVolt.setPercentage(app_values.xl.curr_volt_per);

		if( app_values.xl.curr_voltage != 0 )
		{
			app_values.xl.voltage_backup = app_values.xl.curr_voltage;
		}
	}

}

void app_mcp_set_current( uint8_t cmd, uint16_t value )
{
	bool process_flag=false;
	int8_t set_percent=0;

	switch (cmd)
	{

	case VOICE_CTRL_CMD_SET_CURR:
		process_flag = true;
		app_values.xl.curr_current = value;
		break;
	
	case VOICE_CTRL_CMD_UP_CURR:
		process_flag = true;
		app_values.xl.curr_current += value;
		if( app_values.xl.curr_current > app_cfg.mem.input_current )
		{
			app_values.xl.curr_current = app_cfg.mem.input_current;
		}
		break;

	case VOICE_CTRL_CMD_DOWN_CURR:
		process_flag = true;
		app_values.xl.curr_current -= value;
		if( app_values.xl.curr_current < 0 )
		{
			app_values.xl.curr_current = 0;
		}
		break;

	default:
		break;
	}

	if( process_flag == true )
	{
		set_percent = map( app_values.xl.curr_current, 0, app_cfg.mem.input_current, 0, app_cfg.mem.max_percent);
		app_values.xl.curr_curr_per = DAC_100_PERCENT - set_percent;
		if( app_values.xl.curr_curr_per < 0 )
		{
			app_values.xl.curr_curr_per = 0;
		}

		if( app_values.xl.curr_curr_per > app_cfg.mem.max_percent )
		{
			app_values.xl.curr_curr_per = app_cfg.mem.max_percent;
		}

		mcpCurr.setPercentage(app_values.xl.curr_curr_per);

		if( app_values.xl.curr_current != 0 )
		{
			app_values.xl.current_backup = app_values.xl.curr_current;
		}
	}

}

void app_display_init(void)
{
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
	{
		printf("display err\n");
	}
}

void app_display_test(void)
{
	
	display.clearDisplay();
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(7, 2);
	display.println("<VAforEA>");
	
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(0, 20);
	display.print("V:");
	display.print(app_values.xl.meas_volt, 1);
	display.setCursor(0, 45);
	display.print("C:");
	display.print(app_values.xl.meas_curr, 1);

	display.display();
}

void app_eeprom_init(void)
{
	uint8_t val_8;
	uint16_t val_16;

	EEPROM.begin(APP_MEMORY_SIZE);
	
	val_8 = EEPROM.read(0);
	val_16 = val_8;
	val_16 <<= 8;
	val_16 |= EEPROM.read(1);
	app_cfg.mem.voltage = val_16;

	val_8 = EEPROM.read(2);
	val_16 = val_8;
	val_16 <<= 8;
	val_16 |= EEPROM.read(3);
	app_cfg.mem.current = val_16;

	val_8 = EEPROM.read(4);
	val_16 = val_8;
	val_16 <<= 8;
	val_16 |= EEPROM.read(5);
	app_cfg.mem.input_voltage = val_16;

	val_8 = EEPROM.read(6);
	val_16 = val_8;
	val_16 <<= 8;
	val_16 |= EEPROM.read(7);
	app_cfg.mem.input_current = val_16;

	app_cfg.mem.max_percent = EEPROM.read(8);

	if( app_cfg.mem.voltage == 0xFFFF )
	{
		app_cfg.mem.voltage = 0;
		EEPROM.write(0,0);
		EEPROM.write(1,0);
		EEPROM.commit();
	}

	if( app_cfg.mem.current == 0xFFFF )
	{
		app_cfg.mem.current = 0;
		EEPROM.write(2,0);
		EEPROM.write(3,0);
		EEPROM.commit();
	}

	if( app_cfg.mem.input_voltage == 0xFFFF )
	{
		app_cfg.mem.input_voltage = 12000;
		EEPROM.write(4,0x2E); //set to 12V
		EEPROM.write(5,0xE0);
		EEPROM.commit();
	}

	if( app_cfg.mem.input_current == 0xFFFF )
	{
		app_cfg.mem.input_current = 4000;
		EEPROM.write(6,0xFA); //set to 4A
		EEPROM.write(7,0x00);
		EEPROM.commit();
	}

	if( app_cfg.mem.max_percent == 0xFF )
	{
		app_cfg.mem.max_percent = 100;
		EEPROM.write(8,100);
		EEPROM.commit();
	}

	printf("mem voltage:%d current:%d in_volt:%d in_curr:%d per:%d\n",app_cfg.mem.voltage, app_cfg.mem.current, app_cfg.mem.input_voltage, app_cfg.mem.input_current, app_cfg.mem.max_percent );
}

void app_relay_init(void)
{
	pinMode( RELAY_PIN, OUTPUT );
	#ifdef USE_RELAY_CONTROL
		digitalWrite( RELAY_PIN, RELAY_OFF );
		app_values.xl.relay_status = RELAY_OFF;
	#else
		digitalWrite( RELAY_PIN, RELAY_ON );
		app_values.xl.relay_status = RELAY_UNKNW;
	#endif
}

void app_relay_control(bool val)
{
	#ifdef USE_RELAY_CONTROL
		if( val == RELAY_ON )
		{
			digitalWrite( RELAY_PIN, RELAY_ON );
			app_values.xl.relay_status = RELAY_ON;
		}
		else
		{
			digitalWrite( RELAY_PIN, RELAY_OFF );
			app_values.xl.relay_status = RELAY_OFF;
		}
	#else
		app_values.xl.relay_status = RELAY_UNKNW;
	#endif
}

void app_test_dac_with_percent( void )
{
	
	app_values.xl.curr_volt_per += 1;
	if( app_values.xl.curr_volt_per > 100 )
	{
		app_values.xl.curr_volt_per = 0;
	}

	mcpVolt.setPercentage((100-app_values.xl.curr_volt_per));
}

void app_pc_act_send(void)
{
	uint8_t send_buf[9] = {0xAA, 0x55, 0x00, 0x00, 0x00, 0xA5, 0x5A, 0x0D, 0x0A};

	/* send voltage data */
	send_buf[VOICE_CTRL_BYTE_IDX_CC] = VOICE_PC_ACT_CMD_VLTRED;
	send_buf[VOICE_CTRL_BYTE_IDX_D1] = app_values.xl.curr_voltage >> 8;
	send_buf[VOICE_CTRL_BYTE_IDX_D2] = app_values.xl.curr_voltage;
	Serial.write(send_buf, sizeof(send_buf));

	/* send current data */
	//send_buf[VOICE_CTRL_BYTE_IDX_CC] = VOICE_PC_ACT_CMD_CURRED;
	//send_buf[VOICE_CTRL_BYTE_IDX_D1] = app_values.xl.curr_current >> 8;
	//send_buf[VOICE_CTRL_BYTE_IDX_D2] = app_values.xl.curr_current;
	//Serial.write(send_buf, sizeof(send_buf));

	/* send relay data */
	//send_buf[VOICE_CTRL_BYTE_IDX_CC] = VOICE_PC_ACT_CMD_RLYSTS;
	//send_buf[VOICE_CTRL_BYTE_IDX_D1] = 0;
	//send_buf[VOICE_CTRL_BYTE_IDX_D2] = app_values.xl.relay_status;
	//Serial.write(send_buf, sizeof(send_buf));

}

void app_scan_i2c_devices(void)
{
	uint8_t *dataptr;
	uint8_t I2CAddr;
	uint8_t status;
	uint8_t numberOfbytes;

	printf("started scanning i2c devices\n\n");
	printf("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n00:    ");
	for (I2CAddr = 1; I2CAddr <= MAX_I2C_ADDRESS; I2CAddr++)
	{
		if ((I2CAddr % 0x10) == 0)
		{
			printf("\n%2X: ", I2CAddr);
		}

		Wire.beginTransmission(I2CAddr);
    	status = Wire.endTransmission();
		if (status == 0)
		{
			printf("%2X ", I2CAddr);
		}
		else if( status == 4 )
		{
			printf("XX ");
		}
		else
		{
			printf("-- ");
		}

		/* 100 ms sleep */
		delay(100);
	}
	printf("\n\nstopped scanning i2c devices\n");
}
