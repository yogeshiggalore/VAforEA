#include <Arduino.h>
#include "voice_ctrl.h"

void voice_ctrl_cb_on_cmd_rx(uint8_t cmd, uint16_t value);
void voice_ctrl_cb_on_err(uint8_t err);

struct voice_config vc_cfg;

void setup()
{
	Serial.begin(115200);
	Serial.println("voice test code started");
	vc_cfg.port = SELECT_UART_PORT_TWO;
	vc_cfg.cb_on_cmd_rx = voice_ctrl_cb_on_cmd_rx;
	vc_cfg.cb_on_err = voice_ctrl_cb_on_err;

	Serial.printf("vc cfg port:%d cb_on_cmd_rx:%p, cb_on_err:%p\n", vc_cfg.port, vc_cfg.cb_on_cmd_rx, vc_cfg.cb_on_err);
	vc.begin(&vc_cfg);
	vc.print_cfg();
	Serial.printf("loop statrted\n");
}

void loop()
{
	vc.check();
}

void voice_ctrl_cb_on_cmd_rx(uint8_t cmd, uint16_t value)
{
	Serial.printf("voice cmd received cmd:%s value:%d\n", vc.get_cmd(), value);
}

void voice_ctrl_cb_on_err(uint8_t err)
{
	Serial.printf("voice cmd err:%s\n", vc.get_err());
}