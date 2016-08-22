//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef RATVIDEO_WXCRAFTER_BASE_CLASSES_H
#define RATVIDEO_WXCRAFTER_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>
#include <wx/pen.h>
#include <wx/aui/auibar.h>
#include <map>
#include <wx/toolbar.h>
#include <wx/aui/auibook.h>
#include <wx/panel.h>
#include <wx/imaglist.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/radiobut.h>
#include <wx/richtext/richtextctrl.h>
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


class MainFrameBaseClass : public wxFrame
{
public:
    enum {
        wxID_BGS_PROCESS = 10001,
        wxID_EXTRACT_FRAME = 10002,
        wxID_FRAME_PROCESSOR = 10003,
    };
protected:
    wxMenuBar* m_menuBar;
    wxMenu* m_menuFile;
    wxMenuItem* m_menuItemFileOpen;
    wxMenuItem* m_menuItemExit;
    wxMenu* m_menuView;
    wxMenuItem* m_menuItemViewMsgPane;
    wxMenu* m_menuVideo;
    wxMenuItem* m_menuItemVideoBGSProcess;
    wxMenuItem* m_menuItemVideoFrameProcessor;
    wxMenuItem* m_menuItemVideoExtractFrame;
    wxMenuItem* m_menuItem138;
    wxMenuItem* m_menuItemVideoFGPixels;
    wxMenu* m_menuBackground;
    wxMenuItem* m_menuItemBgKDE;
    wxMenu* m_nameHelp;
    wxMenuItem* m_menuItem9;
    wxStatusBar* m_statusBar;
    wxAuiManager* m_auimgr21;
    wxAuiToolBar* m_auibar23;
    wxAuiNotebook* m_auiBook;
    wxPanel* m_panelKDE;
    wxStaticText* m_staticText59;
    wxTextCtrl* m_textCtrlframesToLearn;
    wxStaticText* m_staticText63;
    wxTextCtrl* m_textCtrlSequenceLen;
    wxStaticText* m_staticText67;
    wxTextCtrl* m_textCtrlTimeWinSize;
    wxStaticText* m_staticText71;
    wxCheckBox* m_checkBoxSDEstFlag;
    wxStaticText* m_staticText75;
    wxCheckBox* m_checkBoxlUseColorRatiosFlag;
    wxStaticText* m_staticText79;
    wxTextCtrl* m_textCtrlThreshold;
    wxStaticText* m_staticText85;
    wxTextCtrl* m_textCtrlAlpha;
    wxPanel* m_panelMoGv2;
    wxPanel* m_panelAlgo;
    wxListBox* m_listBoxBGS;
    wxPanel* m_panelGlobalSet;
    wxStaticText* m_staticText110;
    wxTextCtrl* m_textCtrlFrameWait;
    wxStaticText* m_staticText114;
    wxTextCtrl* m_textCtrlStartFrame;
    wxStaticText* m_staticText122;
    wxTextCtrl* m_textCtrlSampling;
    wxStaticText* m_staticText118;
    wxCheckBox* m_checkBoxShowPreprocess;
    wxRadioButton* m_radioButtonLeftSide;
    wxRadioButton* m_radioButtonRightSide;
    wxPanel* m_panelMsg;
    wxRichTextCtrl* m_richTextMsg;

protected:
    virtual void OnFileOpen(wxCommandEvent& event) { event.Skip(); }
    virtual void OnExit(wxCommandEvent& event) { event.Skip(); }
    virtual void OnViewMsgPane(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoBGSProcess(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoFrameProcessor(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoExtractFrames(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoFGPixels(wxCommandEvent& event) { event.Skip(); }
    virtual void OnBackgroundKDE(wxCommandEvent& event) { event.Skip(); }
    virtual void OnAbout(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoStop(wxCommandEvent& event) { event.Skip(); }
    virtual void OnBookPageChanged(wxAuiNotebookEvent& event) { event.Skip(); }

public:
    wxMenuBar* GetMenuBar() { return m_menuBar; }
    wxStatusBar* GetStatusBar() { return m_statusBar; }
    wxAuiToolBar* GetAuibar23() { return m_auibar23; }
    wxStaticText* GetStaticText59() { return m_staticText59; }
    wxTextCtrl* GetTextCtrlframesToLearn() { return m_textCtrlframesToLearn; }
    wxStaticText* GetStaticText63() { return m_staticText63; }
    wxTextCtrl* GetTextCtrlSequenceLen() { return m_textCtrlSequenceLen; }
    wxStaticText* GetStaticText67() { return m_staticText67; }
    wxTextCtrl* GetTextCtrlTimeWinSize() { return m_textCtrlTimeWinSize; }
    wxStaticText* GetStaticText71() { return m_staticText71; }
    wxCheckBox* GetCheckBoxSDEstFlag() { return m_checkBoxSDEstFlag; }
    wxStaticText* GetStaticText75() { return m_staticText75; }
    wxCheckBox* GetCheckBoxlUseColorRatiosFlag() { return m_checkBoxlUseColorRatiosFlag; }
    wxStaticText* GetStaticText79() { return m_staticText79; }
    wxTextCtrl* GetTextCtrlThreshold() { return m_textCtrlThreshold; }
    wxStaticText* GetStaticText85() { return m_staticText85; }
    wxTextCtrl* GetTextCtrlAlpha() { return m_textCtrlAlpha; }
    wxPanel* GetPanelKDE() { return m_panelKDE; }
    wxPanel* GetPanelMoGv2() { return m_panelMoGv2; }
    wxAuiNotebook* GetAuiBook() { return m_auiBook; }
    wxListBox* GetListBoxBGS() { return m_listBoxBGS; }
    wxPanel* GetPanelAlgo() { return m_panelAlgo; }
    wxStaticText* GetStaticText110() { return m_staticText110; }
    wxTextCtrl* GetTextCtrlFrameWait() { return m_textCtrlFrameWait; }
    wxStaticText* GetStaticText114() { return m_staticText114; }
    wxTextCtrl* GetTextCtrlStartFrame() { return m_textCtrlStartFrame; }
    wxStaticText* GetStaticText122() { return m_staticText122; }
    wxTextCtrl* GetTextCtrlSampling() { return m_textCtrlSampling; }
    wxStaticText* GetStaticText118() { return m_staticText118; }
    wxCheckBox* GetCheckBoxShowPreprocess() { return m_checkBoxShowPreprocess; }
    wxRadioButton* GetRadioButtonLeftSide() { return m_radioButtonLeftSide; }
    wxRadioButton* GetRadioButtonRightSide() { return m_radioButtonRightSide; }
    wxPanel* GetPanelGlobalSet() { return m_panelGlobalSet; }
    wxRichTextCtrl* GetRichTextMsg() { return m_richTextMsg; }
    wxPanel* GetPanelMsg() { return m_panelMsg; }
    wxAuiManager* GetAuimgr21() { return m_auimgr21; }
    MainFrameBaseClass(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("RatVideo"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(700,550), long style = wxCAPTION|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLOSE_BOX);
    virtual ~MainFrameBaseClass();
};

#endif
