/*
  Title : CAT24M01 I2C command library
  Copyright : Copyright (c) 2021 Shingo FUKUYA
  URL : https://github.com/volty98/CAT24M01_lib/
  License : Released under the MIT license
*/
#include "CAT24M01.h"
#include <arduino.h>
#include <Wire.h>

CAT24M01::CAT24M01()
{
    return;
}
CAT24M01::~CAT24M01()
{
    return;
}
void CAT24M01::init(uint8_t adr)
{
	this->base_addr = adr;
	Wire.begin();
	return;
}
char CAT24M01::read(uint32_t addr)
{
	uint16_t eep_addr = (uint16_t)(addr & 0x0000FFFF);
	uint8_t a16 = (addr >> 16) & 0x01;
	char rd;
	
	//set address
	Wire.beginTransmission((this->base_addr) | a16);
	Wire.write((eep_addr >> 8) & 0xFF);
	Wire.write(eep_addr & 0xFF);
	Wire.endTransmission(false);
	//read 1 byte
	Wire.requestFrom((this->base_addr) | a16, 1, false);
	while(Wire.available())
	{
		rd = Wire.read();
	}
	Wire.endTransmission();
	return rd;
}
void CAT24M01::write(uint32_t addr, uint8_t value)
{
	uint16_t eep_addr = (uint16_t)(addr & 0x0000FFFF);
	uint8_t a16 = (addr >> 16) & 0x01;
	
	//set address
	Wire.beginTransmission((this->base_addr) | a16);
	Wire.write((eep_addr >> 8) & 0xFF);
	Wire.write(eep_addr & 0xFF);
	//write 1 byte
	Wire.write(value);
	Wire.endTransmission();
	return;
}
