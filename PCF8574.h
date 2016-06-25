// Copyright Jacob Killelea <jkillelea@protonmail.ch>
// controls the PCF8574 I2C GPIO expander chip from TI.
// code under GNU GPLV3
#ifndef PCF8574_H
#define PCF8574_H

#include <Arduino.h>
#include <Wire.h>

class PCF8574 {
  public:
    PCF8574(int address = 0x38); // default address, with A0, A1, and A2 all floating or grounded.
    void write(uint8_t pin, uint8_t state);
    void writeAll(uint8_t state);
    void writeAllLow();
    void writeAllHigh();

  private:
    uint8_t _READ_ADDRESS;
    uint8_t _WRITE_ADDRESS;
    uint8_t pin_array[8];
    uint8_t pin_byte;

    void readCallback(void);
    void writeCallback(void);
    void sendInstructionToChip(byte, byte);
    int  power(int base, int p);
    uint8_t arrayToByte(uint8_t array[]);
};

#endif
