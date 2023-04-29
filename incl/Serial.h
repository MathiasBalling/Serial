#pragma once
#include <string>

#include "serialib.h"

class Serial {
private:
  serialib m_serialconnection;

public:
  Serial(const char *port, int baudrate);
  void closeSerial();
  int getSeral();
};