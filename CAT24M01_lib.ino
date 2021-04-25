/*
  Title : CAT24M01 library sample code
  Copyright : Copyright (c) 2021 Crane-elec. Co., LTD.
  URL : https://github.com/volty98/CAT24M01_lib/
  License : Released under the MIT license
*/
#include <Wire.h>

/* for Arduino UNO */
#if defined(__AVR_ATmega328P__)
#define LED0 (13)
#endif

//-----------------------------------------
//PLCD1602 functions
#define PLCD_ADDR (0x3E)

void plcd_cmd(char d)
{
  Wire.beginTransmission(PLCD_ADDR);
  Wire.write(0x00);
  Wire.write(d);
  Wire.endTransmission();
}
void plcd_data(char d)
{
  Wire.beginTransmission(PLCD_ADDR);
  Wire.write(0x40);
  Wire.write(d);
  Wire.endTransmission();
}
void plcd_cgram(char adr, char *d, int len)
{
  Wire.beginTransmission(PLCD_ADDR);
  Wire.write(0x00);
  Wire.write(0x40 | (adr * 8));
  Wire.endTransmission();
  for (int i = 0; i < len - 1; i++)
  {
    Wire.beginTransmission(PLCD_ADDR);
    Wire.write(0xc0);
    Wire.write(*d);
    Wire.endTransmission();
    d++;
  }
  Wire.beginTransmission(PLCD_ADDR);
  Wire.write(0x40);
  Wire.write(*d);
  Wire.endTransmission();
}
//-----------------------------------------

void setup() {
  pinMode(LED0, OUTPUT);
  Wire.begin();
  Wire.setClock(100000L);

  //PLCD init.
  delay(40);
  plcd_cmd(0x38);
  plcd_cmd(0x39);
  plcd_cmd(0x04);// EntryModeSet
  plcd_cmd(0x14);
  plcd_cmd(0x70);
  plcd_cmd(0x56);
  plcd_cmd(0x6c);
  delay(200);
  plcd_cmd(0x38);
  plcd_cmd(0x0c);
  plcd_cmd(0x01);
  delay(3);//Clear Display interval

  //setup CGRAM
  char cg0[8] = {0x04, 0x04, 0x04, 0x1f, 0x00, 0x0e, 0x00, 0x04};
  char cg1[8] = {0x1f, 0x1f, 0x1f, 0x1f, 0x11, 0x11, 0x11, 0x1f};
  char cg2[8] = {0x1f, 0x11, 0x11, 0x11, 0x1f, 0x1f, 0x1f, 0x1f};
  char cg3[8] = {0x04, 0x04, 0x15, 0x0e, 0x04, 0x00, 0x1f, 0x1f};
  char cg4[8] = {0x1f, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f};
  char cg5[8] = {0x1e, 0x02, 0x02, 0x03, 0x03, 0x02, 0x02, 0x1e};
  char cg6[8] = {0x1f, 0x11, 0x0a, 0x04, 0x04, 0x04, 0x04, 0x04};
  char cg7[8] = {0x01, 0x01, 0x05, 0x05, 0x15, 0x15, 0x00, 0x1f};
  plcd_cgram(0, cg0, 8);
  plcd_cgram(1, cg1, 8);
  plcd_cgram(2, cg2, 8);
  plcd_cgram(3, cg3, 8);
  plcd_cgram(4, cg4, 8);
  plcd_cgram(5, cg5, 8);
  plcd_cgram(6, cg6, 8);
  plcd_cgram(7, cg7, 8);

  //
  plcd_cmd(0x80 | 0); //Set Cursor (0,0)
  for (int i = 0; i < 8; i++) {
    plcd_data(i);
    plcd_data(' ');
  }
  //
  plcd_cmd(0x80 | 0x40);//Set Cursor (0,1)
  for (int i = 0; i < 8; i++) {
    plcd_data('0' + i);
    plcd_data('-');
  }

}

void loop()
{
  digitalWrite(LED0, HIGH);
  delay(100);
  digitalWrite(LED0, LOW);
  delay(100);

  for (int lp = 0; lp < 256; lp++)
  {
    plcd_cmd(0x80 | 0x4f);//Set Cursor (15,1)
    plcd_data(lp);
    delay(100);
  }
}
