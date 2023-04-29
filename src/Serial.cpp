#include "Serial.h"

Serial::Serial(const char *port, int baudrate, int delay) : m_delay(delay) {
  // Connection to serial port
  m_serialconnection.openDevice(port, baudrate);
}

int Serial::getSeral() {
  if (!m_serialconnection.isDeviceOpen()) {
    return -1;
  } else {
    uint8_t buffer[1];
    // Read the string
    m_serialconnection.readBytes(buffer, '\n', 0, m_delay * 1000);
    return buffer[0];
  }
}

void Serial::closeSerial() {
  m_serialconnection.flushReceiver();
  m_serialconnection.closeDevice();
}