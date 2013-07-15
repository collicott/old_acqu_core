#include "TApplication.h"
void Test()
{
   // Popup the GUI...
  //  Int_t argc = 1;
  //  Char_t* argv[] = {"GuiCtrl"};
  //  TApplication theApp("TDAQguiCtrl",&argc,argv);
  fGUICtrl = new TDAQguiCtrl(gClient->GetRoot(), 0, 600, 200);  
  //  theApp.Run();
}
