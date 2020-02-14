Serial EEPROM Controller

DESCRIPTION:

This library acts to simplify the interface between a users sketch and the 24LC256 serial EEPROM chip. 

This chip supports the storage and retrival of upto 32 kBytes (256/8) of data, via I2C. This memory is arranged in pages of 64 bytes. In order to speed up writes to memory locations, the chip hardware supports a "page write" function that permits the user to serially push a full page of data to the chip. 

The Wire Library (https://www.arduino.cc/en/Reference/Wire) is the Arduino standard for I2C communications. However the implementaion of Wire allows, only 30 bytes to be transferred at a time. 

The 64 byte page write also adds complexity to writing large byte arrays to the chip, as the user needs to be careful to not "page Write" over a page boundry. Doing so will over write the wrong memory space. 

USE:

The class constructer takes the I2C address of the EEPROM chip.

       SerialROM <object>(<I2C address>);

The .begin method, is not absolutely required. It is used to allow the passing of a baud rate to the object, to support debug messages. 

       <object>.begin(baud rate);

The Serial EEPROM Controller library provides two methods .write and .read. For each the user supplies the starting memory address within the chip, a pointer to the data byte array, and the size of the array to be writen or read. 

       <object>.write( <start address>, <data array>, sizeof(<data array> );
       <object>.read(<start address>, <data array>, sizeof(<data array> );



