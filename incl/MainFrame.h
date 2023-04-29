#pragma once
#include "Serial.h"
#include <filesystem>
#include <string>
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString &title);

private:
  wxStaticText *decimalText;
  wxStaticText *binaryText;
  wxStaticText *hexText;
  wxButton *openButton;
  wxButton *closeButton;
  Serial *atmega;
  wxTimer *timer;
  std::vector<std::string> m_ports;
  int m_delay = 10;
  std::string m_seriallocation = "/dev/tty.usbserial-A100VZR6";
  int m_baudrate = 9600;
  wxChoice *m_serialchoices;

  void updateText(wxTimerEvent &event);
  void OnExit(wxCommandEvent &event);
  void onOpenClick(wxCommandEvent &event);
  void onCloseClick(wxCommandEvent &event);
  void onBaudRateChange(wxCommandEvent &event);
  void onDelayChange(wxCommandEvent &event);
  void onSerialLocationChange(wxCommandEvent &event);
  void findSerialPorts(wxCommandEvent &event);
};
