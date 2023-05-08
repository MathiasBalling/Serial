#pragma once
#include "App.h"
#include "Serial.h"
#include <filesystem>
#include <string>
#include <thread>
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString &title);

private:
  Serial *m_serial;
  int m_sendType;
  bool isSerialOpen = false;
  std::thread m_worker;
  std::vector<std::string> m_ports;
  std::string m_seriallocation;
  wxChoice *m_serialchoices;

  wxStaticText *decimalText;
  wxStaticText *binaryText;
  wxStaticText *hexText;

  wxTextCtrl *historyCtrl;

  wxButton *openButton;
  wxButton *closeButton;

  wxTimer *timer;

  wxDialog *settingsDialog;
  wxTextCtrl *m_serialLocationText;
  int m_baudrate = 9600;
  bool m_DTR = false;
  bool m_RTS = false;
  int m_dataBits = 3;
  int m_stopBits = 0;
  int m_parity = 0;
  bool m_costumPath = false;

  void updateText();
  void OnControlClicked(wxCommandEvent &event);
  void onOpenClick(wxCommandEvent &event);
  void onCloseClick(wxCommandEvent &event);
  void findSerialPorts(wxTimerEvent &event);
  void makeUI();
  void makeSettingsDialog();
};
