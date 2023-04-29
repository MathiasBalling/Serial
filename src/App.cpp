#include "App.h"
#include "MainFrame.h"
#include <iostream>
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
  MainFrame *mainFrame = new MainFrame("Serial Monitor");
  mainFrame->Center();
  mainFrame->Show();
  return true;
}
