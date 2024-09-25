#include "Serial.h"
#include <fmt/core.h>
#include <serialib.h>

Serial::Serial(const char *port, int baudrate, int databits, int parity,
               int stopbits) {
  // Connection to serial port
  m_serialconnection.openDevice(
      port, baudrate, static_cast<SerialDataBits>(databits),
      static_cast<SerialParity>(parity), static_cast<SerialStopBits>(stopbits));
}

uint8_t Serial::getSeral() {
  if (!m_serialconnection.isDeviceOpen()) {
    return -1;
  } else {
    int avl = m_serialconnection.available();
    fmt::print("Available: {}\n", avl);
    uint8_t buffer[1];
    // Read the string
    m_serialconnection.readBytes(buffer, 1);
    return buffer[0];
  }
}

void Serial::closeSerial() {
  m_serialconnection.flushReceiver();
  m_serialconnection.closeDevice();
}

void Serial::write(std::string data) {
  m_serialconnection.writeString(data.c_str());
}
