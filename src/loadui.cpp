/*
 * This file is part of FBIde, an open-source (cross-platform) IDE for
 * FreeBasic compiler.
 * Copyright (C) 2005  Albert Varaksin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Contact e-mail: Albert Varaksin <vongodric@hotmail.com>
 * Program URL   : http://fbide.sourceforge.net
 */

#ifndef __WXMSW__
    #include "../rc/bitmaps/open.xpm"
    #include "../rc/bitmaps/save.xpm"
    #include "../rc/bitmaps/cut.xpm"
    #include "../rc/bitmaps/copy.xpm"
    #include "../rc/bitmaps/paste.xpm"
    #include "../rc/bitmaps/undo.xpm"
    #include "../rc/bitmaps/redo.xpm"
    #include "../rc/bitmaps/compile.xpm"
    #include "../rc/bitmaps/run.xpm"
    #include "../rc/bitmaps/compnrun.xpm"
    #include "../rc/bitmaps/qrun.xpm"
    #include "../rc/bitmaps/saveall.xpm"
    #include "../rc/bitmaps/close.xpm"
    #include "../rc/bitmaps/output.xpm"
    #include "../rc/bitmaps/new.xpm"
#endif

#include <wx/filename.h>
#include "inc/main.h"
#include "inc/fbedit.h"
#include "inc/browser.h"


//------------------------------------------------------------------------------
//Load menu's
void MyFrame::LoadUI () {

    LoadToolBar();
    LoadMenu();
    LoadStatusBar();
    
    ConsoleSize = -100;

    FB_App->SetTopWindow(this);
    Freeze();
    
        HSplitter = new wxSplitterWindow( 
            this, 10, wxDefaultPosition, wxDefaultSize, 
            wxSP_FULLSASH|wxNO_BORDER );
        HSplitter->SetSashGravity( 1.0 );
        HSplitter->SetMinimumPaneSize( 100 );
        
        FBConsole = new wxListCtrl(HSplitter, 
                                   wxID_ANY, 
                                   wxDefaultPosition, 
                                   wxDefaultSize, 
                                   wxLC_REPORT| wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES );
                wxFont LbFont (10, wxMODERN, wxNORMAL, wxNORMAL, false);
            	FBConsole->SetFont(LbFont);
                wxListItem itemCol;
                itemCol.SetText(Lang[165]); //"Line"
                itemCol.SetAlign(wxLIST_FORMAT_LEFT);
                FBConsole->InsertColumn(0, itemCol);
                itemCol.SetText(Lang[166]); //"File"
                itemCol.SetAlign(wxLIST_FORMAT_LEFT);
                FBConsole->InsertColumn(1, itemCol);
                itemCol.SetText(Lang[167]); //"Error nr"
                itemCol.SetAlign(wxLIST_FORMAT_LEFT);
                FBConsole->InsertColumn(2, itemCol);
                itemCol.SetText(Lang[161]); //"Messages"
                itemCol.SetAlign(wxLIST_FORMAT_LEFT);
                FBConsole->InsertColumn(3, itemCol);
                FBConsole->SetColumnWidth( 0, 60 );
                FBConsole->SetColumnWidth( 1, 150 );
                FBConsole->SetColumnWidth( 2, 100 );
                FBConsole->SetColumnWidth( 3, 600 );
                FBConsole->Hide();
        
        FBCodePanel = new wxPanel(HSplitter, wxID_ANY,
            wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN);
        FBCodePanel->SetBackgroundColour(wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ));
        HSplitter->Initialize( FBCodePanel );

    Thaw();
    SendSizeEvent();
    stc = 0;
    EnableMenus(false);
    return;

}



