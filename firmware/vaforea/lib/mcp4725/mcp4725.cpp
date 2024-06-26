#include <Arduino.h>
#include "mcp4725.h"

mcp4725::mcp4725( TwoWire *wire)
{
	_wire = wire;
}

bool mcp4725::begin( struct mcp4725_config *mcp_cfg, struct mcp4725_data *mcp_data )
{
	
	cfg = mcp_cfg;
	data = mcp_data;

	data->last_val = 0;
	
	if ((cfg->addr < 0x60) || (cfg->addr > 0x67))
		return false;
	if (!isConnected())
		return false;

	data->last_val = readDAC();
	cfg->pwr_dwn_mode = readPowerDownModeDAC();
	return true;
}

bool mcp4725::isConnected()
{
	_wire->beginTransmission(cfg->addr);
	return (_wire->endTransmission() == 0);
}

uint8_t mcp4725::getAddress()
{
	return cfg->addr;
}

int mcp4725::setValue(const uint16_t value)
{
	if (value == data->last_val)
		return MCP4725_ERR_NONE;
	if (value > MCP4725_MAXVALUE)
		return MCP4725_ERR_VALUE;
	int rv = _writeFastMode(value);
	if (rv == 0)
		data->last_val = value;
	return rv;
}

uint16_t mcp4725::getValue()
{
	return data->last_val;
}

int mcp4725::setPercentage(float percentage)
{
	if ((percentage > 100) || (percentage < 0))
		return MCP4725_ERR_VALUE;
	return setValue(round(percentage * (0.01 * MCP4725_MAXVALUE)));
}

float mcp4725::getPercentage()
{
	return getValue() * (100.0 / MCP4725_MAXVALUE);
}

void mcp4725::setMaxVoltage(float v)
{
	cfg->max_volt = v;
}

float mcp4725::getMaxVoltage()
{
	return cfg->max_volt;
}

int mcp4725::setVoltage(float v)
{
	return setValue(round((v * MCP4725_MAXVALUE) / cfg->max_volt));
}

float mcp4725::getVoltage()
{
	return getValue() * (cfg->max_volt / MCP4725_MAXVALUE);
}

//  unfortunately it is not possible to write a different value
//  to the DAC and EEPROM simultaneously or write EEPROM only.
int mcp4725::writeDAC(const uint16_t value, const bool EEPROM)
{
	if (value > MCP4725_MAXVALUE)
		return MCP4725_ERR_VALUE;
	while (!ready())
		;
	int rv = _writeRegisterMode(value, EEPROM ? MCP4725_DACEEPROM : MCP4725_DAC);
	if (rv == 0)
		data->last_val = value;
	return rv;
}

//  ready checks if the last write to EEPROM has been written.
//  until ready all writes to the mcp4725 are ignored!
bool mcp4725::ready()
{
	yield();
	uint8_t buffer[1];
	_readRegister(buffer, 1);
	return ((buffer[0] & 0x80) > 0);
}

uint16_t mcp4725::readDAC()
{
	while (!ready())
		;
	uint8_t buffer[3];
	_readRegister(buffer, 3);
	uint16_t value = buffer[1];
	value = value << 4;
	value = value + (buffer[2] >> 4);
	return value;
}

uint16_t mcp4725::readEEPROM()
{
	while (!ready())
		;
	uint8_t buffer[5];
	_readRegister(buffer, 5);
	uint16_t value = buffer[3] & 0x0F;
	value = value << 8;
	value = value + buffer[4];
	return value;
}

uint32_t mcp4725::getLastWriteEEPROM()
{
	return cfg->last_wrt_eeprom;
};

//  depending on bool EEPROM the value of PDM is written to
//  (false) DAC or
//  (true) DAC & EEPROM,
int mcp4725::writePowerDownMode(const uint8_t PDM, const bool EEPROM)
{
	cfg->pwr_dwn_mode = (PDM & 0x03); // mask PDM bits only (written later low level)
	return writeDAC(data->last_val, EEPROM);
}

uint8_t mcp4725::readPowerDownModeEEPROM()
{
	while (!ready())
		;
	uint8_t buffer[4];
	_readRegister(buffer, 4);
	uint8_t value = (buffer[3] >> 5) & 0x03;
	return value;
}

uint8_t mcp4725::readPowerDownModeDAC()
{
	while (!ready())
		; //  TODO needed?
	uint8_t buffer[1];
	_readRegister(buffer, 1);
	uint8_t value = (buffer[0] >> 1) & 0x03;
	return value;
}

//  PAGE 22 - experimental
//  DAC value is reset to EEPROM value
//  need to reflect this in cached value
int mcp4725::powerOnReset()
{
	int rv = _generalCall(MCP4725_GC_RESET);
	data->last_val = readDAC(); //  update cache to actual value;
	return rv;
}

//  PAGE 22 - experimental
//  cfg->pwr_dwn_mode DAC resets to 0 -- PDM EEPROM stays same !!!
//  need to reflect this in cached value
int mcp4725::powerOnWakeUp()
{
	int rv = _generalCall(MCP4725_GC_WAKEUP);
	cfg->pwr_dwn_mode = readPowerDownModeDAC(); //  update to actual value;
	return rv;
}

//  PAGE 18 DATASHEET
int mcp4725::_writeFastMode(const uint16_t value)
{
	uint8_t l = value & 0xFF;
	uint8_t h = ((value / 256) & 0x0F); //  set C0 = C1 = 0, no PDmode
	h = h | (cfg->pwr_dwn_mode << 4);

	_wire->beginTransmission(cfg->addr);
	_wire->write(h);
	_wire->write(l);
	return _wire->endTransmission();
}

//  PAGE 19 DATASHEET
//  reg = MCP4725_DAC | MCP4725_EEPROM
int mcp4725::_writeRegisterMode(const uint16_t value, uint8_t reg)
{
	if (reg & MCP4725_DACEEPROM)
	{
		cfg->last_wrt_eeprom = millis();
	}
	uint8_t h = (value / 16);
	uint8_t l = (value & 0x0F) << 4;
	_wire->beginTransmission(cfg->addr);
	reg = reg | (cfg->pwr_dwn_mode << 1);
	_wire->write(reg);
	_wire->write(h);
	_wire->write(l);
	return _wire->endTransmission();
}

//  PAGE 20 DATASHEET
//  typical 3 or 5 bytes
uint8_t mcp4725::_readRegister(uint8_t *buffer, const uint8_t length)
{
	_wire->beginTransmission(cfg->addr);
	int rv = _wire->endTransmission();
	if (rv != 0)
		return 0; //  error

	//  readBytes will always be equal or smaller to length
	uint8_t readBytes = _wire->requestFrom(cfg->addr, length);
	uint8_t cnt = 0;
	while (cnt < readBytes)
	{
		buffer[cnt++] = _wire->read();
	}
	return readBytes;
}

//  name comes from datasheet
int mcp4725::_generalCall(const uint8_t gc)
{
	_wire->beginTransmission(0); //  cfg->addr
	_wire->write(gc);
	return _wire->endTransmission();
}

//  -- END OF FILE --
