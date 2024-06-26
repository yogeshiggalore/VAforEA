#include <ads1115.h>

static uint8_t currentPGA = 255;

ads1115::ads1115()
{

}

ads1115::~ads1115() {}

int ads1115::begin( struct ads1115_config *ads_cfg, struct ads1115_data *ads_data)
{
	cfg = ads_cfg;
	data = ads_data;
	
	reset();
	setDeviceMode(cfg->run_mode);
	setDataRate(cfg->data_rate);
	setPga(cfg->pga);

	return 0;
}

void ads1115::read_channels(void)
{
	uint8_t mask=ADS1115_READ_CHANNEL_0;
	uint8_t input=ADS1115_MUX_AIN0_AIN1;

	if( cfg->cfg_mode == ADS1115_CFG_MODE_SINGLE )
	{
		input = ADS1115_MUX_AIN0_GND;
	}

	for( int cntr=0; cntr<4; cntr++ )
	{
		if( cfg->read_channel & mask )
		{
			setMultiplexer(input);
			startSingleConvertion();

			delayMicroseconds(250); // The ADS1115 needs to wake up from sleep mode and usually it takes 25 uS to do that

			if(getOperationalStatus() != 0)
			{
				data->val_ads[cntr] = readConvertedValue();
			}
			else
			{
				data->val_ads[cntr] = 0xFFFF; 
			}
		}
		mask <<= 1;
		input++;
	}
}

/**
 * Public methods
 */

void ads1115::reset() {
	writeRegister(ADS1115_CONFIG_REG_ADDR, ADS1115_CONFIG_REG_DEF);
	writeRegister(ADS1115_LOW_TRESH_REG_ADDR, ADS1115_LOW_TRESH_REG_DEF);
	writeRegister(ADS1115_HIGH_TRESH_REG_ADDR, ADS1115_HIGH_TRESH_REG_DEF);
}

reg_val_t ads1115::readRegister(reg_addr_t dataAddress) {
	Wire.beginTransmission(cfg->addr);
	Wire.write(dataAddress);
	Wire.endTransmission();
	Wire.requestFrom(cfg->addr, (uint8_t) 2);

	if (Wire.available()) {
		uint8_t firstByte = Wire.read();
		uint8_t secondByte = Wire.read();

		return (firstByte << 8) + secondByte;
	}

	return -1;
}

void ads1115::writeRegister(reg_addr_t dataAddress, uint16_t data) {
	uint8_t hByte = data >> 8;
	uint8_t lByte = data & 255;

	Wire.beginTransmission(cfg->addr);
	Wire.write(dataAddress);
	Wire.write(hByte);
	Wire.write(lByte);
	Wire.endTransmission();
}

reg_val_t ads1115::readRawValue() {
	return readRegister(ADS1115_CONVERSION_REG_ADDR);
}

float ads1115::readConvertedValue() {
	float rawValue = (float) readRawValue();

	if (currentPGA == 255) {
		currentPGA = getPga();
	}

	float multiplier;

	switch (currentPGA) {
		case ADS1115_PGA_0_256:
			multiplier = ADS1115_PGA_0_256_MULT;
			break;
		case ADS1115_PGA_0_512:
			multiplier = ADS1115_PGA_0_512_MULT;
			break;
		case ADS1115_PGA_1_024:
			multiplier = ADS1115_PGA_1_024_MULT;
			break;
		case ADS1115_PGA_2_048:
			multiplier = ADS1115_PGA_2_048_MULT;
			break;
		case ADS1115_PGA_4_096:
			multiplier = ADS1115_PGA_4_096_MULT;
			break;
		case ADS1115_PGA_6_144:
			multiplier = ADS1115_PGA_6_144_MULT;
			break;
		default:
			multiplier = 1.0;
			break;
	}

	return multiplier * rawValue;
}

void ads1115::startSingleConvertion() {
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_OS_FLAG_POS, 1);
}

uint8_t ads1115::getOperationalStatus() {
	return readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_OS_FLAG_POS);
}

void ads1115::setMultiplexer(uint8_t mux) {
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_MUX2_DAT_POS, getFlag(mux, 2));
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_MUX1_DAT_POS, getFlag(mux, 1));
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_MUX0_DAT_POS, getFlag(mux, 0));
}

