/*
 * file OutputVoltage.ino
 *
 * @ https://github.com/DFRobot/DFRobot_MCP4725
 *
 * connect MCP4725 I2C interface with your board (please reference board compatibility)
 *
 * Output a constant voltage value and print through the serial port.
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V0.1
 * date  2018-1-15
 */
#include "DFRobot_MCP4725.h"
#include <TimerOne.h>
#define  REF_VOLTAGE    5000

DFRobot_MCP4725 DAC;

uint16_t OUTPUT_VOLTAGE = 4000;        // unit : mV 
const PROGMEM uint16_t DACLookup_FullSine_5Bit[32] =
{
  2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
  4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
  2048, 1648, 1264,  910,  600,  345,  156,   39,
     0,   39,  156,  345,  600,  910, 1264, 1648
};
int i =0;
void setup(void) {
  Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  Serial.begin(115200);
  /* MCP4725A0_address is 0x60 or 0x61  
   * MCP4725A0_IIC_Address0 -->0x60
   * MCP4725A0_IIC_Address1 -->0x61
   */
  DAC.init(MCP4725A0_IIC_Address1, REF_VOLTAGE);
}

void loop(void) {
/*
  Serial.print("DFRobot_MCP4725 output: ");
  Serial.print(OUTPUT_VOLTAGE); 
  Serial.println(" mV");
  
  DAC.outputVoltage(OUTPUT_VOLTAGE);
  */

  delay(200);
}
void timerIsr()
{
    i++
    DAC.outputVoltage(pgm_read_word(&(DACLookup_FullSine_8Bit[i])));
    if(i >= 31){
        i=0;
    }
}