// wxWidgets "Hello World" Program

#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>
#include <wx/wx.h>

// Header Files
#include "MainFrame.h"

enum SendType { BINARY, HEX, DECIMAL, ASCII };

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title) {

  enum control_ids { ID_timer = wxID_HIGHEST + 1 };
  makeUI();
  makeSettingsDialog();

  auto timer = new wxTimer(this, ID_timer);
  Bind(wxEVT_TIMER, &MainFrame::findSerialPorts, this, ID_timer);
  timer->Start(1000);

  m_worker = std::thread(&MainFrame::updateText, this);
}

void MainFrame::updateText() {
  while (1) {
    if (isSerialOpen) {
      int serialout = m_serial->getSeral();
      if (serialout == -1) {
        isSerialOpen = false;
        wxCommandEvent event;
        onCloseClick(event);
        std::cout << "Serial failed!" << std::endl;
        return;
      }
      // Convert decial to binary
      int binary = 0;
      for (int i = 0; i < 8; i++) {
        if (serialout & (1 << i)) {
          binary += 1 * pow(10, i);
        }
      }

      CallAfter([this, serialout, binary]() {
        historyCtrl->AppendText(wxString::Format("%02x ", serialout));
        decimalText->SetLabel(wxString::Format("%03d", serialout));
        hexText->SetLabel(wxString::Format("0x%02x", serialout));
        binaryText->SetLabel(wxString::Format("0b%08d", binary));
        this->Layout();
      });
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}

void MainFrame::OnControlClicked(wxCommandEvent &event) {
  settingsDialog->Show(true);
}

void MainFrame::onOpenClick(wxCommandEvent &event) {
  const char *seriallocation = m_seriallocation.data();
  m_serial =
      new Serial(seriallocation, m_baudrate, m_dataBits, m_parity, m_stopBits);
  isSerialOpen = true;
  openButton->Enable(false);
  closeButton->Enable(true);
}

void MainFrame::onCloseClick(wxCommandEvent &event) {
  isSerialOpen = false;
  openButton->Enable(true);
  closeButton->Enable(false);
  m_serial->closeSerial();
  delete m_serial;
}

void MainFrame::findSerialPorts(wxTimerEvent &event) {
  std::string path = "/dev/";
  m_ports.clear();
  for (const auto &entry : std::filesystem::directory_iterator(path))
    if (entry.path().string().find("tty.") != std::string::npos)
      m_ports.push_back(entry.path().string());

  wxArrayString choices;
  if (m_costumPath) {
    choices.Add(m_seriallocation);
    m_serialchoices->Set(choices);
    m_serialchoices->SetSelection(0);
  } else {
    for (auto i : m_ports)
      choices.Add(i);
    int selected = m_serialchoices->GetSelection();
    if (selected == -1) {
      selected = 0;
      m_seriallocation = m_ports[0];
    }
    m_serialchoices->Set(choices);
    m_serialchoices->SetSelection(selected);
  }
}

void MainFrame::makeUI() {
  SetBackgroundColour(wxColour(20, 20, 20));
  wxPanel *controls = new wxPanel(this, wxID_ANY);
  controls->SetBackgroundColour(wxColor(20, 20, 20));
  openButton = new wxButton(controls, wxID_ANY, "Open");
  openButton->Bind(wxEVT_BUTTON, &MainFrame::onOpenClick, this);
  closeButton = new wxButton(controls, wxID_ANY, "Close");
  closeButton->Bind(wxEVT_BUTTON, &MainFrame::onCloseClick, this);
  closeButton->Enable(false);
  wxButton *controlButton = new wxButton(controls, wxID_ANY, "Settings");
  controlButton->Bind(wxEVT_BUTTON, &MainFrame::OnControlClicked, this);

  wxBoxSizer *controlSizer = new wxBoxSizer(wxHORIZONTAL);
  controlSizer->Add(openButton, 1, wxLEFT, 10);
  controlSizer->Add(closeButton, 1, wxLEFT, 10);
  controlSizer->Add(controlButton, 1, wxLEFT, 10);
  controlSizer->AddSpacer(10);
  controls->SetSizerAndFit(controlSizer);

  wxPanel *panel = new wxPanel(this, wxID_ANY);
  panel->SetBackgroundColour(wxColor(0, 0, 0));
  decimalText = new wxStaticText(panel, wxID_ANY, "000", wxDefaultPosition,
                                 wxDefaultSize, wxALIGN_CENTRE);
  decimalText->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                              wxFONTWEIGHT_BOLD, false, "Arial"));
  binaryText =
      new wxStaticText(panel, wxID_ANY, "0b00000000", wxDefaultPosition,
                       wxDefaultSize, wxALIGN_CENTRE);
  binaryText->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                             wxFONTWEIGHT_BOLD, false, "Arial"));
  hexText = new wxStaticText(panel, wxID_ANY, "0x00", wxDefaultPosition,
                             wxDefaultSize, wxALIGN_CENTRE);
  hexText->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                          wxFONTWEIGHT_BOLD, false, "Arial"));
  historyCtrl = new wxTextCtrl(
      panel, wxID_ANY, "", wxDefaultPosition, wxSize(500, 200),
      wxTE_MULTILINE | wxTE_READONLY | wxTE_NO_VSCROLL | wxBORDER_NONE);
  historyCtrl->SetBackgroundColour(wxColor(0, 0, 0));
  historyCtrl->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                              wxFONTWEIGHT_NORMAL, false, "Arial"));

  wxBoxSizer *outputSizerHor = new wxBoxSizer(wxHORIZONTAL);
  outputSizerHor->AddSpacer(10);
  outputSizerHor->Add(decimalText, 1);
  outputSizerHor->AddSpacer(10);
  outputSizerHor->Add(hexText, 1);
  outputSizerHor->AddSpacer(10);
  outputSizerHor->Add(binaryText, 1);
  outputSizerHor->AddSpacer(10);

  wxChoice *sendchoices = new wxChoice(panel, wxID_ANY);
  wxArrayString choices;
  choices.Add("Binary");
  choices.Add("Hex");
  choices.Add("Decimal");
  choices.Add("ASCII");
  sendchoices->Set(choices);
  sendchoices->SetSelection(1);
  sendchoices->Bind(wxEVT_CHOICE, [this](wxCommandEvent &event) {
    switch (event.GetSelection()) {
    case 0:
      m_sendType = SendType::BINARY;
      break;
    case 1:
      m_sendType = SendType::HEX;
      break;
    case 2:
      m_sendType = SendType::DECIMAL;
      break;
    case 3:
      m_sendType = SendType::ASCII;
      break;
    }
  });
  wxTextCtrl *sendText = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition,
                                        wxDefaultSize, wxTE_PROCESS_ENTER);
  sendText->Bind(wxEVT_TEXT_ENTER, [this, sendText](wxCommandEvent &event) {
    sendText->Clear();
    std::string text = event.GetString().ToStdString();

    switch (m_sendType) {
    case SendType::BINARY:
      text = std::to_string(std::stoi(text, nullptr, 2));
      break;
    case SendType::HEX:
      text = std::to_string(std::stoi(text, nullptr, 16));
      break;
    case SendType::DECIMAL:
      break;
    case SendType::ASCII:
      break;
    }
    m_serial->write(text);
  });
  wxBoxSizer *textinputSizer = new wxBoxSizer(wxHORIZONTAL);
  textinputSizer->AddSpacer(10);
  textinputSizer->Add(sendchoices, 0, wxALIGN_CENTER_VERTICAL);
  textinputSizer->AddSpacer(10);
  textinputSizer->Add(sendText, 1, wxEXPAND);
  textinputSizer->AddSpacer(10);

  wxBoxSizer *outputSizerVer = new wxBoxSizer(wxVERTICAL);
  outputSizerVer->AddSpacer(10);
  outputSizerVer->Add(outputSizerHor, 0, wxEXPAND);
  outputSizerVer->AddSpacer(10);
  outputSizerVer->Add(historyCtrl, 1, wxEXPAND);
  outputSizerVer->AddSpacer(10);
  outputSizerVer->Add(textinputSizer, 0, wxEXPAND);
  outputSizerVer->AddSpacer(10);
  panel->SetSizerAndFit(outputSizerVer);

  wxPanel *inputs = new wxPanel(this, wxID_ANY);
  inputs->SetBackgroundColour(wxColor(20, 20, 20));
  wxStaticText *serialLocationLabel =
      new wxStaticText(inputs, wxID_ANY, "Serial Location:");
  m_serialchoices = new wxChoice(inputs, wxID_ANY);
  m_serialchoices->Bind(wxEVT_CHOICE, [this](wxCommandEvent &event) {
    m_seriallocation = m_serialchoices->GetStringSelection().ToStdString();
  });

  wxBoxSizer *inputsSizer = new wxBoxSizer(wxHORIZONTAL);
  inputsSizer->Add(serialLocationLabel, 0, wxLEFT, 10);
  inputsSizer->Add(m_serialchoices, 1, wxLEFT, 5);
  inputsSizer->AddSpacer(10);
  inputs->SetSizerAndFit(inputsSizer);

  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  mainSizer->AddSpacer(10);
  mainSizer->Add(controls, 0, wxEXPAND);
  mainSizer->AddSpacer(10);
  mainSizer->Add(inputs, 0, wxEXPAND);
  mainSizer->AddSpacer(5);
  mainSizer->Add(panel, 1, wxEXPAND);
  SetSizerAndFit(mainSizer);
}