void MyFrame::LoadMenu () {

    MenuBar = new wxMenuBar();
    
    //File
    FB_File = new wxMenu;
    //wxMenu * file_history = new wxMenu;
    m_FileHistory->UseMenu( FB_File );
    
    FB_File->Append (Menu_New, Lang[11] + _T("\tCtrl+N"), Lang[12]);
    FB_File->Append (Menu_Open, Lang[13] + _T("\tCtrl+O"), Lang[14]);
    //FB_File->Append (Menu_FileHistory, Lang[13] + _T("..."), file_history );
    
    FB_File->AppendSeparator();
    FB_File->Append (Menu_Save,	Lang[15] + _T("\tCtrl+S"), Lang[16]);
    FB_File->Append (Menu_SaveAS, Lang[17] + _T("\tCtrl+Shift+S"), Lang[18]);
    FB_File->Append (Menu_SaveAll, Lang[19], Lang[20]);
    
    FB_File->AppendSeparator();
    FB_File->Append (Menu_SessionLoad, Lang[169], Lang[170]);
    FB_File->Append (Menu_SessionSave, Lang[171], Lang[172]);
    
    FB_File->AppendSeparator();
    FB_File->Append (Menu_Close, Lang[21] + _T("\tCtrl+F4"), Lang[22]);
    FB_File->Append (Menu_CloseAll, Lang[173], Lang[174]);

    FB_File->AppendSeparator();
    FB_File->Append (Menu_NewEditor, Lang[23] + _T("\tShift+Ctrl+N"), Lang[24]);
    FB_File->Append (Menu_Quit, Lang[25] + _T("\tCtrl+Q"), Lang[26]);



    // Edit menu
    _FB_Edit = new wxMenu;
    _FB_Edit->Append (Menu_Undo, 	_(Lang[27] + _T("\tCtrl+Z")), _(Lang[28]));
    _FB_Edit->Append (Menu_Redo, 	_(Lang[29] + _T("\tCtrl+Shift+Z")), _(Lang[30]));
    _FB_Edit->AppendSeparator();
    
    _FB_Edit->Append (Menu_Cut, 		_(Lang[31] + _T("\tCtrl+X")), _(Lang[32]));
    _FB_Edit->Append (Menu_Copy, 	_(Lang[33] + _T("\tCtrl+C")), _(Lang[34]));
    _FB_Edit->Append (Menu_Paste, 	_(Lang[35] + _T("\tCtrl+V")), _(Lang[36]));
    _FB_Edit->AppendSeparator();

    _FB_Edit->Append (Menu_SelectAll,	_(Lang[37] + _T("\tCtrl+A")), _(Lang[38]));
    _FB_Edit->Append (Menu_SelectLine,	_(Lang[39] + _T("\tCtrl+L")), _(Lang[40]));
    _FB_Edit->AppendSeparator();

    _FB_Edit->Append (Menu_IndentIncrease, _(Lang[41] + _T("\tTab")), _(Lang[42]));
    _FB_Edit->Append (Menu_IndentDecrease, _(Lang[43] + _T("\tShift+Tab")), _(Lang[44]));

    _FB_Edit->AppendSeparator();
    _FB_Edit->Append (Menu_Comment,      _(Lang[45] + _T("\tCtrl+M")), _(Lang[46]));
    _FB_Edit->Append (Menu_UnComment,    _(Lang[47] + _T("\tCtrl+Shift+M")), _(Lang[48]));


    // Search menu
    FB_Search = new wxMenu;
    FB_Search->Append (Menu_Find,	  _(Lang[49] + _T("\tCtrl+F")), _(Lang[50]));
    FB_Search->Append (Menu_FindNext, _(Lang[51] + _T("\tF3")), _(Lang[52]));
    FB_Search->Append (Menu_Replace,  _(Lang[53] + _T("\tCtrl+R")), _(Lang[54]));
    FB_Search->Append (Menu_GotoLine, _(Lang[55] + _T("\tCtrl+G")), _(Lang[56]));



    // View menu
    FB_View = new wxMenu;
    FB_View->Append          (Menu_Settings, _(Lang[57]), _(Lang[58]));
    FB_View->Append          (Menu_Format, Lang[175], Lang[176]);
    FB_View->AppendCheckItem (Menu_Result, _(Lang[59] + _T("\tF4")), _(Lang[60]));
    FB_View->Append          (Menu_Subs, _(Lang[61] + _T("\tF2")), _(Lang[62]));
    FB_View->Append          (Menu_CompilerLog, _(Lang[236]), _(Lang[237] ) );
    //FB_Tools->Append (Menu_Converter, _(Language.ToolsConverter), _(Language.ToolsConverterDesc));


    //Run menu
    FB_Run = new wxMenu;
    FB_Run->Append (Menu_Compile,       _(Lang[63] + _T("\tCtrl+F9")), _(Lang[64]));
    FB_Run->Append (Menu_CompileAndRun, _(Lang[65] + _T("\tF9")), _(Lang[66]));
    FB_Run->Append (Menu_Run,           _(Lang[67] + _T("\tShift+Ctrl+F9")), _(Lang[68]));
    FB_Run->Append (Menu_QuickRun,      _(Lang[69] + _T("\tF5")), _(Lang[70]));
    FB_Run->Append (Menu_CmdPromt,      _(Lang[71] + _T("\tF8")), _(Lang[72]));
    FB_Run->Append (Menu_Parameters,    _(Lang[73]), _(Lang[74]));
    FB_Run->AppendCheckItem (Menu_ShowExitCode, _(Lang[77]), _(Lang[78]));
	FB_Run->Check  (Menu_ShowExitCode,  Prefs.ShowExitCode);
	
    FB_Run->AppendCheckItem ( Menu_ActivePath, _(Lang[234]), _(Lang[235]));
    FB_Run->Check  ( Menu_ActivePath,   Prefs.ActivePath );


    //Help
    HelpMenu = new wxMenu;
    HelpMenu->Append(Menu_Help, Lang[ 10 ] + _T("\tF1")  );
    if( !Prefs.UseHelp ) HelpMenu->Enable( Menu_Help, false );
    HelpMenu->Append(Menu_QuickKeys, _T("QuickKeys.txt") );
    HelpMenu->Append(Menu_ReadMe, _T("ReadMe.txt") );
    //HelpMenu->Append(Menu_Fpp, _T("Fpp.txt") );
    HelpMenu->AppendSeparator();
    HelpMenu->Append(Menu_About,   Lang[79],    Lang[80]);


    //Implement menus
    MenuBar->Append(FB_File,  Lang[4]);
    MenuBar->Append(_FB_Edit,  Lang[5]);
    MenuBar->Append(FB_Search,Lang[6]);
    MenuBar->Append(FB_View,  Lang[7]);
    MenuBar->Append(FB_Run,   Lang[9]);
    MenuBar->Append(HelpMenu, Lang[10]);
    SetMenuBar(MenuBar);
    
    return;
}



