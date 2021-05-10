/*
  Title : CAT24M01 I2C command library
  Copyright : Copyright (c) 2021 Shingo FUKUYA
  URL : https://github.com/volty98/CAT24M01_lib/
  License : Released under the MIT license
*/
#include "CAT24M01.h"
#include <arduino.h>
#include <Wire.h>

CAT24M01::CAT24M01(TwoWire* twowire, uint8_t eeprom_base_address)
{
	this->base_addr = eeprom_base_address;
	tw = twowire;
	tw->begin();
}
CAT24M01::~CAT24M01()
{
}
char CAT24M01::read(uint32_t addr)
{
	uint16_t eep_addr = (uint16_t)(addr & 0x0000FFFF);
	uint8_t a16 = (addr >> 16) & 0x01;
	char rd = 0;
	
	//set address
	tw->beginTransmission((this->base_addr) | a16);
	tw->write((eep_addr >> 8) & 0xFF);
	tw->write(eep_addr & 0xFF);
	tw->endTransmission(false);
	//read 1 byte
	tw->requestFrom((this->base_addr) | a16, 1, false);
	while(tw->available())
	{
		rd = tw->read();
	}
	tw->endTransmission();
	return rd;
}
void CAT24M01::write(uint32_t addr, uint8_t value)
{
	uint16_t eep_addr = (uint16_t)(addr & 0x0000FFFF);
	uint8_t a16 = (addr >> 16) & 0x01;
	
	//set address
	tw->beginTransmission((this->base_addr) | a16);
	tw->write((eep_addr >> 8) & 0xFF);
	tw->write(eep_addr & 0xFF);
	//write 1 byte
	tw->write(value);
	tw->endTransmission();
	return;
}
