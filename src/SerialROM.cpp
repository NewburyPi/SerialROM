/*
 * main program needs to include the wire and Serial libraries?
 */

#include "SerialROM.h"  // call to the include file

SerialROM::SerialROM(int8_t I2C_Address, bool displayMsg) { // Constructor
  _I2C_addr = I2C_Address;  // capture the I2C address
  _msg = displayMsg;
}

void SerialROM::begin(uint16_t baudRate) {// this is our 'begin' public method
  Serial.begin(baudRate);
  if (_msg) {
    Serial.println("");
    Serial.println("SerialROM constructor instantiated successfully.");
  }
}

void SerialROM::write(uint16_t start_address, uint8_t *data, uint16_t dataSize){   // Serial EEPROM write method
  uint8_t addr = _I2C_addr;               // recover I2C address, saved by constructer 
  uint16_t numberOfBytes = dataSize;  // calculate the total number of bytes to send
  uint16_t byteIndex = 0;                 // index to byte in data, to be sent out
  uint16_t bytesLeft;                 // bytes to send within a page block
  Wire.begin();
//  Serial.print(" occurs just after Wire.begin");
//  Serial.print(" bytesLeft = ");
//  Serial.print( bytesLeft);
//  Serial.print(" start address = ");
//  Serial.print(start_address);
//  Serial.print(" numberOfBytes = ");
//  Serial.print(numberOfBytes);
//  Serial.print(" byteIndex = ");
//  Serial.println(byteIndex);
  while( numberOfBytes > 0) {             // There are still bytes to send
    Wire.beginTransmission(addr);         // input EEPROM I2C address
    Wire.write((uint16_t)((start_address+byteIndex) >> 8));   // MSB of internal address
    Wire.write((uint16_t)((start_address+byteIndex) & 0xFF)); // LSB of internal address
    Wire.write(data[byteIndex]);          // load the first byte in the wire queue 
//    Serial.println("address and first byte queued up");    
//    Serial.print(" bytesLeft = na");
//    Serial.print(" - byteIndex = ");
//    Serial.print(byteIndex);
//    Serial.print(" - numberOfBytes = ");
//    Serial.print(numberOfBytes);
//    Serial.print(" - First data byte = ");
//    Serial.println(data[byteIndex]);
    numberOfBytes--;                      // one less byte to send
    byteIndex++;
//    Serial.print(" byteIndex = ");
//    Serial.print(byteIndex);
//    Serial.print(" - numberOfBytes = ");
//    Serial.print(numberOfBytes);
    bytesLeft = min( 29, numberOfBytes);   // move no more than a wire buffer size (32 bytes)
//    Serial.print(" bytesLeft = ");
//    Serial.println(bytesLeft);
         
    while (bytesLeft > 0 && byteIndex%64 ) { // byte is inside of a 64 byte block or <30 bytes pushed
      Wire.write(data[byteIndex]);         // pop byte off to wire buffer
//      Serial.print(" bytesLeft = ");
//      Serial.print(bytesLeft);
//      Serial.print(" - byteIndex = ");
//      Serial.print(byteIndex);
//      Serial.print(" - numberOfBytes = ");
//      Serial.print(numberOfBytes);
//      Serial.print(" - another data byte = ");
//      Serial.println(data[byteIndex]);
      numberOfBytes--;     // one less byte to send
      bytesLeft--;
      byteIndex++;         // index to next memory address
    }
//    Serial.println("Start endTransmission");
    Wire.endTransmission();      // send thats group of bytes to ROM
//    Serial.println("Wait a bit");
    delay(6);                     // wait for ROM to complete page write
//    Serial.println("endTransmission complete");
  }
}

void SerialROM::read(uint16_t start_address, uint8_t *data, uint16_t dataSize) {   // Serial EEPROM read method

  uint8_t addr = _I2C_addr;               // recover I2C address, saved by constructer 
  int16_t numberOfBytes = dataSize;  // calculate the total number of bytes to send
  uint16_t byteIndex = 0;                 // index to byte in data, to be sent out
  uint16_t bytesLeft;                      // numbmer of bytes left for this xfer

  Wire.begin();
//  Serial.println("");
//  Serial.println("");
//  Serial.println("Start of rom read method");
//  Serial.println("");
//  Serial.println("");
//  Serial.println("");
//  Serial.print("start address = ");
//  Serial.println(start_address);
//  Serial.print("numberOfBytes = ");
//  Serial.println(numberOfBytes);
//  Serial.print("byteIndex = ");
//  Serial.println(byteIndex);
  while( numberOfBytes > 0) {             // There are still bytes to send
    Wire.beginTransmission(addr);         // input EEPROM I2C address
    Wire.write((uint16_t)((start_address+byteIndex) >> 8));   // MSB of internal address
    Wire.write((uint16_t)((start_address+byteIndex) & 0xFF)); // LSB of internal address
    Wire.endTransmission ();              // Push read address into the stream

    bytesLeft = min( 29, numberOfBytes);   // move no more than a wire buffer size (32 bytes)
//    Serial.println("address queued up and sent");
//    Serial.print(" byteIndex = ");
//    Serial.print(byteIndex);
//    Serial.print(" - numberOfBytes = ");
//    Serial.print(numberOfBytes);
//    Serial.print(" - First data byte = ");
//    Serial.print(data[byteIndex]);
//    Serial.print(" bytesLeft = ");
//    Serial.println(bytesLeft);

    Wire.requestFrom(addr, bytesLeft);           // ask ROM to prep for sending bytes 
    delay(6);
    data[byteIndex] = Wire.read();        // grab the first byte out of the ram
//    Serial.print("grab the first byte = ");
//    Serial.print(" byteIndex = ");
//    Serial.print(byteIndex);
//    Serial.print(" - bytesLeft = ");
//    Serial.print(bytesLeft);
//    Serial.print(" - numberOfBytes = ");
//    Serial.print(numberOfBytes);
//    Serial.print(" - first data byte = ");
//    Serial.println(data[byteIndex]);
    byteIndex++;
    numberOfBytes--;
    
    for (int i=0; i < bytesLeft-1; i++) {  // get rest of bytes
      data[byteIndex] = Wire.read();              
//      Serial.print("grab the next byte = ");
//      Serial.print(" i = ");
//      Serial.print(i);
//      Serial.print(" byteIndex = ");
//      Serial.print(byteIndex);
//      Serial.print(" - bytesLeft = ");
//      Serial.print(bytesLeft);
//      Serial.print(" - numberOfBytes = ");
//      Serial.print(numberOfBytes);
//      Serial.print(" - another data byte = ");
//      Serial.println(data[byteIndex]);
      numberOfBytes--;
      byteIndex++;             
    }
    Wire.endTransmission ();      // send thats group of bytes to ROM
    delay(6);                     // wait for ROM to complete page write ???
  }        
}


// Private method can only be called from within the class
float SerialROM::getPi() {
  return 3.1415926;
}