//------------------------------------------------------------------------------
// Load toolbar
void MyFrame::LoadToolBar () {
	
    // FB_Toolbar = GetToolBar();
    FB_Toolbar = CreateToolBar( wxNO_BORDER | wxTB_HORIZONTAL | wxTB_DOCKABLE | wxTB_FLAT );
    
    // Add controls:
    wxBitmap toolBarBitmaps[15];
    toolBarBitmaps[0] = wxBITMAP(new);
    toolBarBitmaps[1] = wxBITMAP(open);
    toolBarBitmaps[2] = wxBITMAP(save);
    toolBarBitmaps[3] = wxBITMAP(cut);
    toolBarBitmaps[4] = wxBITMAP(copy);
    toolBarBitmaps[5] = wxBITMAP(paste);
    toolBarBitmaps[6] = wxBITMAP(undo);
    toolBarBitmaps[7] = wxBITMAP(redo);
    toolBarBitmaps[8] = wxBITMAP(compile);
    toolBarBitmaps[9] = wxBITMAP(run);
    toolBarBitmaps[10]= wxBITMAP(compnrun);
    toolBarBitmaps[11]= wxBITMAP(qrun);
    toolBarBitmaps[12]= wxBITMAP(saveall);
    toolBarBitmaps[13]= wxBITMAP(close);
    toolBarBitmaps[14]= wxBITMAP(output);

    
    FB_Toolbar->AddTool(Menu_New, toolBarBitmaps[0], wxNullBitmap, FALSE, 100, 100, (wxObject *) NULL, _(Lang[83]));
    FB_Toolbar->AddTool(Menu_Open, toolBarBitmaps[1], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[84]));
    FB_Toolbar->AddTool(Menu_Save, toolBarBitmaps[2], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[85]));
    FB_Toolbar->AddTool(Menu_SaveAll, toolBarBitmaps[12], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[86]));
    FB_Toolbar->AddTool(Menu_Close, toolBarBitmaps[13], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[87]));
    FB_Toolbar->AddSeparator();
    FB_Toolbar->AddTool(Menu_Cut, toolBarBitmaps[3], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[88]));
    FB_Toolbar->AddTool(Menu_Copy, toolBarBitmaps[4], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[89]));
    FB_Toolbar->AddTool(Menu_Paste, toolBarBitmaps[5], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[90]));
    FB_Toolbar->AddSeparator();
    FB_Toolbar->AddTool(Menu_Undo, toolBarBitmaps[6], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[91]));
    FB_Toolbar->AddTool(Menu_Redo, toolBarBitmaps[7], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[92]));
    FB_Toolbar->AddSeparator();
    FB_Toolbar->AddTool(Menu_Compile, toolBarBitmaps[8], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[93]));
    FB_Toolbar->AddTool(Menu_Run, toolBarBitmaps[9], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[94]));
    FB_Toolbar->AddTool(Menu_CompileAndRun, toolBarBitmaps[10], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[95]));
    FB_Toolbar->AddTool(Menu_QuickRun, toolBarBitmaps[11], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[96]));
    FB_Toolbar->AddTool(Menu_Result, toolBarBitmaps[14], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[97]));
    
    FB_Toolbar->Realize();

    return;
}

