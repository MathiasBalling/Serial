#include "Serial.h"
#include "serialib.h"

Serial::Serial(const char *port, int baudrate, int databits, int parity,
               int stopbits) {
  // Connection to serial port
  m_serialconnection.openDevice(
      port, baudrate, static_cast<SerialDataBits>(databits),
      static_cast<SerialParity>(parity), static_cast<SerialStopBits>(stopbits));
  m_serialconnection.flushReceiver();
}

int Serial::getSeral() {
  if (!m_serialconnection.isDeviceOpen()) {
    return -1;
    
  } else {
    uint8_t buffer[1];
    // Read the string
    m_serialconnection.readBytes(buffer, 1);
    return buffer[0];
  }
}

void Serial::closeSerial() {
  // m_serialconnection.flushReceiver();
  m_serialconnection.closeDevice();
}

void Serial::sendSerial(uint8_t data) {
  uint8_t val[1] = {data};
  m_serialconnection.writeBytes(val, 1);
}