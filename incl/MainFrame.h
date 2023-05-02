#pragma once
#include "Serial.h"
#include <filesystem>
#include <string>
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString &title);

private:
  Serial *atmega;
  std::vector<std::string> m_ports;
  std::string m_seriallocation;
  wxChoice *m_serialchoices;

  wxStaticText *decimalText;
  wxStaticText *binaryText;
  wxStaticText *hexText;

  wxButton *openButton;
  wxButton *closeButton;

  wxTimer *timer;

  wxDialog *settingsDialog;
  wxTextCtrl *m_serialLocationText;
  int m_baudrate = 9600;
  bool m_DTR = true;
  bool m_RTS = true;
  int m_delay = 100;
  int m_dataBits = 8;
  int m_stopBits = 1;
  int m_parity = 0;
  bool m_costumPath = false;

  void updateText(wxTimerEvent &event);
  void OnControlClicked(wxCommandEvent &event);
  void onOpenClick(wxCommandEvent &event);
  void onCloseClick(wxCommandEvent &event);
  void findSerialPorts(wxTimerEvent &event);
  void makeUI();
  void makeSettingsDialog();
};
