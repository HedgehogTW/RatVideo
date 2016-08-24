//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxcrafter.h"


// Declare the bitmap loading function
extern void wxC9ED9InitBitmapResources();

static bool bBitmapLoaded = false;


MainFrameBaseClass::MainFrameBaseClass(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    m_menuBar = new wxMenuBar(0);
    this->SetMenuBar(m_menuBar);
    
    m_menuFile = new wxMenu();
    m_menuBar->Append(m_menuFile, _("File"));
    
    m_menuItemFileOpen = new wxMenuItem(m_menuFile, wxID_OPEN, _("Open"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemFileOpen);
    
    m_menuItemExit = new wxMenuItem(m_menuFile, wxID_EXIT, _("Exit\tAlt-X"), _("Quit"), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemExit);
    
    m_menuView = new wxMenu();
    m_menuBar->Append(m_menuView, _("View"));
    
    m_menuItemViewMsgPane = new wxMenuItem(m_menuView, wxID_ANY, _("Show Message Pane"), wxT(""), wxITEM_CHECK);
    m_menuView->Append(m_menuItemViewMsgPane);
    m_menuItemViewMsgPane->Check();
    
    m_menuItemViewShowProfile = new wxMenuItem(m_menuView, wxID_ANY, _("Show Profile"), wxT(""), wxITEM_NORMAL);
    m_menuView->Append(m_menuItemViewShowProfile);
    
    m_menuItemViewShowFrameType = new wxMenuItem(m_menuView, wxID_ANY, _("Show Frame Type"), wxT(""), wxITEM_NORMAL);
    m_menuView->Append(m_menuItemViewShowFrameType);
    
    m_menuVideo = new wxMenu();
    m_menuBar->Append(m_menuVideo, _("Video"));
    
    m_menuItemVideoBGSProcess = new wxMenuItem(m_menuVideo, wxID_BGS_PROCESS, _("BGS Process"), wxT(""), wxITEM_NORMAL);
    m_menuVideo->Append(m_menuItemVideoBGSProcess);
    
    m_menuItemVideoFrameProcessor = new wxMenuItem(m_menuVideo, wxID_FRAME_PROCESSOR, _("FrameProcessor"), wxT(""), wxITEM_NORMAL);
    m_menuVideo->Append(m_menuItemVideoFrameProcessor);
    
    m_menuItemVideoExtractFrame = new wxMenuItem(m_menuVideo, wxID_EXTRACT_FRAME, _("Extract Frames"), wxT(""), wxITEM_NORMAL);
    m_menuVideo->Append(m_menuItemVideoExtractFrame);
    
    m_menuVideo->AppendSeparator();
    
    m_menuItemVideoFGPixels = new wxMenuItem(m_menuVideo, wxID_ANY, _("FG Pixel Profile"), wxT(""), wxITEM_NORMAL);
    m_menuVideo->Append(m_menuItemVideoFGPixels);
    
    m_menuBackground = new wxMenu();
    m_menuBar->Append(m_menuBackground, _("Background"));
    
    m_menuItemBgKDE = new wxMenuItem(m_menuBackground, wxID_ANY, _("KDE"), wxT(""), wxITEM_NORMAL);
    m_menuBackground->Append(m_menuItemBgKDE);
    
    m_menuProfile = new wxMenu();
    m_menuBar->Append(m_menuProfile, _("Profile"));
    
    m_menuItemProfileGaussianSmooth = new wxMenuItem(m_menuProfile, wxID_ANY, _("Gaussian Smooth"), wxT(""), wxITEM_NORMAL);
    m_menuProfile->Append(m_menuItemProfileGaussianSmooth);
    
    m_menuItemProfileClassification = new wxMenuItem(m_menuProfile, wxID_ANY, _("Profile classification"), wxT(""), wxITEM_NORMAL);
    m_menuProfile->Append(m_menuItemProfileClassification);
    
    m_nameHelp = new wxMenu();
    m_menuBar->Append(m_nameHelp, _("Help"));
    
    m_menuItem9 = new wxMenuItem(m_nameHelp, wxID_ABOUT, _("About..."), wxT(""), wxITEM_NORMAL);
    m_nameHelp->Append(m_menuItem9);
    
    m_statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE);
    m_statusBar->SetFieldsCount(1);
    this->SetStatusBar(m_statusBar);
    
    m_auimgr21 = new wxAuiManager;
    m_auimgr21->SetManagedWindow( this );
    m_auimgr21->SetFlags( wxAUI_MGR_LIVE_RESIZE|wxAUI_MGR_TRANSPARENT_HINT|wxAUI_MGR_TRANSPARENT_DRAG|wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_ALLOW_FLOATING);
    m_auimgr21->GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_NONE);
    
    m_auibar23 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxAUI_TB_PLAIN_BACKGROUND|wxAUI_TB_DEFAULT_STYLE);
    m_auibar23->SetToolBitmapSize(wxSize(32,32));
    
    m_auimgr21->AddPane(m_auibar23, wxAuiPaneInfo().Direction(wxAUI_DOCK_TOP).Layer(0).Row(0).Position(0).BestSize(42,42).MinSize(42,42).CaptionVisible(true).MaximizeButton(false).CloseButton(false).MinimizeButton(false).PinButton(false).ToolbarPane());
    
    m_auibar23->AddTool(wxID_OPEN, _("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, wxDefaultSize), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    
    m_auibar23->AddTool(wxID_BGS_PROCESS, _("BGS Process"), wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR, wxDefaultSize), wxNullBitmap, wxITEM_NORMAL, _("BGS Process"), wxT(""), NULL);
    
    m_auibar23->AddTool(wxID_FRAME_PROCESSOR, _("FrameProcessor"), wxXmlResource::Get()->LoadBitmap(wxT("pokeball")), wxNullBitmap, wxITEM_NORMAL, _("FrameProcessor"), wxT(""), NULL);
    
    m_auibar23->AddTool(wxID_STOP, _("Stop"), wxXmlResource::Get()->LoadBitmap(wxT("error")), wxNullBitmap, wxITEM_NORMAL, _("Stop"), wxT(""), NULL);
    
    m_auibar23->AddTool(wxID_EXTRACT_FRAME, _("Extract Frames"), wxXmlResource::Get()->LoadBitmap(wxT("download")), wxNullBitmap, wxITEM_NORMAL, _("Extract Frames"), wxT(""), NULL);
    m_auibar23->Realize();
    
    m_auiBook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(250,250)), wxAUI_NB_TAB_FIXED_WIDTH|wxBK_DEFAULT);
    m_auiBook->SetName(wxT("m_auiBook"));
    
    m_auimgr21->AddPane(m_auiBook, wxAuiPaneInfo().Direction(wxAUI_DOCK_CENTER).Layer(0).Row(0).Position(0).BestSize(100,100).MinSize(100,100).MaxSize(100,100).CaptionVisible(false).MaximizeButton(false).CloseButton(false).MinimizeButton(false).PinButton(false));
    
    m_panelProfile = new wxPanel(m_auiBook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_auiBook, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_auiBook->AddPage(m_panelProfile, _("Profile"), true);
    
    wxFlexGridSizer* flexGridSizer152 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer152->SetFlexibleDirection( wxBOTH );
    flexGridSizer152->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    m_panelProfile->SetSizer(flexGridSizer152);
    
    m_staticText154 = new wxStaticText(m_panelProfile, wxID_ANY, _("Gaussian kSize"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer152->Add(m_staticText154, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlGausKSize = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("15"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlGausKSize->SetHint(wxT(""));
    #endif
    
    flexGridSizer152->Add(m_textCtrlGausKSize, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelKDE = new wxPanel(m_auiBook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_auiBook, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_auiBook->AddPage(m_panelKDE, _("KDE"), false);
    
    wxFlexGridSizer* flexGridSizer92 = new wxFlexGridSizer(0, 2, 0, 10);
    flexGridSizer92->SetFlexibleDirection( wxBOTH );
    flexGridSizer92->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    m_panelKDE->SetSizer(flexGridSizer92);
    
    m_staticText59 = new wxStaticText(m_panelKDE, wxID_ANY, _("frames To Learn"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer92->Add(m_staticText59, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlframesToLearn = new wxTextCtrl(m_panelKDE, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlframesToLearn->SetHint(wxT(""));
    #endif
    
    flexGridSizer92->Add(m_textCtrlframesToLearn, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText63 = new wxStaticText(m_panelKDE, wxID_ANY, _("Sequence Length"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer92->Add(m_staticText63, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlSequenceLen = new wxTextCtrl(m_panelKDE, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlSequenceLen->SetHint(wxT(""));
    #endif
    
    flexGridSizer92->Add(m_textCtrlSequenceLen, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText67 = new wxStaticText(m_panelKDE, wxID_ANY, _("Time Window Size"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer92->Add(m_staticText67, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlTimeWinSize = new wxTextCtrl(m_panelKDE, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlTimeWinSize->SetHint(wxT(""));
    #endif
    
    flexGridSizer92->Add(m_textCtrlTimeWinSize, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText71 = new wxStaticText(m_panelKDE, wxID_ANY, _("SD Estimation Flag"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer92->Add(m_staticText71, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxSDEstFlag = new wxCheckBox(m_panelKDE, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    m_checkBoxSDEstFlag->SetValue(false);
    
    flexGridSizer92->Add(m_checkBoxSDEstFlag, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText75 = new wxStaticText(m_panelKDE, wxID_ANY, _("lUse Color Ratios Flag"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer92->Add(m_staticText75, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxlUseColorRatiosFlag = new wxCheckBox(m_panelKDE, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    m_checkBoxlUseColorRatiosFlag->SetValue(false);
    
    flexGridSizer92->Add(m_checkBoxlUseColorRatiosFlag, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText79 = new wxStaticText(m_panelKDE, wxID_ANY, _("threshold"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer92->Add(m_staticText79, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlThreshold = new wxTextCtrl(m_panelKDE, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlThreshold->SetHint(wxT(""));
    #endif
    
    flexGridSizer92->Add(m_textCtrlThreshold, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText85 = new wxStaticText(m_panelKDE, wxID_ANY, _("alpha"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer92->Add(m_staticText85, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlAlpha = new wxTextCtrl(m_panelKDE, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlAlpha->SetHint(wxT(""));
    #endif
    
    flexGridSizer92->Add(m_textCtrlAlpha, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelAlgo = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    wxFont m_panelAlgoFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    m_panelAlgo->SetFont(m_panelAlgoFont);
    
    m_auimgr21->AddPane(m_panelAlgo, wxAuiPaneInfo().Caption(_("Algorithm")).Direction(wxAUI_DOCK_RIGHT).Layer(0).Row(0).Position(0).BestSize(220,100).MinSize(200,100).MaxSize(200,100).CaptionVisible(true).MaximizeButton(false).CloseButton(false).MinimizeButton(false).PinButton(false));
    
    wxBoxSizer* boxSizer53 = new wxBoxSizer(wxVERTICAL);
    m_panelAlgo->SetSizer(boxSizer53);
    
    wxArrayString m_listBoxBGSArr;
    m_listBoxBGSArr.Add(_("FrameDifferenceBGS"));
    m_listBoxBGSArr.Add(_("StaticFrameDifferenceBGS"));
    m_listBoxBGSArr.Add(_("WeightedMovingMeanBGS"));
    m_listBoxBGSArr.Add(_("WeightedMovingVarianceBGS"));
    m_listBoxBGSArr.Add(_("MixtureOfGaussianV2BGS"));
    m_listBoxBGSArr.Add(_("AdaptiveBackgroundLearning"));
    m_listBoxBGSArr.Add(_("DPAdaptiveMedianBGS"));
    m_listBoxBGSArr.Add(_("DPGrimsonGMMBGS"));
    m_listBoxBGSArr.Add(_("DPZivkovicAGMMBGS"));
    m_listBoxBGSArr.Add(_("DPMeanBGS"));
    m_listBoxBGSArr.Add(_("DPWrenGABGS"));
    m_listBoxBGSArr.Add(_("DPPratiMediodBGS"));
    m_listBoxBGSArr.Add(_("DPEigenbackgroundBGS"));
    m_listBoxBGSArr.Add(_("DPTextureBGS"));
    m_listBoxBGSArr.Add(_("T2FGMM_UM"));
    m_listBoxBGSArr.Add(_("T2FGMM_UV"));
    m_listBoxBGSArr.Add(_("T2FMRF_UM"));
    m_listBoxBGSArr.Add(_("T2FMRF_UV"));
    m_listBoxBGSArr.Add(_("FuzzySugenoIntegral"));
    m_listBoxBGSArr.Add(_("FuzzyChoquetIntegral"));
    m_listBoxBGSArr.Add(_("LBSimpleGaussian"));
    m_listBoxBGSArr.Add(_("LBFuzzyGaussian"));
    m_listBoxBGSArr.Add(_("LBMixtureOfGaussians"));
    m_listBoxBGSArr.Add(_("LBAdaptiveSOM"));
    m_listBoxBGSArr.Add(_("LBFuzzyAdaptiveSOM"));
    m_listBoxBGSArr.Add(_("LbpMrf"));
    m_listBoxBGSArr.Add(_("VuMeter"));
    m_listBoxBGSArr.Add(_("KDE"));
    m_listBoxBGSArr.Add(_("IndependentMultimodalBGS"));
    m_listBoxBGSArr.Add(_("MultiCueBGS"));
    m_listBoxBGSArr.Add(_("SigmaDeltaBGS"));
    m_listBoxBGSArr.Add(_("SuBSENSEBGS"));
    m_listBoxBGSArr.Add(_("LOBSTERBGS"));
    m_listBoxBGS = new wxListBox(m_panelAlgo, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelAlgo, wxSize(-1,220)), m_listBoxBGSArr, wxLB_SINGLE);
    
    boxSizer53->Add(m_listBoxBGS, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelGlobalSet = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(50,-1)), wxTAB_TRAVERSAL);
    
    m_auimgr21->AddPane(m_panelGlobalSet, wxAuiPaneInfo().Caption(_("Global Setting")).Direction(wxAUI_DOCK_BOTTOM).Layer(0).Row(0).Position(0).BestSize(50,100).MinSize(50,100).MaxSize(50,100).CaptionVisible(true).MaximizeButton(false).CloseButton(false).MinimizeButton(false).PinButton(false));
    
    wxGridSizer* gridSizer108 = new wxGridSizer(0, 2, 0, 0);
    m_panelGlobalSet->SetSizer(gridSizer108);
    
    m_staticText110 = new wxStaticText(m_panelGlobalSet, wxID_ANY, _("Frame wait (ms)"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    
    gridSizer108->Add(m_staticText110, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlFrameWait = new wxTextCtrl(m_panelGlobalSet, wxID_ANY, wxT("100"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlFrameWait->SetHint(wxT(""));
    #endif
    
    gridSizer108->Add(m_textCtrlFrameWait, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText114 = new wxStaticText(m_panelGlobalSet, wxID_ANY, _("Start frame"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    
    gridSizer108->Add(m_staticText114, 0, wxALL|wxALIGN_TOP, WXC_FROM_DIP(5));
    
    m_textCtrlStartFrame = new wxTextCtrl(m_panelGlobalSet, wxID_ANY, wxT("2"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlStartFrame->SetHint(wxT(""));
    #endif
    
    gridSizer108->Add(m_textCtrlStartFrame, 0, wxALL|wxALIGN_TOP, WXC_FROM_DIP(5));
    
    m_staticText122 = new wxStaticText(m_panelGlobalSet, wxID_ANY, _("Sampling rate"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    
    gridSizer108->Add(m_staticText122, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlSampling = new wxTextCtrl(m_panelGlobalSet, wxID_ANY, wxT("1"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlSampling->SetHint(wxT(""));
    #endif
    
    gridSizer108->Add(m_textCtrlSampling, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText118 = new wxStaticText(m_panelGlobalSet, wxID_ANY, _("Show preprocess"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    
    gridSizer108->Add(m_staticText118, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxShowPreprocess = new wxCheckBox(m_panelGlobalSet, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    m_checkBoxShowPreprocess->SetValue(false);
    
    gridSizer108->Add(m_checkBoxShowPreprocess, 0, wxALL, WXC_FROM_DIP(5));
    
    m_radioButtonLeftSide = new wxRadioButton(m_panelGlobalSet, wxID_ANY, _("Left side"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), wxRB_GROUP);
    m_radioButtonLeftSide->SetValue(1);
    
    gridSizer108->Add(m_radioButtonLeftSide, 0, wxALL, WXC_FROM_DIP(5));
    
    m_radioButtonRightSide = new wxRadioButton(m_panelGlobalSet, wxID_ANY, _("Right side"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    m_radioButtonRightSide->SetValue(1);
    
    gridSizer108->Add(m_radioButtonRightSide, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelMsg = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    wxFont m_panelMsgFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Calibri"));
    m_panelMsg->SetFont(m_panelMsgFont);
    
    m_auimgr21->AddPane(m_panelMsg, wxAuiPaneInfo().Name(wxT("MsgPane")).Caption(_("Message Pane")).Direction(wxAUI_DOCK_BOTTOM).Layer(0).Row(0).Position(0).BestSize(450,150).MinSize(450,150).MaxSize(450,300).CaptionVisible(true).MaximizeButton(false).CloseButton(true).MinimizeButton(false).PinButton(false));
    m_auimgr21->Update();
    
    wxBoxSizer* boxSizer27 = new wxBoxSizer(wxVERTICAL);
    m_panelMsg->SetSizer(boxSizer27);
    
    m_richTextMsg = new wxRichTextCtrl(m_panelMsg, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelMsg, wxSize(-1,-1)), wxTE_MULTILINE|wxTE_PROCESS_TAB|wxTE_PROCESS_ENTER|wxWANTS_CHARS);
    wxFont m_richTextMsgFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Calibri"));
    m_richTextMsg->SetFont(m_richTextMsgFont);
    
    boxSizer27->Add(m_richTextMsg, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_auiBook)){
        wxPersistenceManager::Get().RegisterAndRestore(m_auiBook);
    } else {
        wxPersistenceManager::Get().Restore(m_auiBook);
    }
    #endif
    
    SetName(wxT("MainFrameBaseClass"));
    SetSize(700,550);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    this->Connect(m_menuItemFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Connect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Connect(m_menuItemViewMsgPane->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewMsgPane), NULL, this);
    this->Connect(m_menuItemViewShowProfile->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowProfile), NULL, this);
    this->Connect(m_menuItemViewShowFrameType->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowFrameType), NULL, this);
    this->Connect(m_menuItemVideoBGSProcess->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoBGSProcess), NULL, this);
    this->Connect(m_menuItemVideoFrameProcessor->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFrameProcessor), NULL, this);
    this->Connect(m_menuItemVideoExtractFrame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoExtractFrames), NULL, this);
    this->Connect(m_menuItemVideoFGPixels->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFGPixels), NULL, this);
    this->Connect(m_menuItemBgKDE->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnBackgroundKDE), NULL, this);
    this->Connect(m_menuItemProfileGaussianSmooth->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileGaussianSmooth), NULL, this);
    this->Connect(m_menuItemProfileClassification->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileClassification), NULL, this);
    this->Connect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    this->Connect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Connect(wxID_BGS_PROCESS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoBGSProcess), NULL, this);
    this->Connect(wxID_FRAME_PROCESSOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFrameProcessor), NULL, this);
    this->Connect(wxID_STOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoStop), NULL, this);
    this->Connect(wxID_EXTRACT_FRAME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoExtractFrames), NULL, this);
    m_auiBook->Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(MainFrameBaseClass::OnBookPageChanged), NULL, this);
    
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    this->Disconnect(m_menuItemFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Disconnect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Disconnect(m_menuItemViewMsgPane->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewMsgPane), NULL, this);
    this->Disconnect(m_menuItemViewShowProfile->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowProfile), NULL, this);
    this->Disconnect(m_menuItemViewShowFrameType->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowFrameType), NULL, this);
    this->Disconnect(m_menuItemVideoBGSProcess->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoBGSProcess), NULL, this);
    this->Disconnect(m_menuItemVideoFrameProcessor->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFrameProcessor), NULL, this);
    this->Disconnect(m_menuItemVideoExtractFrame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoExtractFrames), NULL, this);
    this->Disconnect(m_menuItemVideoFGPixels->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFGPixels), NULL, this);
    this->Disconnect(m_menuItemBgKDE->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnBackgroundKDE), NULL, this);
    this->Disconnect(m_menuItemProfileGaussianSmooth->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileGaussianSmooth), NULL, this);
    this->Disconnect(m_menuItemProfileClassification->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileClassification), NULL, this);
    this->Disconnect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    this->Disconnect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Disconnect(wxID_BGS_PROCESS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoBGSProcess), NULL, this);
    this->Disconnect(wxID_FRAME_PROCESSOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFrameProcessor), NULL, this);
    this->Disconnect(wxID_STOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoStop), NULL, this);
    this->Disconnect(wxID_EXTRACT_FRAME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoExtractFrames), NULL, this);
    m_auiBook->Disconnect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(MainFrameBaseClass::OnBookPageChanged), NULL, this);
    
    m_auimgr21->UnInit();
    delete m_auimgr21;

}
