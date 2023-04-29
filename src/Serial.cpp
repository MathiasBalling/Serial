#include "Serial.h"

Serial::Serial(const char *port, int baudrate, int delay)
    : m_port(port), m_baudrate(baudrate), m_delay(delay) {
  // Connection to serial port
  char errorOpening = m_serialconnection.openDevice(port, baudrate);

  if (errorOpening != 1) {
    std::cout << "Error while opening port. Permission problem ?" << std::endl;
  } else {
    std::cout << "Successful connection to " << port << std::endl;
  }
  m_serialconnection.flushReceiver();
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

void Serial::closeSerial() { m_serialconnection.closeDevice(); }