void MyFrame::EnableMenus ( bool state ) {

    int arrMenus[] = { 
        Menu_Undo, Menu_Redo, Menu_Cut, Menu_Copy, Menu_Paste, 
        Menu_SelectAll, Menu_Find, Menu_Replace, Menu_Save, Menu_SaveAll, 
        Menu_SaveAS, Menu_Close, Menu_SessionSave, Menu_CloseAll, Menu_SelectLine, 
        Menu_IndentIncrease, Menu_IndentDecrease, Menu_Comment, Menu_UnComment, Menu_FindNext, 
        Menu_GotoLine, Menu_Format, Menu_Subs };
    
    for( int idx = 0; idx < 23; idx++ ) {
        MenuBar->Enable( arrMenus[idx], state );
    }

    if ( !ProcessIsRunning ) {
        MenuBar->Enable(Menu_Compile, state);
        MenuBar->Enable(Menu_Run, state);
        MenuBar->Enable(Menu_CompileAndRun, state);
        MenuBar->Enable(Menu_QuickRun, state);
    }
    
    FB_Toolbar->EnableTool(Menu_Save, state);
    FB_Toolbar->EnableTool(Menu_SaveAll, state);
    FB_Toolbar->EnableTool(Menu_Close, state);
    FB_Toolbar->EnableTool(Menu_Cut, state);
    FB_Toolbar->EnableTool(Menu_Copy, state);
    FB_Toolbar->EnableTool(Menu_Paste, state);
    FB_Toolbar->EnableTool(Menu_Undo, state);
    FB_Toolbar->EnableTool(Menu_Redo, state);
    if ( !ProcessIsRunning ) {
        FB_Toolbar->EnableTool(Menu_Compile, state);
        FB_Toolbar->EnableTool(Menu_Run, state);
        FB_Toolbar->EnableTool(Menu_CompileAndRun, state);
        FB_Toolbar->EnableTool(Menu_QuickRun, state);
    }

    return;
}

//------------------------------------------------------------------------------
// Load Statusbar
void MyFrame::LoadStatusBar () {
    CreateStatusBar(2);
    SetStatusText(Lang[1]);
    return;
}


