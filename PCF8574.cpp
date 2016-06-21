// Copyright Jacob Killelea <jkillelea@protonmail.ch>
// controls the PCF8574 I2C GPIO expander chip from TI.
// code under GNU GPLV3

#include <Arduino.h>
#include <Wire.h>

#include "PCF8574.h"

// PRIVATE
uint8_t _READ_ADDRESS  = 0x39; // defaults
uint8_t _WRITE_ADDRESS = 0x38;
uint8_t pin_array[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; // initial pin state
uint8_t pin_byte       = 0x00;

const byte ALL_HIGH = byte(B11111111);  // all high on the GPIOs, can't sink current.
const byte ALL_LOW  = byte(B00000000);  // all low, sinks current on all of them.

// PUBLIC
void PCF8574::writeAllHigh() {
  for (uint8_t i = 0; i < 8; i++) {
    pin_array[i] = 1;
  }
  writeCallback();
}

void PCF8574::writeAllLow() {
  for (uint8_t i = 0; i < 8; i++) {
    pin_array[i] = 0;
  }
  writeCallback();
}

// CONSTRUCTOR
PCF8574::PCF8574(int address = 0x38){ // default address is with A0, A1, and A2 all floating or grounded.
  _WRITE_ADDRESS = address;
  _READ_ADDRESS  = address + 1;
  Wire.begin();
}

int PCF8574::write(uint8_t pin, uint8_t state) {
  // public facing pin-state writing
  if (pin >= 8 || state < 0 || state > 1) {
    return 1; // do nothing - invalid pin number, state is not 0 or 1
  }
  else {
    pin_array[pin] = state;
    writeCallback();
    return 0;
  }
}

// PRIVATE
void PCF8574::writeCallback() {
  //writes pin states, reads things, ect
  pin_byte = arrayToByte(pin_array);
  sendInstructionToChip(_WRITE_ADDRESS, pin_byte);
}

void PCF8574::readCallback() {
  //reads things, ect
  // TODO
}

void PCF8574::sendInstructionToChip(byte address, byte instruction) {
  Wire.beginTransmission(address);
  Wire.write(instruction);
  Wire.endTransmission();
}


// MATH FUNCTIONS
int PCF8574::power(int base, int p) {
  // int-only version of Arduino pow() function
  if (p == 0){
    return 1;
  }
  else {
    int to_return = 1;
    for (int i = 0; i < p; i++) {
      to_return *= base;
    }
    return to_return;
  }
}

uint8_t PCF8574::arrayToByte(uint8_t array[]){
  // Takes an array and converts it from binary to its numerical value
  uint8_t out_byte = 0;
  for (uint8_t i = 0; i < 8; i++) {
    out_byte += array[i] * power(2, 7 - i);
  }
  return out_byte;
}
