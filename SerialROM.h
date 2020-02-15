// Controls proper inclusion
//#ifndef TowWire_h
#include <Wire.h>
//#endif

#ifndef _Serial_ROM_
#define _Serial_ROM_

#if (ARDUINO >=100) // to ensure Arduio macros are included
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class SerialROM  {
  public: // define to make method available
  
    // Constructor 
    SerialROM( uint8_t I2C_Address, bool displayMsg=false);

    // Methods
    void begin(uint16_t baudRate);
    void write(uint16_t start_address, uint8_t *data, uint16_t dataSize);
    void  read(uint16_t start_address, uint8_t *data, uint16_t dataSize);

  private:  // define to hide these methods
    uint8_t _I2C_addr;
    bool _msg;
};
#endif
