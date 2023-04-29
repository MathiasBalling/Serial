#pragma once
#include "Serial.h"
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString &title);

private:
  wxStaticText *decimalText;
  wxStaticText *binaryText;
  wxStaticText *hexText;
  Serial *atmega;
  wxTimer *timer;
  void updateText(wxTimerEvent &event);
};
