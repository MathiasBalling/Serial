// wxWidgets "Hello World" Program

#include <iostream>
#include <vector>
#include <wx/wx.h>

// Header Files
#include "MainFrame.h"

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title) {

  enum control_ids { ID_Timer = wxID_HIGHEST + 1 };
  wxPanel *control = new wxPanel(this, wxID_ANY);
  control->SetBackgroundColour(wxColor(20, 20, 20));
  openButton = new wxButton(control, wxID_ANY, "Open");
  openButton->Bind(wxEVT_BUTTON, &MainFrame::onOpenClick, this);
  closeButton = new wxButton(control, wxID_ANY, "Close");
  closeButton->Bind(wxEVT_BUTTON, &MainFrame::onCloseClick, this);
  closeButton->Enable(false);
  wxButton *exitButton = new wxButton(control, wxID_ANY, "Exit");
  exitButton->Bind(wxEVT_BUTTON, &MainFrame::OnExit, this);

  wxBoxSizer *controlSizer = new wxBoxSizer(wxHORIZONTAL);
  controlSizer->Add(openButton, 1, wxALL, 10);
  controlSizer->Add(closeButton, 1, wxALL, 10);
  controlSizer->Add(exitButton, 1, wxALL, 10);
  control->SetSizerAndFit(controlSizer);

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

  wxBoxSizer *outputSizer = new wxBoxSizer(wxVERTICAL);
  outputSizer->Add(decimalText, 1, wxALIGN_CENTER | wxALL, 10);
  outputSizer->Add(hexText, 1, wxALIGN_CENTER | wxALL, 10);
  outputSizer->Add(binaryText, 1, wxALIGN_CENTER | wxALL, 10);
  panel->SetSizerAndFit(outputSizer);

  wxPanel *inputs = new wxPanel(this, wxID_ANY);
  inputs->SetBackgroundColour(wxColor(20, 20, 20));
  wxStaticText *serialLocationLabel =
      new wxStaticText(inputs, wxID_ANY, "Serial Location:");
  m_serialchoices = new wxChoice(inputs, wxID_ANY);
  m_serialchoices->Bind(wxEVT_CHOICE, &MainFrame::findSerialPorts, this);
  Bind(wxEVT_CHOICE, &MainFrame::onSerialLocationChange, this);
  wxCommandEvent tempevt;
  findSerialPorts(tempevt);
  wxStaticText *baudRateLabel =
      new wxStaticText(inputs, wxID_ANY, "Baud Rate:");
  wxTextCtrl *baudRateText =
      new wxTextCtrl(inputs, wxID_ANY, wxString::Format("%d", m_baudrate));
  baudRateText->Bind(wxEVT_TEXT, &MainFrame::onBaudRateChange, this);
  wxStaticText *delayLabel = new wxStaticText(inputs, wxID_ANY, "Delay(ms):");
  wxTextCtrl *delayText =
      new wxTextCtrl(inputs, wxID_ANY, wxString::Format("%d", m_delay));
  delayText->Bind(wxEVT_TEXT, &MainFrame::onDelayChange, this);

  wxBoxSizer *inputsSizer2 = new wxBoxSizer(wxHORIZONTAL);
  inputsSizer2->Add(baudRateLabel, 0, wxALIGN_CENTER | wxLEFT, 10);
  inputsSizer2->Add(baudRateText, 1, wxALIGN_CENTER);
  inputsSizer2->Add(delayLabel, 0, wxALIGN_CENTER | wxLEFT, 10);
  inputsSizer2->Add(delayText, 1, wxALIGN_CENTER);

  wxBoxSizer *inputsSizer1 = new wxBoxSizer(wxHORIZONTAL);
  inputsSizer1->Add(serialLocationLabel, 0, wxALIGN_CENTER | wxLEFT, 10);
  inputsSizer1->Add(m_serialchoices, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT, 3);

  wxBoxSizer *inputsSizer = new wxBoxSizer(wxVERTICAL);
  inputsSizer->Add(inputsSizer1, 0, wxEXPAND | wxALL, 3);
  inputsSizer->Add(inputsSizer2, 0, wxEXPAND | wxALL, 3);
  inputs->SetSizerAndFit(inputsSizer);

  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  mainSizer->Add(control, 0, wxEXPAND);
  mainSizer->Add(inputs, 0, wxEXPAND);
  mainSizer->Add(panel, 1, wxEXPAND);
  SetSizerAndFit(mainSizer);

  timer = new wxTimer(this, ID_Timer);
  Bind(wxEVT_TIMER, &MainFrame::updateText, this, ID_Timer);
}

void MainFrame::updateText(wxTimerEvent &event) {
  int serialout = atmega->getSeral();
  if (serialout == -1) {
    wxCommandEvent event;
    onCloseClick(event);
    std::cout << "Serial failed!" << std::endl;
    return;
  }
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

void MainFrame::OnExit(wxCommandEvent &event) { Close(true); }

void MainFrame::onOpenClick(wxCommandEvent &event) {
  const char *seriallocation = m_seriallocation.data();
  atmega = new Serial(seriallocation, m_baudrate, m_delay);
  timer->Start(m_delay);
  openButton->Enable(false);
  closeButton->Enable(true);
}

void MainFrame::onCloseClick(wxCommandEvent &event) {
  openButton->Enable(true);
  closeButton->Enable(false);
  atmega->closeSerial();
  delete atmega;
  timer->Stop();
}

void MainFrame::onSerialLocationChange(wxCommandEvent &event) {
  std::cout << m_seriallocation << std::endl;
  std::cout << "Serial Location Changed:" << std::endl;
  m_seriallocation = event.GetString().ToStdString();
  std::cout << m_seriallocation << std::endl;
}

void MainFrame::onBaudRateChange(wxCommandEvent &event) {
  m_baudrate = wxAtoi(event.GetString());
}

void MainFrame::onDelayChange(wxCommandEvent &event) {
  m_delay = wxAtoi(event.GetString());
}

void MainFrame::findSerialPorts(wxCommandEvent &event) {
  std::string path = "/dev/";
  m_ports.clear();
  for (const auto &entry : std::filesystem::directory_iterator(path))
    if (entry.path().string().find("tty.") != std::string::npos)
      m_ports.push_back(entry.path().string());

  wxArrayString choices;
  for (auto i : m_ports)
    choices.Add(i);

  int selected = m_serialchoices->GetSelection();

  m_serialchoices->Set(choices);
  m_serialchoices->SetSelection(selected);
}
