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
    
    m_menuItemFileOpen = new wxMenuItem(m_menuFile, wxID_OPEN, _("Open Video"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemFileOpen);
    
    m_menuItemLoadProfile = new wxMenuItem(m_menuFile, wxID_ANY, _("Load Profile"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemLoadProfile);
    
    m_menuItemExit = new wxMenuItem(m_menuFile, wxID_EXIT, _("Exit\tAlt-X"), _("Quit"), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemExit);
    
    m_menuView = new wxMenu();
    m_menuBar->Append(m_menuView, _("View"));
    
    m_menuItemViewMsgPane = new wxMenuItem(m_menuView, wxID_ANY, _("Show Message Pane"), wxT(""), wxITEM_CHECK);
    m_menuView->Append(m_menuItemViewMsgPane);
    m_menuItemViewMsgPane->Check();
    
    m_menuItemViewShowProfile = new wxMenuItem(m_menuView, wxID_SHOW_PROFILE, _("Show Profile"), wxT(""), wxITEM_NORMAL);
    m_menuView->Append(m_menuItemViewShowProfile);
    
    m_menuItemViewShowFrameType = new wxMenuItem(m_menuView, wxID_ANY, _("Show Frame Type"), wxT(""), wxITEM_NORMAL);
    m_menuView->Append(m_menuItemViewShowFrameType);
    
    m_menuVideo = new wxMenu();
    m_menuBar->Append(m_menuVideo, _("Video"));
    
    m_menuItemVideoFrameProcessor = new wxMenuItem(m_menuVideo, wxID_FRAME_PROCESSOR, _("Background subtraction"), _("Background subtraction"), wxITEM_NORMAL);
    m_menuVideo->Append(m_menuItemVideoFrameProcessor);
    
    m_menuItemVideoExtractFrame = new wxMenuItem(m_menuVideo, wxID_EXTRACT_FRAME, _("Extract Frames"), wxT(""), wxITEM_NORMAL);
    m_menuVideo->Append(m_menuItemVideoExtractFrame);
    
    m_menuVideo->AppendSeparator();
    
    m_menuItemVideoFGPixels = new wxMenuItem(m_menuVideo, wxID_ANY, _("1. FG Pixel Profile"), wxT(""), wxITEM_NORMAL);
    m_menuVideo->Append(m_menuItemVideoFGPixels);
    
    m_menuVideo->AppendSeparator();
    
    m_menuBackground = new wxMenu();
    m_menuBar->Append(m_menuBackground, _("Background"));
    
    m_menuItemBgKDE = new wxMenuItem(m_menuBackground, wxID_BGKDE, _("2. KDE"), wxT(""), wxITEM_NORMAL);
    m_menuBackground->Append(m_menuItemBgKDE);
    
    m_menuProfile = new wxMenu();
    m_menuBar->Append(m_menuProfile, _("Profile"));
    
    m_menuItemProfileGaussianSmooth = new wxMenuItem(m_menuProfile, wxID_PROFILE_SMOOTH, _("Gaussian Smooth"), wxT(""), wxITEM_NORMAL);
    m_menuProfile->Append(m_menuItemProfileGaussianSmooth);
    
    m_menuItemProfileClassification = new wxMenuItem(m_menuProfile, wxID_ANY, _("Profile classification"), wxT(""), wxITEM_NORMAL);
    m_menuProfile->Append(m_menuItemProfileClassification);
    
    m_menuProfile->AppendSeparator();
    
    m_menuItemProfileCentroid = new wxMenuItem(m_menuProfile, wxID_ANY, _("Centroid"), wxT(""), wxITEM_NORMAL);
    m_menuProfile->Append(m_menuItemProfileCentroid);
    
    m_menuItemTrainData = new wxMenuItem(m_menuProfile, wxID_ANY, _("Generate TrainData"), wxT(""), wxITEM_NORMAL);
    m_menuProfile->Append(m_menuItemTrainData);
    
    m_menuItemTestResult = new wxMenuItem(m_menuProfile, wxID_ANY, _("Show Test Result"), wxT(""), wxITEM_NORMAL);
    m_menuProfile->Append(m_menuItemTestResult);
    
    m_menuProfile->AppendSeparator();
    
    m_menuItemPredictedResult = new wxMenuItem(m_menuProfile, wxID_ANY, _("Show Predicted Result"), wxT(""), wxITEM_NORMAL);
    m_menuProfile->Append(m_menuItemPredictedResult);
    
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
    
    m_auibar23->AddTool(wxID_SHOW_PROFILE, _("Show Profile"), wxXmlResource::Get()->LoadBitmap(wxT("analytics")), wxNullBitmap, wxITEM_NORMAL, _("Show Profile"), wxT(""), NULL);
    
    m_auibar23->AddSeparator();
    
    m_auibar23->AddTool(wxID_FRAME_PROCESSOR, _("FrameProcessor"), wxXmlResource::Get()->LoadBitmap(wxT("pokeball")), wxNullBitmap, wxITEM_NORMAL, _("FrameProcessor"), wxT(""), NULL);
    
    m_auibar23->AddTool(wxID_BGKDE, _("KDE Bg"), wxXmlResource::Get()->LoadBitmap(wxT("layer")), wxNullBitmap, wxITEM_NORMAL, _("KDE background"), wxT(""), NULL);
    
    m_auibar23->AddTool(wxID_STOP, _("Stop"), wxXmlResource::Get()->LoadBitmap(wxT("error")), wxNullBitmap, wxITEM_NORMAL, _("Stop"), wxT(""), NULL);
    
    m_auibar23->AddTool(wxID_PAUSE, _("Pause"), wxXmlResource::Get()->LoadBitmap(wxT("pause")), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    
    m_auibar23->AddTool(wxID_EXTRACT_FRAME, _("Extract Frames"), wxXmlResource::Get()->LoadBitmap(wxT("download")), wxNullBitmap, wxITEM_NORMAL, _("Extract Frames"), wxT(""), NULL);
    
    m_auibar23->AddTool(wxID_PROFILE_SMOOTH, _("Profile Gaussian Smooth"), wxXmlResource::Get()->LoadBitmap(wxT("smooth")), wxNullBitmap, wxITEM_NORMAL, _("Profile Gaussian Smooth"), wxT(""), NULL);
    m_auibar23->Realize();
    
    m_auiBook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(250,720)), wxAUI_NB_TAB_FIXED_WIDTH|wxBK_DEFAULT);
    m_auiBook->SetName(wxT("m_auiBook"));
    
    m_auimgr21->AddPane(m_auiBook, wxAuiPaneInfo().Direction(wxAUI_DOCK_CENTER).Layer(0).Row(0).Position(0).BestSize(250,200).MinSize(250,200).MaxSize(250,550).CaptionVisible(false).MaximizeButton(false).CloseButton(false).MinimizeButton(false).PinButton(false));
    
    m_panelProfile = new wxPanel(m_auiBook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_auiBook, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_auiBook->AddPage(m_panelProfile, _("Profile"), true);
    
    wxBoxSizer* boxSizer160 = new wxBoxSizer(wxVERTICAL);
    m_panelProfile->SetSizer(boxSizer160);
    
    wxBoxSizer* boxSizer203 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer160->Add(boxSizer203, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText205 = new wxStaticText(m_panelProfile, wxID_ANY, _("DataFile"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    boxSizer203->Add(m_staticText205, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlDataPath = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(150,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlDataPath->SetHint(wxT(""));
    #endif
    
    boxSizer203->Add(m_textCtrlDataPath, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_buttonBrowse = new wxButton(m_panelProfile, wxID_ANY, _("Browse"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    boxSizer203->Add(m_buttonBrowse, 0, wxALL, WXC_FROM_DIP(5));
    
    wxFlexGridSizer* flexGridSizer163 = new wxFlexGridSizer(3, 4, 0, 0);
    flexGridSizer163->SetFlexibleDirection( wxBOTH );
    flexGridSizer163->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    boxSizer160->Add(flexGridSizer163, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText154 = new wxStaticText(m_panelProfile, wxID_ANY, _("Gaussian kSize"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer163->Add(m_staticText154, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlGausKSize = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("120"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlGausKSize->SetHint(wxT(""));
    #endif
    
    flexGridSizer163->Add(m_textCtrlGausKSize, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText179 = new wxStaticText(m_panelProfile, wxID_ANY, _("Threshold"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer163->Add(m_staticText179, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlProfileTh = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("3"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlProfileTh->SetHint(wxT(""));
    #endif
    
    flexGridSizer163->Add(m_textCtrlProfileTh, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText183 = new wxStaticText(m_panelProfile, wxID_ANY, _("Min. silence"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer163->Add(m_staticText183, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlMinSilence = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("400"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlMinSilence->SetHint(wxT(""));
    #endif
    
    flexGridSizer163->Add(m_textCtrlMinSilence, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText193 = new wxStaticText(m_panelProfile, wxID_ANY, _("Min. Active"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer163->Add(m_staticText193, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlMinActive = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("70"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlMinActive->SetHint(wxT(""));
    #endif
    
    flexGridSizer163->Add(m_textCtrlMinActive, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText211 = new wxStaticText(m_panelProfile, wxID_ANY, _("gnuplot width"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer163->Add(m_staticText211, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlGnuplotW = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("1200"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlGnuplotW->SetHint(wxT(""));
    #endif
    
    flexGridSizer163->Add(m_textCtrlGnuplotW, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText215 = new wxStaticText(m_panelProfile, wxID_ANY, _("height"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer163->Add(m_staticText215, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlGnuplotH = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("300"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlGnuplotH->SetHint(wxT(""));
    #endif
    
    flexGridSizer163->Add(m_textCtrlGnuplotH, 0, wxALL, WXC_FROM_DIP(5));
    
    wxFlexGridSizer* flexGridSizer165 = new wxFlexGridSizer(5, 3, 0, 0);
    flexGridSizer165->SetFlexibleDirection( wxBOTH );
    flexGridSizer165->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    boxSizer160->Add(flexGridSizer165, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText197 = new wxStaticText(m_panelProfile, wxID_ANY, _("Classification"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer165->Add(m_staticText197, 0, wxALL, WXC_FROM_DIP(5));
    
    m_radioButtonClassWMM = new wxRadioButton(m_panelProfile, wxID_ANY, _("WMM"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), wxRB_GROUP);
    m_radioButtonClassWMM->SetValue(0);
    
    flexGridSizer165->Add(m_radioButtonClassWMM, 0, wxALL, WXC_FROM_DIP(5));
    
    m_radioButtonClassFD = new wxRadioButton(m_panelProfile, wxID_ANY, _("FD"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    m_radioButtonClassFD->SetValue(1);
    
    flexGridSizer165->Add(m_radioButtonClassFD, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText167 = new wxStaticText(m_panelProfile, wxID_ANY, _("x range"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer165->Add(m_staticText167, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlRangeXMin = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("0"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlRangeXMin->SetHint(wxT(""));
    #endif
    
    flexGridSizer165->Add(m_textCtrlRangeXMin, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlRangeXMax = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("5000"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlRangeXMax->SetHint(wxT(""));
    #endif
    
    flexGridSizer165->Add(m_textCtrlRangeXMax, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText173 = new wxStaticText(m_panelProfile, wxID_ANY, _("y range"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer165->Add(m_staticText173, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlRangeYMin = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("-200"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlRangeYMin->SetHint(wxT(""));
    #endif
    
    flexGridSizer165->Add(m_textCtrlRangeYMin, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlRangeYMax = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("2500"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlRangeYMax->SetHint(wxT(""));
    #endif
    
    flexGridSizer165->Add(m_textCtrlRangeYMax, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText187 = new wxStaticText(m_panelProfile, wxID_ANY, _("frame<->time"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    
    flexGridSizer165->Add(m_staticText187, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlFrameNo = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), wxTE_PROCESS_ENTER);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlFrameNo->SetHint(wxT(""));
    #endif
    
    flexGridSizer165->Add(m_textCtrlFrameNo, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlMMSS = new wxTextCtrl(m_panelProfile, wxID_ANY, wxT("14:55"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(50,-1)), wxTE_PROCESS_ENTER);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlMMSS->SetHint(wxT(""));
    #endif
    
    flexGridSizer165->Add(m_textCtrlMMSS, 0, wxALL, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer296 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer160->Add(boxSizer296, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_checkBoxSceneChangeBreak = new wxCheckBox(m_panelProfile, wxID_ANY, _("Scene change break"), wxDefaultPosition, wxDLG_UNIT(m_panelProfile, wxSize(-1,-1)), 0);
    m_checkBoxSceneChangeBreak->SetValue(false);
    
    boxSizer296->Add(m_checkBoxSceneChangeBreak, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelKDE = new wxPanel(m_auiBook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_auiBook, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_auiBook->AddPage(m_panelKDE, _("KDE"), false);
    
    wxBoxSizer* boxSizer219 = new wxBoxSizer(wxVERTICAL);
    m_panelKDE->SetSizer(boxSizer219);
    
    wxFlexGridSizer* flexGridSizer221 = new wxFlexGridSizer(0, 2, 0, 10);
    flexGridSizer221->SetFlexibleDirection( wxBOTH );
    flexGridSizer221->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    boxSizer219->Add(flexGridSizer221, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText223 = new wxStaticText(m_panelKDE, wxID_ANY, _("KernelBW"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer221->Add(m_staticText223, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlKernelBW = new wxTextCtrl(m_panelKDE, wxID_ANY, wxT("2"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlKernelBW->SetHint(wxT(""));
    #endif
    
    flexGridSizer221->Add(m_textCtrlKernelBW, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText227 = new wxStaticText(m_panelKDE, wxID_ANY, _("fg Prob"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer221->Add(m_staticText227, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlFgProb = new wxTextCtrl(m_panelKDE, wxID_ANY, wxT("0.0000001"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlFgProb->SetHint(wxT(""));
    #endif
    
    flexGridSizer221->Add(m_textCtrlFgProb, 0, wxALL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* staticBoxSizer239 = new wxStaticBoxSizer( new wxStaticBox(m_panelKDE, wxID_ANY, _("Snake Parameters")), wxVERTICAL);
    
    boxSizer219->Add(staticBoxSizer239, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxFlexGridSizer* flexGridSizer241 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer241->SetFlexibleDirection( wxBOTH );
    flexGridSizer241->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    staticBoxSizer239->Add(flexGridSizer241, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText276 = new wxStaticText(m_panelKDE, wxID_ANY, _("Snake Processing"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer241->Add(m_staticText276, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxEnableSnake = new wxCheckBox(m_panelKDE, wxID_ANY, _("Enable"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    m_checkBoxEnableSnake->SetValue(false);
    
    flexGridSizer241->Add(m_checkBoxEnableSnake, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText243 = new wxStaticText(m_panelKDE, wxID_ANY, _("lambda out"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer241->Add(m_staticText243, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlLambdaOut = new wxTextCtrl(m_panelKDE, wxID_ANY, wxT("20"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlLambdaOut->SetHint(wxT(""));
    #endif
    
    flexGridSizer241->Add(m_textCtrlLambdaOut, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText247 = new wxStaticText(m_panelKDE, wxID_ANY, _("lambda in"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    
    flexGridSizer241->Add(m_staticText247, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlLambdaIn = new wxTextCtrl(m_panelKDE, wxID_ANY, wxT("9"), wxDefaultPosition, wxDLG_UNIT(m_panelKDE, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlLambdaIn->SetHint(wxT(""));
    #endif
    
    flexGridSizer241->Add(m_textCtrlLambdaIn, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelAlgo = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    wxFont m_panelAlgoFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    m_panelAlgo->SetFont(m_panelAlgoFont);
    
    m_auimgr21->AddPane(m_panelAlgo, wxAuiPaneInfo().Caption(_("Algorithm")).Direction(wxAUI_DOCK_RIGHT).Layer(0).Row(0).Position(0).BestSize(220,200).MinSize(200,200).MaxSize(200,200).CaptionVisible(true).MaximizeButton(false).CloseButton(false).MinimizeButton(false).PinButton(false));
    
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
    m_listBoxBGS->SetSelection(0);
    
    boxSizer53->Add(m_listBoxBGS, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelGlobalSet = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(50,-1)), wxTAB_TRAVERSAL);
    
    m_auimgr21->AddPane(m_panelGlobalSet, wxAuiPaneInfo().Caption(_("Global Setting")).Direction(wxAUI_DOCK_BOTTOM).Layer(0).Row(0).Position(0).BestSize(50,100).MinSize(50,100).MaxSize(50,100).CaptionVisible(true).MaximizeButton(false).CloseButton(false).MinimizeButton(false).PinButton(false));
    
    wxBoxSizer* boxSizer262 = new wxBoxSizer(wxVERTICAL);
    m_panelGlobalSet->SetSizer(boxSizer262);
    
    wxFlexGridSizer* flexGridSizer264 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer264->SetFlexibleDirection( wxBOTH );
    flexGridSizer264->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    boxSizer262->Add(flexGridSizer264, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText110 = new wxStaticText(m_panelGlobalSet, wxID_ANY, _("Frame wait (ms)"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    
    flexGridSizer264->Add(m_staticText110, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlFrameWait = new wxTextCtrl(m_panelGlobalSet, wxID_ANY, wxT("100"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlFrameWait->SetHint(wxT(""));
    #endif
    
    flexGridSizer264->Add(m_textCtrlFrameWait, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText114 = new wxStaticText(m_panelGlobalSet, wxID_ANY, _("Start frame"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    
    flexGridSizer264->Add(m_staticText114, 0, wxALL|wxALIGN_TOP, WXC_FROM_DIP(5));
    
    m_textCtrlStartFrame = new wxTextCtrl(m_panelGlobalSet, wxID_ANY, wxT("0"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlStartFrame->SetHint(wxT(""));
    #endif
    
    flexGridSizer264->Add(m_textCtrlStartFrame, 0, wxALL|wxALIGN_TOP, WXC_FROM_DIP(5));
    
    m_staticText122 = new wxStaticText(m_panelGlobalSet, wxID_ANY, _("Sampling rate"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    
    flexGridSizer264->Add(m_staticText122, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlSampling = new wxTextCtrl(m_panelGlobalSet, wxID_ANY, wxT("1"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(50,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlSampling->SetHint(wxT(""));
    #endif
    
    flexGridSizer264->Add(m_textCtrlSampling, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText118 = new wxStaticText(m_panelGlobalSet, wxID_ANY, _("Show preprocess"), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    
    flexGridSizer264->Add(m_staticText118, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxShowPreprocess = new wxCheckBox(m_panelGlobalSet, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), 0);
    m_checkBoxShowPreprocess->SetValue(false);
    
    flexGridSizer264->Add(m_checkBoxShowPreprocess, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panel270 = new wxPanel(m_panelGlobalSet, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelGlobalSet, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    boxSizer262->Add(m_panel270, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer257 = new wxBoxSizer(wxHORIZONTAL);
    m_panel270->SetSizer(boxSizer257);
    
    wxArrayString m_radioBoxWholeRegionArr;
    m_radioBoxWholeRegionArr.Add(_("whole"));
    m_radioBoxWholeRegionArr.Add(_("left"));
    m_radioBoxWholeRegionArr.Add(_("right"));
    m_radioBoxWholeRegion = new wxRadioBox(m_panel270, wxID_ANY, _("Processing Region"), wxDefaultPosition, wxDLG_UNIT(m_panel270, wxSize(-1,-1)), m_radioBoxWholeRegionArr, 3, 0);
    m_radioBoxWholeRegion->SetSelection(2);
    
    boxSizer257->Add(m_radioBoxWholeRegion, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelMsg = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    wxFont m_panelMsgFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Calibri"));
    m_panelMsg->SetFont(m_panelMsgFont);
    
    m_auimgr21->AddPane(m_panelMsg, wxAuiPaneInfo().Name(wxT("MsgPane")).Caption(_("Message Pane")).Direction(wxAUI_DOCK_BOTTOM).Layer(0).Row(0).Position(1).BestSize(450,200).MinSize(450,190).MaxSize(450,280).CaptionVisible(true).MaximizeButton(false).CloseButton(true).MinimizeButton(false).PinButton(false));
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
    SetSize(750,700);
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
    this->Connect(m_menuItemLoadProfile->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileLoadProfile), NULL, this);
    this->Connect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Connect(m_menuItemViewMsgPane->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewMsgPane), NULL, this);
    this->Connect(m_menuItemViewShowProfile->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowProfile), NULL, this);
    this->Connect(m_menuItemViewShowFrameType->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowFrameType), NULL, this);
    this->Connect(m_menuItemVideoFrameProcessor->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFrameProcessor), NULL, this);
    this->Connect(m_menuItemVideoExtractFrame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoExtractFrames), NULL, this);
    this->Connect(m_menuItemVideoFGPixels->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFGPixels), NULL, this);
    this->Connect(m_menuItemBgKDE->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnBackgroundKDE), NULL, this);
    this->Connect(m_menuItemProfileGaussianSmooth->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileGaussianSmooth), NULL, this);
    this->Connect(m_menuItemProfileClassification->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileClassification), NULL, this);
    this->Connect(m_menuItemProfileCentroid->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileCentroid), NULL, this);
    this->Connect(m_menuItemTrainData->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnTrainData), NULL, this);
    this->Connect(m_menuItemTestResult->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnShowTestResult), NULL, this);
    this->Connect(m_menuItemPredictedResult->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnShowPredictedResult), NULL, this);
    this->Connect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    this->Connect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Connect(wxID_SHOW_PROFILE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowProfile), NULL, this);
    this->Connect(wxID_FRAME_PROCESSOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFrameProcessor), NULL, this);
    this->Connect(wxID_STOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoStop), NULL, this);
    this->Connect(wxID_PAUSE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoPause), NULL, this);
    this->Connect(wxID_EXTRACT_FRAME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoExtractFrames), NULL, this);
    m_auiBook->Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(MainFrameBaseClass::OnBookPageChanged), NULL, this);
    m_buttonBrowse->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnBrowseDataPath), NULL, this);
    m_textCtrlFrameNo->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(MainFrameBaseClass::OnTextFrameNoEnter), NULL, this);
    m_textCtrlMMSS->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(MainFrameBaseClass::OnTextMMSSEnter), NULL, this);
    m_checkBoxSceneChangeBreak->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnSceneChangeBreak), NULL, this);
    
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    this->Disconnect(m_menuItemFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Disconnect(m_menuItemLoadProfile->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileLoadProfile), NULL, this);
    this->Disconnect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Disconnect(m_menuItemViewMsgPane->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewMsgPane), NULL, this);
    this->Disconnect(m_menuItemViewShowProfile->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowProfile), NULL, this);
    this->Disconnect(m_menuItemViewShowFrameType->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowFrameType), NULL, this);
    this->Disconnect(m_menuItemVideoFrameProcessor->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFrameProcessor), NULL, this);
    this->Disconnect(m_menuItemVideoExtractFrame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoExtractFrames), NULL, this);
    this->Disconnect(m_menuItemVideoFGPixels->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFGPixels), NULL, this);
    this->Disconnect(m_menuItemBgKDE->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnBackgroundKDE), NULL, this);
    this->Disconnect(m_menuItemProfileGaussianSmooth->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileGaussianSmooth), NULL, this);
    this->Disconnect(m_menuItemProfileClassification->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileClassification), NULL, this);
    this->Disconnect(m_menuItemProfileCentroid->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnProfileCentroid), NULL, this);
    this->Disconnect(m_menuItemTrainData->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnTrainData), NULL, this);
    this->Disconnect(m_menuItemTestResult->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnShowTestResult), NULL, this);
    this->Disconnect(m_menuItemPredictedResult->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnShowPredictedResult), NULL, this);
    this->Disconnect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    this->Disconnect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Disconnect(wxID_SHOW_PROFILE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnViewShowProfile), NULL, this);
    this->Disconnect(wxID_FRAME_PROCESSOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoFrameProcessor), NULL, this);
    this->Disconnect(wxID_STOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoStop), NULL, this);
    this->Disconnect(wxID_PAUSE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoPause), NULL, this);
    this->Disconnect(wxID_EXTRACT_FRAME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnVideoExtractFrames), NULL, this);
    m_auiBook->Disconnect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(MainFrameBaseClass::OnBookPageChanged), NULL, this);
    m_buttonBrowse->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnBrowseDataPath), NULL, this);
    m_textCtrlFrameNo->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(MainFrameBaseClass::OnTextFrameNoEnter), NULL, this);
    m_textCtrlMMSS->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(MainFrameBaseClass::OnTextMMSSEnter), NULL, this);
    m_checkBoxSceneChangeBreak->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnSceneChangeBreak), NULL, this);
    
    m_auimgr21->UnInit();
    delete m_auimgr21;

}
