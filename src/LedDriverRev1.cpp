#include <brzo_i2c.h>
#include "LedDriverRev1.h"

LedDriverRev1::LedDriverRev1(){
  brzo_i2c_setup(1, 3, 1000);
  initPCA9685();
  initCol();
}

void ICACHE_RAM_ATTR LedDriverRev1::parseColors(uint8_t *data, int length){
  uint8_t regLow;
  uint8_t colorLow;
  uint8_t colorHigh;
  uint8_t dataI;
  uint8_t dataIp1;
  uint8_t dataColH;

  length <<= 1;
  brzo_i2c_start_transaction(0x40, 1000);
  for (int i = 0; i < length; i+=2) {
    dataI = data[i];
    dataIp1 = data[i+1];
    regLow = (dataIp1 >> 4)*4;
    colorLow = regLow +1;
    colorHigh = colorLow+1;

    if (col[colorLow] != dataI) {
      if (col[colorHigh] == dataIp1) {
        col[colorLow] = dataI;
        brzo_i2c_write(&col[regLow], 2, false);
      }else{
        col[colorLow] = dataI;
        col[colorHigh] = dataIp1 & 0x0F;
        brzo_i2c_write(&col[regLow], 3, false);
      }
    }else{
      dataColH = dataIp1 & 0x0F;
      col[colorHigh] = dataColH;
      colRev[0] = col[colorHigh + 1];
      colRev[1] = dataColH;
      brzo_i2c_write(colRev, 2, false);
    }
    yield();
  }
  brzo_i2c_end_transaction();
}

void ICACHE_RAM_ATTR LedDriverRev1::initPCA9685(){
  uint8_t preScale[2];
  preScale[0] = 0xFE;
  preScale[1] = 0x03;

  uint8_t mode1[2];
  mode1[0] = 0x00;
  mode1[1] = 0x21;

  uint8_t mode2[2];
  mode2[0] = 0x01;
  mode2[1] = 0x05;

  brzo_i2c_start_transaction(0x40, 200);
  brzo_i2c_write(preScale, 2, false);
  brzo_i2c_write(mode1, 2, false);
  brzo_i2c_write(mode2, 2, false);
  brzo_i2c_end_transaction();
}

void LedDriverRev1::initCol() {
  // s1
  col[0] =  0x40;
  col[3] =  0x41;

  col[4] =  0x44;
  col[7] =  0x45;

  col[8] =  0x3C;
  col[11] = 0x3D;

  col[12] = 0x38;
  col[15] = 0x39;

  // s2
  col[16] = 0x10;
  col[19] = 0x11;

  col[20] = 0x14;
  col[23] = 0x15;

  col[24] = 0x0C;
  col[27] = 0x0D;

  col[28] = 0x08;
  col[31] = 0x09;

  // s3
  col[32] = 0x20;
  col[35] = 0x21;

  col[36] = 0x24;
  col[39] = 0x25;

  col[40] = 0x1C;
  col[43] = 0x1D;

  col[44] = 0x18;
  col[47] = 0x19;

  // s4
  col[48] = 0x30;
  col[51] = 0x31;

  col[52] = 0x34;
  col[55] = 0x35;

  col[56] = 0x2C;
  col[59] = 0x2D;

  col[60] = 0x28;
  col[63] = 0x29;
}
