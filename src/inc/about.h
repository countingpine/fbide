
// Don't modify comment 
#ifndef __VwX_MyDlg_H__
#define __VwX_MyDlg_H__
#include <wx/settings.h>
#include <wx/dialog.h>
 
//[inc]add your include files here



//[inc]end your include
 
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/textctrl.h>
class about;
class aboutEvt:public wxEvtHandler
{
public:
 aboutEvt(about *parent){ptr_winPan=parent;}
protected:  
 about *ptr_winPan;
 DECLARE_EVENT_TABLE()     
//[evt]add your code here



//[evt]end your code
};

class about:public wxDialog
{
 friend class aboutEvt;
public:
 about(wxWindow* parent, wxWindowID id = -1, const wxString& title = wxT(""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_NO_PARENT|wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX, const wxString& name = wxT("dialogBox"));
// Pointer control
 wxStaticBitmap *bm3;
 wxButton *button_ok;
 wxTextCtrl *txm7;
 virtual ~about();
 virtual void Dabout();
 virtual void initBefore();
 void OnPreCreate();
 void VwXinit();
 wxBitmap *bm3Img0;
 wxBitmap fileImgBuf[1];
 virtual void initAfter();
 MyFrame * Parent;
protected:
 void VwXVwXEvOnButtonClick(wxCommandEvent& event);
 void button_ok_VwXEvOnButtonClick(wxCommandEvent& event,int index=-1);
 DECLARE_EVENT_TABLE()  
//[win]add your code here



 #define VwX_INIT_OBJECTS_about
//[win]end your code 
};
// end about

#endif