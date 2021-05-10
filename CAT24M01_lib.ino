/*
  Title : CAT24M01 library sample code
  Target Board : Onsemi CAT24M01 with Extension Board on Seeeduino XIAO.
  Copyright : Copyright (c) 2021 Shingo FUKUYA.
  URL : https://github.com/volty98/CAT24M01_lib/
  License : Released under the MIT license
*/

#include <Wire.h>
#include "src/CAT24M01.h"

#define USER_BTN 1 //xiao extender board user button
//#define ADDR_CAT24M01 0x50 //not working. PCF8563 I2C address is 0x51(conflict)
#define ADDR_CAT24M01 0x52 //A1 pin = HIGH

//CAT24M01 eep(&Wire, ADDR_CAT24M01);//0x52
CAT24M01 eep(&Wire, CAT24M01::ADDR_LH);

bool isI2C(uint8_t address)
{
  Wire.beginTransmission(address);
  if (Wire.endTransmission() == 0)
  {
    return true;
  }
  return false;
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(USER_BTN, INPUT_PULLUP);
  delay(4000);

  Serial.print(F("I2C Devices(0x) : "));
  Wire.begin();
  Wire.setClock(100000UL);
  //I2C spec. have reserved address!! these scanning escape it address.
  for (uint8_t ad = 0x08; ad < 0x77; ad++)
  {
    if (isI2C(ad))
    {
      Serial.print(ad, HEX);
      Serial.write(' ');
    }
  }
  Serial.write('\n');

}

void loop()
{
  static uint32_t ad = 0;
  static char c;
  digitalWrite(LED_BUILTIN, LOW);

  //read test cycle
  // a16 = 0, first half of 1Mbits. 
  Serial.print(ad, HEX);
  Serial.print(" : ");
  for (uint32_t i = 0; i < 16; i++)
  {
    c = eep.read(ad+i);
    Serial.print(c, HEX);
    Serial.print(" ");
  }
  Serial.print(" | ");
  // a16 = 1, back half of 1Mbits. 
  Serial.print(ad | 0x00010000, HEX);
  Serial.print(" : ");
  for (uint32_t i = 0; i < 16; i++)
  {
    c = eep.read((ad | 0x00010000) + i);
    Serial.print(c, HEX);
    Serial.print(" ");
  }
  Serial.println("");

  ad+=16;
  if(0x0000FFF0 < ad)
  {
    ad = 0;
  }

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);

  //write test mode
  if (digitalRead(USER_BTN) == 0)
  {
    //write test indicator LED
    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED_BUILTIN, LOW);
      delay(10);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(10);
    }
    //write test
    uint32_t add = 0;
    for(add = 0;add < 256;add++)
    {
      // a16 = 0, first half of 1Mbits. 
      eep.write(add,(uint8_t)(add & 0xFF));
      delay(5);//write cycle delay
      // a16 = 1, back half of 1Mbits. 
      eep.write(add | 0x00010000,0xFF - (uint8_t)(add));
      delay(5);//write cycle delay
    }
    
  }
}