void MainFrame::makeSettingsDialog() {
  // Make the control dialog
  settingsDialog = new wxDialog(this, wxID_ANY, "Settings");
  settingsDialog->SetBackgroundColour(wxColor(20, 20, 20));

  wxCheckBox *RTSLabel = new wxCheckBox(settingsDialog, wxID_ANY, "RTS");
  RTSLabel->SetValue(m_RTS);
  RTSLabel->Bind(wxEVT_CHECKBOX,
                 [this](wxCommandEvent &event) { m_RTS = event.IsChecked(); });
  wxCheckBox *DTRLabel = new wxCheckBox(settingsDialog, wxID_ANY, "DTR");
  DTRLabel->SetValue(m_DTR);
  DTRLabel->Bind(wxEVT_CHECKBOX,
                 [this](wxCommandEvent &event) { m_DTR = event.IsChecked(); });

  wxCheckBox *costumPathCheckBox =
      new wxCheckBox(settingsDialog, wxID_ANY, "Costum Path:");
  wxStaticText *serialLocationLabel =
      new wxStaticText(settingsDialog, wxID_ANY, "Serial Location:");
  serialLocationLabel->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT,
                                      wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD,
                                      false, "Arial"));
  m_serialLocationText = new wxTextCtrl(settingsDialog, wxID_ANY, "/dev/tty",
                                        wxDefaultPosition, wxSize(150, -1));
  m_serialLocationText->Enable(false);
  m_serialLocationText->SetValidator(wxTextValidator(wxFILTER_ASCII));
  m_serialLocationText->Bind(wxEVT_TEXT, [this](wxCommandEvent &event) {
    m_seriallocation = event.GetString().ToStdString();
  });
  costumPathCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent &event) {
    if (event.IsChecked()) {
      m_costumPath = true;
      m_serialLocationText->Enable(true);
    } else {
      m_costumPath = false;
      m_seriallocation = m_ports[0];
      m_serialLocationText->Enable(false);
    }
  });

  wxStaticText *baudRateLabel =
      new wxStaticText(settingsDialog, wxID_ANY, "Baud Rate:");
  baudRateLabel->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_BOLD, false, "Arial"));
  wxTextCtrl *baudRateText =
      new wxTextCtrl(settingsDialog, wxID_ANY, std::to_string(m_baudrate));
  baudRateText->SetValidator(wxTextValidator(wxFILTER_DIGITS));
  baudRateText->Bind(wxEVT_TEXT, [this](wxCommandEvent &event) {
    m_baudrate = wxAtoi(event.GetString());
  });

  wxStaticText *dataBitsLabel =
      new wxStaticText(settingsDialog, wxID_ANY, "Data Bits:");
  dataBitsLabel->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_BOLD, false, "Arial"));
  wxChoice *dataBitsChoice = new wxChoice(settingsDialog, wxID_ANY);
  wxArrayString choices;
  choices.Add("5");
  choices.Add("6");
  choices.Add("7");
  choices.Add("8");
  choices.Add("16");
  dataBitsChoice->Set(choices);
  dataBitsChoice->SetSelection(3);
  dataBitsChoice->Bind(wxEVT_CHOICE, [this](wxCommandEvent &event) {
    m_dataBits = event.GetSelection();
  });

  wxStaticText *parityLabel =
      new wxStaticText(settingsDialog, wxID_ANY, "Parity:");
  parityLabel->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                              wxFONTWEIGHT_BOLD, false, "Arial"));
  wxChoice *parityChoice = new wxChoice(settingsDialog, wxID_ANY);
  choices.Clear();
  choices.Add("None");
  choices.Add("Even");
  choices.Add("Odd");
  choices.Add("Mark");
  choices.Add("Space");
  parityChoice->Set(choices);
  parityChoice->SetSelection(0);
  parityChoice->Bind(wxEVT_CHOICE, [this](wxCommandEvent &event) {
    m_parity = event.GetSelection();
  });

  wxStaticText *stopBitsLabel =
      new wxStaticText(settingsDialog, wxID_ANY, "Stop Bits:");
  stopBitsLabel->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_BOLD, false, "Arial"));
  wxChoice *stopBitsChoice = new wxChoice(settingsDialog, wxID_ANY);
  choices.Clear();
  choices.Add("1");
  choices.Add("1.5");
  choices.Add("2");
  stopBitsChoice->Set(choices);
  stopBitsChoice->SetSelection(0);
  stopBitsChoice->Bind(wxEVT_CHOICE, [this](wxCommandEvent &event) {
    m_stopBits = event.GetSelection();
  });

  wxBoxSizer *Sizer1 = new wxBoxSizer(wxHORIZONTAL);
  Sizer1->Add(RTSLabel, 0, wxLEFT, 10);
  Sizer1->Add(DTRLabel, 0, wxLEFT, 10);
  Sizer1->AddSpacer(10);

  wxBoxSizer *seiralSizer = new wxBoxSizer(wxHORIZONTAL);
  seiralSizer->Add(costumPathCheckBox, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  seiralSizer->Add(m_serialLocationText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT,
                   5);
  seiralSizer->AddSpacer(10);

  wxBoxSizer *baudRateSizer = new wxBoxSizer(wxHORIZONTAL);
  baudRateSizer->Add(baudRateLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  baudRateSizer->Add(baudRateText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  baudRateSizer->AddSpacer(10);

  wxBoxSizer *dataBitsSizer = new wxBoxSizer(wxHORIZONTAL);
  dataBitsSizer->Add(dataBitsLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  dataBitsSizer->Add(dataBitsChoice, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  dataBitsSizer->AddSpacer(10);

  wxBoxSizer *paritySizer = new wxBoxSizer(wxHORIZONTAL);
  paritySizer->Add(parityLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  paritySizer->Add(parityChoice, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  paritySizer->AddSpacer(10);

  wxBoxSizer *stopBitsSizer = new wxBoxSizer(wxHORIZONTAL);
  stopBitsSizer->Add(stopBitsLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  stopBitsSizer->Add(stopBitsChoice, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  stopBitsSizer->AddSpacer(10);

  wxBoxSizer *controlSizer = new wxBoxSizer(wxVERTICAL);
  controlSizer->AddSpacer(10);

  controlSizer->Add(Sizer1, 1, wxEXPAND);

  controlSizer->AddSpacer(10);

  controlSizer->Add(serialLocationLabel, 0, wxLEFT, 10);
  controlSizer->AddSpacer(5);
  controlSizer->Add(seiralSizer, 1, wxEXPAND);

  controlSizer->AddSpacer(10);

  controlSizer->Add(baudRateSizer, 1, wxEXPAND);

  controlSizer->AddSpacer(10);

  controlSizer->Add(dataBitsSizer, 1, wxEXPAND);

  controlSizer->AddSpacer(10);

  controlSizer->Add(paritySizer, 1, wxEXPAND);

  controlSizer->AddSpacer(10);

  controlSizer->Add(stopBitsSizer, 1, wxEXPAND);

  controlSizer->AddSpacer(10);
  settingsDialog->SetSizerAndFit(controlSizer);
}