uint8_t ads1115::getMultiplexer() {
	uint8_t mux = readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_MUX0_DAT_POS);

	mux |= readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_MUX1_DAT_POS) << 1;
	mux |= readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_MUX2_DAT_POS) << 2;

	return mux;
}

void ads1115::setPga(uint8_t pga) {
	currentPGA = pga;

	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_PGA2_DAT_POS, getFlag(pga, 2));
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_PGA1_DAT_POS, getFlag(pga, 1));
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_PGA0_DAT_POS, getFlag(pga, 0));
}

uint8_t ads1115::getPga() {
	uint8_t pga = readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_PGA0_DAT_POS);

	pga |= readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_PGA1_DAT_POS) << 1;
	pga |= readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_PGA2_DAT_POS) << 2;

	return pga;
}

void ads1115::setDeviceMode(uint8_t mode) {
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_MODE_FLAG_POS, mode);
}

uint8_t ads1115::getDeviceMode() {
	return readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_MODE_FLAG_POS);
}

void ads1115::setDataRate(uint8_t dataRate) {
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_DR2_DAT_POS, getFlag(dataRate, 2));
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_DR1_DAT_POS, getFlag(dataRate, 1));
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_DR0_DAT_POS, getFlag(dataRate, 0));
}

uint8_t ads1115::getDataRate() {
	uint8_t dataRate = readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_DR0_DAT_POS);

	dataRate |= readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_DR1_DAT_POS) << 1;
	dataRate |= readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_DR2_DAT_POS) << 2;

	return dataRate;
}

void ads1115::setComparatorMode(uint8_t mode) {
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_MODE_FLAG_POS, mode);
}

uint8_t ads1115::getComparatorMode() {
	return readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_MODE_FLAG_POS);
}

void ads1115::setComparatorPolarity(uint8_t polarity) {
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_POL_FLAG_POS, polarity);
}

uint8_t ads1115::getComparatorPolarity() {
	return readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_POL_FLAG_POS);
}

void ads1115::setLatching(bool latching) {
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_LAT_FLAG_POS, latching);
}

bool ads1115::isLatching() {
	return readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_LAT_FLAG_POS);
}

void ads1115::setComparatorQueue(uint8_t queue) {
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_QUE1_DAT_POS, getFlag(queue, 1));
	writeFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_QUE0_DAT_POS, getFlag(queue, 0));
}

uint8_t ads1115::getComparatorQueue() {
	uint8_t queue = readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_QUE0_DAT_POS);

	queue |= readFlag(ADS1115_CONFIG_REG_ADDR, ADS1115_COMP_QUE1_DAT_POS) << 1;

	return queue;
}

void ads1115::setLowTreshold(reg_val_t lowTreshold) {
	writeRegister(ADS1115_LOW_TRESH_REG_ADDR, lowTreshold);
}

reg_val_t ads1115::getLowTreshold() {
	return readRegister(ADS1115_LOW_TRESH_REG_ADDR);
}

void ads1115::setHighTreshold(reg_val_t highTreshold) {
	writeRegister(ADS1115_HIGH_TRESH_REG_ADDR, highTreshold);
}

reg_val_t ads1115::getHighTreshold() {
	return readRegister(ADS1115_HIGH_TRESH_REG_ADDR);
}

/**
 * Private methods
 */

flag_val_t ads1115::readFlag(reg_addr_t dataAddress, flag_pos_t flagPos) {
	reg_val_t registerValue = readRegister(dataAddress);

	return getFlag(registerValue, flagPos);
}

void ads1115::writeFlag(reg_addr_t dataAddress, flag_pos_t flagPos, flag_val_t flagValue) {
	reg_val_t registerValue = readRegister(dataAddress);
	registerValue = setFlag(registerValue, flagPos, flagValue);
	writeRegister(dataAddress, registerValue);
}

flag_val_t ads1115::getFlag(reg_val_t registerValue, flag_pos_t flagPos) {
	return getFlag(registerValue, flagPos, 1);
}

flag_val_t ads1115::getFlag(reg_val_t registerValue, flag_pos_t flagPos, flag_val_t customVal) {
	return ((registerValue >> flagPos) & 1) == 1 ? customVal : 0;
}

reg_val_t ads1115::setFlag(reg_val_t registerValue, flag_pos_t flagPos, flag_val_t flagVal) {
	if (flagVal == 1) {
		return registerValue | (1 << flagPos);
	}

	return registerValue & (~(1 << flagPos));
}