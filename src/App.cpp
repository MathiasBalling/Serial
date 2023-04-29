#include "App.h"
#include "MainFrame.h"
#include <iostream>
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
  MainFrame *mainFrame = new MainFrame("ATmega Serial Monitor");
  mainFrame->SetClientSize(300, 250);
  mainFrame->Center();
  mainFrame->Show();
  return true;
}
