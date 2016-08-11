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
    m_auibar23->Realize();
    
    m_panel25 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    wxFont m_panel25Font(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Calibri"));
    m_panel25->SetFont(m_panel25Font);
    
    m_auimgr21->AddPane(m_panel25, wxAuiPaneInfo().Name(wxT("MsgPane")).Caption(_("Message Pane")).Direction(wxAUI_DOCK_CENTER).Layer(0).Row(0).Position(0).BestSize(100,300).MinSize(100,300).MaxSize(100,300).CaptionVisible(true).MaximizeButton(false).CloseButton(true).MinimizeButton(false).PinButton(false));
    m_auimgr21->Update();
    
    wxBoxSizer* boxSizer27 = new wxBoxSizer(wxVERTICAL);
    m_panel25->SetSizer(boxSizer27);
    
    m_richTextMsg = new wxRichTextCtrl(m_panel25, wxID_ANY, wxT("Hello.... Cute Rat ..."), wxDefaultPosition, wxDLG_UNIT(m_panel25, wxSize(-1,-1)), wxTE_MULTILINE|wxTE_PROCESS_TAB|wxTE_PROCESS_ENTER|wxWANTS_CHARS);
    
    boxSizer27->Add(m_richTextMsg, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    SetName(wxT("MainFrameBaseClass"));
    SetSize(550,550);
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
    this->Connect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    this->Connect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    this->Disconnect(m_menuItemFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Disconnect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Disconnect(m_menuItemViewMsgPane->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnViewMsgPane), NULL, this);
    this->Disconnect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    this->Disconnect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    
    m_auimgr21->UnInit();
    delete m_auimgr21;

}