void MyFrame::NewSTCPage ( wxString InitFile, bool select, int FileType ) {
    
    void* doc;
    if ( InitFile == _T("") ) InitFile = FBUNNAMED;
    Buffer* buff;
    
    wxFileName File(InitFile);
    
    if(File.GetExt() == _T("html")||File.GetExt() == _T("htm")) { FileType = 1; }
    else if( File.GetExt() == _T("txt") ) { FileType = 2; }

    if (stc==NULL) {
        Freeze();
            EnableMenus(true);
            OldTabSelected = -1;
    
            m_TabStcSizer = new wxBoxSizer( wxVERTICAL );
                
            FBNotebook = new wxMyNotebook( this, FBCodePanel, wxID_ANY, wxDefaultPosition,
                wxDefaultSize, wxSTATIC_BORDER|wxTB_TOP|wxTB_X);
    
            stc = new FB_Edit( this, FBCodePanel, -1, _T("") );
            m_TabStcSizer->Add( FBNotebook, 0, wxTOP | wxLEFT | wxRIGHT | wxEXPAND | wxALIGN_TOP, 0 );
            m_TabStcSizer->Add( stc, 1, wxBOTTOM | wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 0 );
            FBCodePanel->SetSizer( m_TabStcSizer );
            FBCodePanel->Layout();
                            
            CurrentFileType = FileType;
            stc->LoadSTCTheme( CurrentFileType );
            stc->LoadSTCSettings();
            stc->StyleClearAll();
            stc->LoadSTCTheme( CurrentFileType );
            stc->LoadSTCSettings();
            buff = bufferList.AddFileBuffer(_T(""), _T(""));
            buff->SetFileType(FileType);
            doc = stc->GetDocPointer();
            stc->AddRefDocument(doc);
            stc->SetDocPointer(doc);
            if (InitFile!=FBUNNAMED) stc->LoadFile(InitFile);
            FBNotebook->AddPage( wxFileNameFromPath(InitFile), true );
            
            SendSizeEvent();
        Thaw();
    }
    else {
        stc->SetBuffer( (Buffer *) 0 );
        buff = bufferList.AddFileBuffer(_T(""), _T(""));
        buff->SetFileType(FileType);
        SaveDocumentStatus(FBNotebook->GetSelection());
        doc = stc->CreateDocument();
        stc->AddRefDocument(doc);
        stc->SetDocPointer(doc);
        OldTabSelected = -1;
        if (InitFile!=FBUNNAMED) stc->LoadFile(InitFile);
        FBNotebook->AddPage( wxFileNameFromPath(InitFile), true);
        
        if ( FileType != CurrentFileType ) {
            CurrentFileType = FileType;
            stc->LoadSTCTheme( CurrentFileType );
        }
        stc->LoadSTCSettings();
    }
    
    buff->SetFileName(InitFile);
    buff->SetModified(false);
    buff->UpdateModTime();
    buff->SetDocument(doc);
    stc->SetBuffer( (Buffer *) buff );
    stc->SetFocus();
    
    if (SFDialog) SFDialog->Rebuild();
    if ( select ) {
        SetTitle( _T("FBIde - ") + InitFile );
    }

    return;
}


void MyFrame::ChangeNBPage   ( wxTabbedCtrlEvent& event) {
    if (OldTabSelected==-1) {
        OldTabSelected = 0 ;
        return;
    }
    if (stc==0) return;
    
    int index = event.GetSelection();
    if (FBNotebook->GetPageCount()>1) SaveDocumentStatus(event.GetOldSelection());
    SetSTCPage ( index );
    SetTitle( _T("FBIde - ") + bufferList[index]->GetFileName() );
    return;
}


void MyFrame::SaveDocumentStatus ( int docID ) {
    Buffer* buff = bufferList.GetBuffer(docID);
    buff->SetPositions(stc->GetSelectionStart(), stc->GetSelectionEnd());
    buff->SetLine(stc->GetFirstVisibleLine());
    buff->SetCaretPos(stc->GetCurrentPos());
}

void MyFrame::SetSTCPage ( int index ) {
    if (stc == 0) return;
    
    //stc->Freeze();
                
        stc->SetBuffer( (Buffer *) 0 );
        
        Buffer* buff = bufferList.GetBuffer(index);
        
        void* doc = buff->GetDocument();
        stc->AddRefDocument(doc);
        stc->SetDocPointer(doc);
    
        stc->ScrollToLine(buff->GetLine());
        stc->SetCurrentPos(buff->GetCaretPos());
        stc->SetSelectionStart(buff->GetSelectionStart());
        stc->SetSelectionEnd(buff->GetSelectionEnd());
        stc->SetFocus();
        stc->SetBuffer( (Buffer *) buff);
        if ( buff->GetFileType() != CurrentFileType ) {
            CurrentFileType = buff->GetFileType();
            stc->LoadSTCTheme( CurrentFileType );
            stc->LoadSTCSettings();
        }
        
    //stc->Thaw();
    if (SFDialog) SFDialog->Rebuild();
}

void MyFrame::SetModified ( int index, bool status ) {

    if (index==-1) index = FBNotebook->GetSelection();
    
    Buffer* buff = bufferList.GetBuffer(index);

    buff->SetWasModified(buff->GetModified());
    wxString NewName;
    if(status) NewName << _T("[*] ");
    
    bufferList.SetBufferModified(index, status);
    NewName << wxFileNameFromPath(buff->GetFileName());
    FBNotebook->SetPageText( index, NewName );

}

