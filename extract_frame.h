//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: extract_frame.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef RATVIDEO_EXTRACT_FRAME_BASE_CLASSES_H
#define RATVIDEO_EXTRACT_FRAME_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif


class DlgExtractFrameBase : public wxDialog
{
protected:
    wxStaticText* m_staticText6;
    wxTextCtrl* m_textCtrlFromMM;
    wxStaticText* m_staticText10;
    wxTextCtrl* m_textCtrlFromSS;
    wxStaticText* m_staticText14;
    wxStaticText* m_staticText20;
    wxTextCtrl* m_textCtrlToMM;
    wxStaticText* m_staticText22;
    wxTextCtrl* m_textCtrlToSS;
    wxStaticText* m_staticText26;
    wxStdDialogButtonSizer* m_stdBtnSizer28;
    wxButton* m_buttonOK;
    wxButton* m_buttonCancel;

protected:

public:
    wxStaticText* GetStaticText6() { return m_staticText6; }
    wxTextCtrl* GetTextCtrlFromMM() { return m_textCtrlFromMM; }
    wxStaticText* GetStaticText10() { return m_staticText10; }
    wxTextCtrl* GetTextCtrlFromSS() { return m_textCtrlFromSS; }
    wxStaticText* GetStaticText14() { return m_staticText14; }
    wxStaticText* GetStaticText20() { return m_staticText20; }
    wxTextCtrl* GetTextCtrlToMM() { return m_textCtrlToMM; }
    wxStaticText* GetStaticText22() { return m_staticText22; }
    wxTextCtrl* GetTextCtrlToSS() { return m_textCtrlToSS; }
    wxStaticText* GetStaticText26() { return m_staticText26; }
    DlgExtractFrameBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Extract Frames"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(300,200), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~DlgExtractFrameBase();
};

#endif