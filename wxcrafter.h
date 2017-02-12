//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _RATVIDEO_WXCRAFTER_BASE_CLASSES_H
#define _RATVIDEO_WXCRAFTER_BASE_CLASSES_H

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
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/radiobox.h>
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
        wxID_BGKDE = 10001,
        wxID_EXTRACT_FRAME = 10002,
        wxID_FRAME_PROCESSOR = 10003,
        wxID_PAUSE = 10004,
        wxID_PROFILE_SMOOTH = 10005,
    };
protected:
    wxMenuBar* m_menuBar;
    wxMenu* m_menuFile;
    wxMenuItem* m_menuItemFileOpen;
    wxMenuItem* m_menuItemExit;
    wxMenu* m_menuView;
    wxMenuItem* m_menuItemViewMsgPane;
    wxMenuItem* m_menuItemViewShowProfile;
    wxMenuItem* m_menuItemViewShowFrameType;
    wxMenu* m_menuVideo;
    wxMenuItem* m_menuItemVideoFrameProcessor;
    wxMenuItem* m_menuItemVideoExtractFrame;
    wxMenuItem* m_menuItem138;
    wxMenuItem* m_menuItemVideoFGPixels;
    wxMenuItem* m_menuItem231;
    wxMenu* m_menuBackground;
    wxMenuItem* m_menuItemBgKDE;
    wxMenu* m_menuProfile;
    wxMenuItem* m_menuItemProfileGaussianSmooth;
    wxMenuItem* m_menuItemProfileClassification;
    wxMenuItem* m_menuItem253;
    wxMenuItem* m_menuItemProfileCentroid;
    wxMenuItem* m_menuItemTrainData;
    wxMenuItem* m_menuItemTestResult;
    wxMenuItem* m_menuItem282;
    wxMenuItem* m_menuItemPredictedResult;
    wxMenu* m_nameHelp;
    wxMenuItem* m_menuItem9;
    wxStatusBar* m_statusBar;
    wxAuiManager* m_auimgr21;
    wxAuiToolBar* m_auibar23;
    wxAuiNotebook* m_auiBook;
    wxPanel* m_panelProfile;
    wxStaticText* m_staticText205;
    wxTextCtrl* m_textCtrlDataPath;
    wxButton* m_buttonBrowse;
    wxStaticText* m_staticText154;
    wxTextCtrl* m_textCtrlGausKSize;
    wxStaticText* m_staticText179;
    wxTextCtrl* m_textCtrlProfileTh;
    wxStaticText* m_staticText183;
    wxTextCtrl* m_textCtrlMinSilence;
    wxStaticText* m_staticText193;
    wxTextCtrl* m_textCtrlMinActive;
    wxStaticText* m_staticText211;
    wxTextCtrl* m_textCtrlGnuplotW;
    wxStaticText* m_staticText215;
    wxTextCtrl* m_textCtrlGnuplotH;
    wxStaticText* m_staticText197;
    wxRadioButton* m_radioButtonClassWMM;
    wxRadioButton* m_radioButtonClassFD;
    wxStaticText* m_staticText167;
    wxTextCtrl* m_textCtrlRangeXMin;
    wxTextCtrl* m_textCtrlRangeXMax;
    wxStaticText* m_staticText173;
    wxTextCtrl* m_textCtrlRangeYMin;
    wxTextCtrl* m_textCtrlRangeYMax;
    wxStaticText* m_staticText187;
    wxTextCtrl* m_textCtrlFrameNo;
    wxTextCtrl* m_textCtrlMMSS;
    wxPanel* m_panelKDE;
    wxStaticText* m_staticText223;
    wxTextCtrl* m_textCtrlKernelBW;
    wxStaticText* m_staticText227;
    wxTextCtrl* m_textCtrlFgProb;
    wxStaticText* m_staticText276;
    wxCheckBox* m_checkBoxEnableSnake;
    wxStaticText* m_staticText243;
    wxTextCtrl* m_textCtrlLambdaOut;
    wxStaticText* m_staticText247;
    wxTextCtrl* m_textCtrlLambdaIn;
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
    wxPanel* m_panel270;
    wxRadioBox* m_radioBoxWholeRegion;
    wxPanel* m_panelMsg;
    wxRichTextCtrl* m_richTextMsg;

