#include "Serial.h"
#include "serialib.h"

Serial::Serial(const char *port, int baudrate) {
  // Connection to serial port
  m_serialconnection.openDevice(port, baudrate);
}

int Serial::getSeral() {
  if (!m_serialconnection.isDeviceOpen()) {
    return -1;
  } else {
    uint8_t buffer[1];
    // Read the string
    m_serialconnection.readBytes(buffer, '\n');
    return buffer[0];
  }
}

void Serial::closeSerial() {
  m_serialconnection.flushReceiver();
  m_serialconnection.closeDevice();
}