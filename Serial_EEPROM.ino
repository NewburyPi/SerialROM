// Library inclusions

#include <Wire.h>       // Include the I2C Wire Library
#include "SerialROM.h"  // include Serial ROM handler library

// EEPROM I2C Address
#define i2c_eeprom 0x50

SerialROM mem(i2c_eeprom);

// Test parameters
#define DATA_BLOCK_SIZE 200
#define START_ADDRESS 0

uint8_t dataToBeWriten[DATA_BLOCK_SIZE];
uint8_t dataReadBack[DATA_BLOCK_SIZE];

void setup() {
  Serial.begin(9600);
  mem.begin(9600);
  randomSeed(65);
}

void loop() {
  for(uint16_t i = 0; i < DATA_BLOCK_SIZE; i++){  // create an array of random number test data
    dataToBeWriten[i] = random(255);      
  }
  Serial.println("Write data prepped\n");

  for(uint16_t i = 0; i < DATA_BLOCK_SIZE; i++){  // create an array of random number test data
    if ((i)%8 == 0) {
      Serial.print(i+1+START_ADDRESS);
      Serial.print(" - ");
    }
    Serial.print(dataToBeWriten[i]);
    Serial.print(",");
    if ((i+1)%8 == 0) {
      Serial.print(" - ");
      Serial.println(i+1+START_ADDRESS);
    }
  }
 
  mem.write(START_ADDRESS, dataToBeWriten, sizeof(dataToBeWriten));  // write test data into the EEPROM
  Serial.println("\n\nTest data has been writen to the EEPROM\n");

//  ****  UNCOMMENT BELOW TO VERIFY ERROR CAPTURE  ****
//  for(uint16_t i = 0; i < 5; i++){  // create an array of random number test data
//    dataToBeWriten[i] = random(62);      
//  }
//  ****  UNCOMMENT ABOVE TO VERIFY ERROR CAPTURE  ****


  mem.read(START_ADDRESS, dataReadBack, sizeof(dataToBeWriten));  // write test data into the EEPROM
  Serial.print("Test data has been read back from the EEPROM\n\n");

  Serial.println("\n Data as read.\n");
  
  for(uint16_t i = 0; i < DATA_BLOCK_SIZE; i++){  // create an array of random number test data
    if ((i)%8 == 0) {
      Serial.print(i+1+START_ADDRESS);
      Serial.print(" - ");
    }
    Serial.print(dataReadBack[i]);
    Serial.print(",");
    if ((i+1)%8 == 0) {
      Serial.print(" - ");
      Serial.println(i+1+START_ADDRESS);
    }
  }
  Serial.println("");

  
  uint8_t faults = 0;      // Check for and display errors
  for (uint8_t i = 0; i < DATA_BLOCK_SIZE; i++){    
    if (dataToBeWriten[i] != dataReadBack[i]) {
      faults++;
      Serial.print("   Error found at index ");
      Serial.print(i);
      Serial.print(" - Data writen: ");
      Serial.print(dataToBeWriten[i]);
      Serial.print(" - Data read: ");
      Serial.println(dataReadBack[i]);
    }
  }
  Serial.println("");
  if (faults) {
    Serial.print(faults);
  } else {
    Serial.print("No");
  }
  Serial.println(" Faults");
  
  while(1);   // sit around and wait :)
}
