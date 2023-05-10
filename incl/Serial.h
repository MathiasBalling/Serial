#pragma once
#include <stdint.h>
#include <string>

#include "serialib.h"

class Serial {
private:
  serialib m_serialconnection;

public:
  Serial(const char *port, int baudrate, int databits, int parity,
         int stopbits);
  void closeSerial();
  int getSeral();
  void sendSerial(uint8_t data);
};