protected:
    virtual void OnFileOpen(wxCommandEvent& event) { event.Skip(); }
    virtual void OnExit(wxCommandEvent& event) { event.Skip(); }
    virtual void OnViewMsgPane(wxCommandEvent& event) { event.Skip(); }
    virtual void OnViewShowProfile(wxCommandEvent& event) { event.Skip(); }
    virtual void OnViewShowFrameType(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoFrameProcessor(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoExtractFrames(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoFGPixels(wxCommandEvent& event) { event.Skip(); }
    virtual void OnBackgroundKDE(wxCommandEvent& event) { event.Skip(); }
    virtual void OnProfileGaussianSmooth(wxCommandEvent& event) { event.Skip(); }
    virtual void OnProfileClassification(wxCommandEvent& event) { event.Skip(); }
    virtual void OnProfileCentroid(wxCommandEvent& event) { event.Skip(); }
    virtual void OnTrainData(wxCommandEvent& event) { event.Skip(); }
    virtual void OnShowTestResult(wxCommandEvent& event) { event.Skip(); }
    virtual void OnShowPredictedResult(wxCommandEvent& event) { event.Skip(); }
    virtual void OnAbout(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoStop(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoPause(wxCommandEvent& event) { event.Skip(); }
    virtual void OnBookPageChanged(wxAuiNotebookEvent& event) { event.Skip(); }
    virtual void OnBrowseDataPath(wxCommandEvent& event) { event.Skip(); }
    virtual void OnTextFrameNoEnter(wxCommandEvent& event) { event.Skip(); }
    virtual void OnTextMMSSEnter(wxCommandEvent& event) { event.Skip(); }

public:
    wxMenuBar* GetMenuBar() { return m_menuBar; }
    wxStatusBar* GetStatusBar() { return m_statusBar; }
    wxAuiToolBar* GetAuibar23() { return m_auibar23; }
    wxStaticText* GetStaticText205() { return m_staticText205; }
    wxTextCtrl* GetTextCtrlDataPath() { return m_textCtrlDataPath; }
    wxButton* GetButtonBrowse() { return m_buttonBrowse; }
    wxStaticText* GetStaticText154() { return m_staticText154; }
    wxTextCtrl* GetTextCtrlGausKSize() { return m_textCtrlGausKSize; }
    wxStaticText* GetStaticText179() { return m_staticText179; }
    wxTextCtrl* GetTextCtrlProfileTh() { return m_textCtrlProfileTh; }
    wxStaticText* GetStaticText183() { return m_staticText183; }
    wxTextCtrl* GetTextCtrlMinSilence() { return m_textCtrlMinSilence; }
    wxStaticText* GetStaticText193() { return m_staticText193; }
    wxTextCtrl* GetTextCtrlMinActive() { return m_textCtrlMinActive; }
    wxStaticText* GetStaticText211() { return m_staticText211; }
    wxTextCtrl* GetTextCtrlGnuplotW() { return m_textCtrlGnuplotW; }
    wxStaticText* GetStaticText215() { return m_staticText215; }
    wxTextCtrl* GetTextCtrlGnuplotH() { return m_textCtrlGnuplotH; }
    wxStaticText* GetStaticText197() { return m_staticText197; }
    wxRadioButton* GetRadioButtonClassWMM() { return m_radioButtonClassWMM; }
    wxRadioButton* GetRadioButtonClassFD() { return m_radioButtonClassFD; }
    wxStaticText* GetStaticText167() { return m_staticText167; }
    wxTextCtrl* GetTextCtrlRangeXMin() { return m_textCtrlRangeXMin; }
    wxTextCtrl* GetTextCtrlRangeXMax() { return m_textCtrlRangeXMax; }
    wxStaticText* GetStaticText173() { return m_staticText173; }
    wxTextCtrl* GetTextCtrlRangeYMin() { return m_textCtrlRangeYMin; }
    wxTextCtrl* GetTextCtrlRangeYMax() { return m_textCtrlRangeYMax; }
    wxStaticText* GetStaticText187() { return m_staticText187; }
    wxTextCtrl* GetTextCtrlFrameNo() { return m_textCtrlFrameNo; }
    wxTextCtrl* GetTextCtrlMMSS() { return m_textCtrlMMSS; }
    wxPanel* GetPanelProfile() { return m_panelProfile; }
    wxStaticText* GetStaticText223() { return m_staticText223; }
    wxTextCtrl* GetTextCtrlKernelBW() { return m_textCtrlKernelBW; }
    wxStaticText* GetStaticText227() { return m_staticText227; }
    wxTextCtrl* GetTextCtrlFgProb() { return m_textCtrlFgProb; }
    wxStaticText* GetStaticText276() { return m_staticText276; }
    wxCheckBox* GetCheckBoxEnableSnake() { return m_checkBoxEnableSnake; }
    wxStaticText* GetStaticText243() { return m_staticText243; }
    wxTextCtrl* GetTextCtrlLambdaOut() { return m_textCtrlLambdaOut; }
    wxStaticText* GetStaticText247() { return m_staticText247; }
    wxTextCtrl* GetTextCtrlLambdaIn() { return m_textCtrlLambdaIn; }
    wxPanel* GetPanelKDE() { return m_panelKDE; }
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
    wxRadioBox* GetRadioBoxWholeRegion() { return m_radioBoxWholeRegion; }
    wxPanel* GetPanel270() { return m_panel270; }
    wxPanel* GetPanelGlobalSet() { return m_panelGlobalSet; }
    wxRichTextCtrl* GetRichTextMsg() { return m_richTextMsg; }
    wxPanel* GetPanelMsg() { return m_panelMsg; }
    wxAuiManager* GetAuimgr21() { return m_auimgr21; }
    MainFrameBaseClass(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("RatVideo"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(750,650), long style = wxCAPTION|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLOSE_BOX);
    virtual ~MainFrameBaseClass();
};

#endif
