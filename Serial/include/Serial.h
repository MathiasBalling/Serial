#pragma once

#include "serialib.h"

class Serial {
private:
  serialib m_serialconnection;

public:
  Serial(const char *port, int baudrate, int databits, int parity,
         int stopbits);
  void closeSerial();
  uint8_t getSeral();
  void write(std::string data);
};
