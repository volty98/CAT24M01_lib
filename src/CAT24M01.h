#ifndef CAT24M01_H
#define CAT24M01_H
#include <arduino.h>
#include <Wire.h>

//CAT24M01 eeprom address size is 17bits.(---- ---- ---- ---Y XXXX XXXX XXXX XXXX)
//bit Y is eeprom address MSB bit, and this bit is used I2C LSB bit when I2C access.
//
//CAT24M01 I2C address is 1010 [A2] [A1] [a16] (a16 is eeprom MSB address bit)
//101000Y : 0x50, 0x51
//101001Y : 0x52, 0x53
//101010Y : 0x54, 0x55
//101011Y : 0x56, 0x57

class CAT24M01
{
public:
    enum eeprom_address {
        ADDR_DEFAULT = 0x50,
        ADDR_LL = 0x50,
        ADDR_LH = 0x52,
        ADDR_HL = 0x54,
        ADDR_HH = 0x56,
        };

    /*
    * \brief Constructor and initialize parameters.
    * \param twowire TwoWire instance address.
    * \param eeprom_base_address I2C device address of the CAT24M01. (default:0x50 = ADDR_DEFAULT)
    */
    CAT24M01(TwoWire* twowire, uint8_t eeprom_base_address);

    ~CAT24M01();

    /*
    * \brief 1 byte read from EEPROM.
    * \param addr EEPROM address.(0x00000000～0x0001FFFF)
    */
	char read(uint32_t addr);

    /*
    * \brief 1 byte write to EEPROM.
    * \param addr EEPROM address.(0x00000000～0x0001FFFF)
    * \param value write value.
    */
	void write(uint32_t addr, uint8_t value);
private:
    uint8_t base_addr;// I2C device address
    TwoWire* tw;
};
#endif
