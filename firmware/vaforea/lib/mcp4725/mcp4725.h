#pragma once


#include "Wire.h"
#include "Arduino.h"

#define MCP4725_VERSION (F("0.4.0"))

//  CONSTANTS
#define MCP4725_MAXVALUE 4095
//  registerMode
#define MCP4725_DAC 0x40
#define MCP4725_DACEEPROM 0x60

//  page 22
#define MCP4725_GC_RESET 0x06
#define MCP4725_GC_WAKEUP 0x09
#define MCP4725_MIDPOINT 2048

enum MCP4725_ERR
{
	//  ERRORS
	MCP4725_ERR_NONE = 0,
	MCP4725_ERR_VALUE = -999,
	MCP4725_ERR_REG = -998,
	MCP4725_ERR_NOT_CON = -997,
};

enum MCP4725_PD_MODE
{
	//  powerDown Mode - TODO ENUM?
	MCP4725_PDMODE_NORMAL = 0x00,
	MCP4725_PDMODE_1K = 0x01,
	MCP4725_PDMODE_100K = 0x02,
	MCP4725_PDMODE_500K = 0x03,
};

struct mcp4725_config 
{
	uint8_t addr;
	float max_volt;
	uint8_t pwr_dwn_mode;
	uint32_t last_wrt_eeprom;
};

struct mcp4725_data 
{
	uint16_t last_val;
	float val;
};

class mcp4725
{
public:
	//  address = 0x60..0x67
	explicit mcp4725(TwoWire *wire = &Wire);

	bool begin( struct mcp4725_config *mcp_cfg, struct mcp4725_data *mcp_data );
	bool isConnected();
	uint8_t getAddress();

	//  uses writeFastMode
	int setValue(const uint16_t value = 0);
	//  returns last value set - cached - much faster than readDAC();
	uint16_t getValue();

	//  0..100.0% - input checked.
	//  will set the closest integer value in range 0..4095
	int setPercentage(float percentage = 0);
	//  due to rounding the returned value can differ slightly.
	float getPercentage();

	//  typical and default value = 5.0 V
	void setMaxVoltage(float v = 5.0);
	float getMaxVoltage();
	//  will set 0..5V to the closest integer value in range 0..4095
	int setVoltage(float v);
	// returns (approximation of) the set voltage. Assumes linear mapping.
	float getVoltage();

	//  unfortunately it is not possible to write a different value
	//  to the DAC and EEPROM simultaneously or write EEPROM only.
	int writeDAC(const uint16_t value, const bool EEPROM = false);
	//  ready checks if the last write to EEPROM has been written.
	//  until ready all writes to the mcp4725 are ignored!
	bool ready();
	uint16_t readDAC();
	uint16_t readEEPROM();
	uint32_t getLastWriteEEPROM(); //  returns timestamp

	//  experimental
	int writePowerDownMode(const uint8_t PDM, const bool EEPROM = false);
	uint8_t readPowerDownModeEEPROM();
	uint8_t readPowerDownModeDAC();
	int powerOnReset();
	int powerOnWakeUp();

private:
	struct mcp4725_config *cfg;
	struct mcp4725_data *data;

	int _writeFastMode(const uint16_t value);

	int _writeRegisterMode(const uint16_t value, uint8_t reg);
	uint8_t _readRegister(uint8_t *buffer, const uint8_t length);

	int _generalCall(const uint8_t gc);

	TwoWire *_wire;
};

//  -- END OF FILE --
