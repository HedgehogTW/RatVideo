#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/bitmap.h>
#include <wx/log.h>
#include <wx/config.h>

#include <wx/dcclient.h>
#include <wx/dirdlg.h> 
#include <wx/msgdlg.h> 
#include <wx/filename.h>
#include <wx/utils.h> 
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/sound.h>
#include <wx/filefn.h> 

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
//using namespace std;
using namespace cv;
using namespace bgslibrary;

MainFrame *	MainFrame::m_pThis=NULL;

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
#if defined(__WXMAC__)
/*
	wxIcon  icon;
	wxBitmap bitmap(wxT("ratty32.png"), wxBITMAP_TYPE_PNG); 
	icon.CopyFromBitmap(bitmap); 
	SetIcon(icon);
*/
#else
	SetIcon(wxICON(frame_icon));
#endif

	m_pThis = this;
	int statusWidth[4] = {250, 110, 40, 140};
	m_statusBar->SetFieldsCount(4, statusWidth);	
	
	wxConfigBase *pConfig = wxConfigBase::Get();
	m_FileHistory = new wxFileHistory(9);
	m_FileHistory->UseMenu(m_menuFile);
	m_FileHistory->AddFilesToMenu(m_menuFile);
	m_FileHistory->Load(*pConfig);


	this->Connect(wxID_FILE1, wxID_FILE9, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMRUFile), NULL, this);
	
	SetSize(700, 550);
	Center();	
	
	DeleteContents();
}

MainFrame::~MainFrame()
{
	DeleteContents();
	
	wxConfigBase *pConfig = wxConfigBase::Get();
	m_FileHistory->Save(*pConfig);
	delete m_FileHistory;	
	delete wxConfigBase::Set((wxConfigBase *) NULL);	
}

void MainFrame::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	wxConfigBase *pConfig = wxConfigBase::Get();

}
void MainFrame::OnMRUFile(wxCommandEvent& event)
{
	wxString f(m_FileHistory->GetHistoryFile(event.GetId() - wxID_FILE1));
	if (f.empty())  return;
	
	openFile(f);
	m_FileHistory->AddFileToHistory(f);	
}
void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("My MainFrame"));
    info.SetLicence(_("GPL v2 or later"));
    info.SetDescription(_("Short description goes here"));
    ::wxAboutBox(info);
}
void MainFrame::OnFileOpen(wxCommandEvent& event)
{
	wxString  strHistoryFile = wxEmptyString;
	if(m_FileHistory->GetCount() >0) 
		strHistoryFile= m_FileHistory->GetHistoryFile(0);
		
	static wxString strInitFile (strHistoryFile);

	wxString wildcard = "avi and mp4 files (*.avi;*.mp4)|*.avi;*.mp4|\
	All Files (*.*)|*.*";

	wxFileDialog* OpenDialog = new wxFileDialog(
	    this, _("Choose a file to open"), wxEmptyString, strInitFile,
	    wildcard, wxFD_OPEN, wxDefaultPosition);
	OpenDialog->SetFilterIndex(0);
	if (OpenDialog->ShowModal() == wxID_OK) {
	    wxString pathName = OpenDialog->GetPath();
		m_FileHistory->AddFileToHistory(pathName);
		strInitFile = pathName;
		
		openFile(pathName);	
	    //wxSize sz = m_scrollWin->setImage(pathName);
	    //wxString str;
	    //str.Printf("W %d, H %d", sz.GetWidth(), sz.GetHeight());
	    //m_statusBar->SetStatusText(str, 1);	
	}
	// Clean up after ourselves
	OpenDialog->Destroy();

	wxBell();	
}

void MainFrame::openFile(wxString &fileName)
{
//	wxBeginBusyCursor();
	DeleteContents();

//	cv::VideoCapture vidCap(fileName.ToStdString());
//	if(vidCap.isOpened()==false) {
//		myMsgOutput( "Load ... " + fileName + " ERROR\n");
//	}
	m_Filename = fileName;		
	m_strSourcePath = fileName;
	myMsgOutput("\n++++++++++++++++++++++++++++++++++++++++++++++++\n");
	myMsgOutput( "Load ... " + fileName + "\n");
/*
    namedWindow("Rat0",1);
    while(1){
		Mat frame;
		if(!vidCap.grab()) break;
		vidCap.retrieve(frame);

		imshow("Rat0", frame);
		if(waitKey(20) >= 0) break;
    }
*/	
//	wxEndBusyCursor();

}
void MainFrame::OnViewMsgPane(wxCommandEvent& event)
{
	wxAuiPaneInfo &pane =  m_auimgr21->GetPane(wxT("MsgPane"));
	pane.Show(!pane.IsShown());

	m_auimgr21->Update();	
}
void MainFrame::OnVideoBGSubtraction(wxCommandEvent& event)
{
//	myMsgOutput( "Load ... " + m_Filename);
	m_Filename = "d:\\tmp\\1218(4).AVI";
	cv::VideoCapture vidCap(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
    int64 start_time;
    int64 delta_time;
    double freq;
    double fps;	
	int frameNumber = 0;
	frameProcessor = new FrameProcessor;
	frameProcessor->init();
//	frameProcessor->frameToStop = frameToStop;

    do
    {
		frameNumber++;

		cv::Mat frame;
		vidCap >> frame;
		if (frame.empty()) break;

		cv::Mat img_input;
		frame.copyTo(img_input);
		cv::imshow("Input", img_input);

		start_time = cv::getTickCount();

		frameProcessor->process(img_input);
		delta_time = cv::getTickCount() - start_time;
		freq = cv::getTickFrequency();
		fps = freq / delta_time;
		//std::cout << "FPS: " << fps << std::endl;
		if(cv::waitKey(20) >= 0) break;
		
	}while(1);

	delete frameProcessor;

}
void MainFrame::OnBookPageChanged(wxAuiNotebookEvent& event)
{
	int tab = m_auiBook->GetSelection();
	myMsgOutput("OnBookPageChanged %d\n", tab);
}
