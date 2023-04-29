// wxWidgets "Hello World" Program

#include <wx/wx.h>

// Header Files
#include "MainFrame.h"

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title) {
  int delay = 10;
  atmega = new Serial("/dev/tty.usbserial-A100VZR6", 9600, delay);

  wxPanel *panel = new wxPanel(this, wxID_ANY);
  panel->SetBackgroundColour(wxColor(0, 0, 0));
  decimalText = new wxStaticText(panel, wxID_ANY, "000", wxDefaultPosition,
                                 wxDefaultSize, wxALIGN_RIGHT);
  decimalText->SetFont(wxFont(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                              wxFONTWEIGHT_BOLD, false, "Arial"));
  binaryText =
      new wxStaticText(panel, wxID_ANY, "0b00000000", wxDefaultPosition);
  binaryText->SetFont(wxFont(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                             wxFONTWEIGHT_BOLD, false, "Arial"));
  hexText = new wxStaticText(panel, wxID_ANY, "0x00", wxDefaultPosition);
  hexText->SetFont(wxFont(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                          wxFONTWEIGHT_BOLD, false, "Arial"));

  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
  vbox->Add(decimalText, 1, wxALIGN_CENTER | wxALL, 10);
  vbox->Add(hexText, 1, wxALIGN_CENTER | wxALL, 10);
  vbox->Add(binaryText, 1, wxALIGN_CENTER | wxALL, 10);
  panel->SetSizerAndFit(vbox);

  timer = new wxTimer(this, wxID_LOWEST - 1);
  Connect(wxID_LOWEST - 1, wxEVT_TIMER,
          wxTimerEventHandler(MainFrame::updateText));
  timer->Start(delay);
}

void MainFrame::updateText(wxTimerEvent &event) {
  int serialout = atmega->getSeral();
  decimalText->SetLabel(wxString::Format("%03d", serialout));
  hexText->SetLabel(wxString::Format("0x%02x", serialout));
  // How to Convert decial to binary
  int binary = 0;
  for (int i = 0; i < 8; i++) {
    if (serialout & (1 << i)) {
      binary += 1 * pow(10, i);
    }
  }

  binaryText->SetLabel(wxString::Format("0b%08d", binary));
}