
#include "DFRobot_MCP4725.h"

const PROGMEM uint16_t DACLookup_FullSine_5Bit[32] =
{
  2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
  4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
  2048, 1648, 1264,  910,  600,  345,  156,   39,
     0,   39,  156,  345,  600,  910, 1264, 1648
};
void DFRobot_MCP4725::init(uint8_t addr, uint16_t vRef) 
{
  byte error;
  _IIC_addr = addr;
  _refVoltage = vRef;
  _PowerMode = MCP4725_NORMAL_MODE;
  Wire.begin();
  Wire.beginTransmission(_IIC_addr);
     
  error = Wire.endTransmission();
  while(error)
  {
     Wire.beginTransmission(_IIC_addr);
     
     error = Wire.endTransmission();
     Serial.println("ERROR! Not found I2C device address ");
     delay(500);
  }
}

void DFRobot_MCP4725::setMode(uint8_t powerMode)
{
  _PowerMode = powerMode;
  outputVoltage(_voltage);
}




void DFRobot_MCP4725::outputSin(uint16_t amp, uint16_t freq, uint16_t offset)
{
#ifdef TWBR
    uint8_t twbrback = TWBR;
    TWBR = ((F_CPU / 400000L) - 16) / 2; // Set I2C frequency to 400kHz
#endif
    float data = 0;
    for(int i=0;i<360;i++){
        data =amp*(float)sin(i*(PI/180))+offset*2;
        //data = pgm_read_word(&(DACLookup_FullSine_5Bit[i]));
        if(data <= 0){
            data=0;
        }
        data = (uint16_t)(((float)data / _refVoltage) * 4095);

        Wire.beginTransmission(_IIC_addr);
        Wire.write(MCP4725_Write_CMD);
        Wire.write((int)data / 16);
        Wire.write(((int)data % 16) << 4);
        Wire.endTransmission();
    }
#ifdef TWBR
    TWBR = twbrback;
#endif
}

void DFRobot_MCP4725::outputVoltage( uint16_t voltage)
{
#ifdef TWBR
  uint8_t twbrback = TWBR;
  TWBR = ((F_CPU / 400000L) - 16) / 2; // Set I2C frequency to 400kHz
#endif
  uint16_t data = 0;
  _voltage = voltage;
  if(_voltage > _refVoltage)
  {
     Serial.println("ERROR! The input voltage is greater than the maximum voltage!");
     return ;
  }
  else
  {
    data = (uint16_t)(((float)_voltage / _refVoltage) * 4095);
    Wire.beginTransmission(_IIC_addr);
    Wire.write(MCP4725_Write_CMD | (_PowerMode << 1));
    Wire.write(data / 16);
    Wire.write((data % 16) << 4);
    Wire.endTransmission();
  }
  #ifdef TWBR
  TWBR = twbrback;
#endif
}

void DFRobot_MCP4725::outputVoltageEEPROM( uint16_t voltage)
{
  uint16_t data = 0;
  _voltage = voltage;
  if(_voltage > _refVoltage)
  {
     Serial.print("ERROR! The input voltage is greater than the maximum voltage!");
     return ;
  }
  else
  {
    data = (uint16_t)(((float)_voltage / _refVoltage) * 4095);
  
    Wire.beginTransmission(_IIC_addr);
  
    Wire.write(MCP4725_WriteEEPROM_CMD | (_PowerMode << 1));

    Wire.write(data / 16);
    Wire.write((data % 16) << 4);
    Wire.endTransmission();
  }
}
