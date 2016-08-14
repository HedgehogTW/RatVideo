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

#include "PreProcessor.h"
#include "package_bgs/ae/KDE.h"
#include "package_bgs/MixtureOfGaussianV2BGS.h"

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
	
//	m_bgs = NULL;
//	m_bgs = new KDE;

//	DeleteContents();

	SetPageKDE();
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
//	wxConfigBase *pConfig = wxConfigBase::Get();
/*
	if(m_bgs) {
		delete m_bgs;
		m_bgs = NULL;
	}
 */
	if(m_pPreProcessor)  {
		delete m_pPreProcessor;
		m_pPreProcessor = NULL;
	}
	
	if(m_vidCap.isOpened()) m_vidCap.release();
		
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

void MainFrame::SetPageKDE()
{

	CvFileStorage* fs = cvOpenFileStorage("./config/KDE.xml", 0, CV_STORAGE_READ);

	framesToLearn = cvReadIntByName(fs, 0, "framesToLearn", 10);
	SequenceLength = cvReadIntByName(fs, 0, "SequenceLength", 50);
	TimeWindowSize = cvReadIntByName(fs, 0, "TimeWindowSize", 100);
	SDEstimationFlag = cvReadIntByName(fs, 0, "SDEstimationFlag", 1);
	lUseColorRatiosFlag = cvReadIntByName(fs, 0, "lUseColorRatiosFlag", 1);
	th = cvReadRealByName(fs, 0, "th", 10e-8);
	alpha = cvReadRealByName(fs, 0, "alpha", 0.3);
	//showOutput = cvReadIntByName(fs, 0, "showOutput", true);

	cvReleaseFileStorage(&fs);		
	
	wxString  s1, s2, s3, s4, s5, s6;
	s1 << framesToLearn;
	s2 << SequenceLength;
	s3 << TimeWindowSize;
	s4 << th;
	s5 << alpha;
	
	m_textCtrlframesToLearn->SetValue(s1);
	m_textCtrlSequenceLen->SetValue(s2);
	m_textCtrlTimeWinSize->SetValue(s3);
	m_checkBoxSDEstFlag->SetValue(SDEstimationFlag);
	m_checkBoxlUseColorRatiosFlag->SetValue(lUseColorRatiosFlag);
	m_textCtrlThreshold->SetValue(s4);
	m_textCtrlAlpha->SetValue(s5);
	
}

void MainFrame::SavePageKDE()
{		
	
	wxString  str;
	long a;
	double  value;
	
	str = m_textCtrlframesToLearn->GetValue();
	str.ToLong(&a);	
	framesToLearn = a;
	
	str = m_textCtrlSequenceLen->GetValue();
	str.ToLong(&a);	
	SequenceLength = a;
	
	str = m_textCtrlTimeWinSize->GetValue();
	str.ToLong(&a);	
	TimeWindowSize = a;
	
	SDEstimationFlag = m_checkBoxSDEstFlag->GetValue();
	lUseColorRatiosFlag = m_checkBoxlUseColorRatiosFlag->GetValue();
	
	str = m_textCtrlThreshold->GetValue();
	str.ToDouble(&value);	
	th = value;
	
	str = m_textCtrlAlpha->GetValue();
	str.ToDouble(&value);	
	alpha = value;	
	
	CvFileStorage* fs = cvOpenFileStorage("./config/KDE.xml", 0, CV_STORAGE_WRITE);

	cvWriteInt(fs, "framesToLearn", framesToLearn);
	cvWriteInt(fs, "SequenceLength", SequenceLength);
	cvWriteInt(fs, "TimeWindowSize", TimeWindowSize);
	cvWriteInt(fs, "SDEstimationFlag", SDEstimationFlag);
	cvWriteInt(fs, "lUseColorRatiosFlag", lUseColorRatiosFlag);
	cvWriteReal(fs, "th", th);
	cvWriteReal(fs, "alpha", alpha);
//	cvWriteInt(fs, "showOutput", showOutput);

	cvReleaseFileStorage(&fs);	
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
void MainFrame::OnVideoBGSProcess(wxCommandEvent& event)
{
	DeleteContents();
		


	m_Filename = "d:\\tmp\\1218(4).AVI";
	m_vidCap.open(m_Filename.ToStdString());
	if(m_vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	myMsgOutput( "\nLoad ... " + m_Filename + " OK\n");
	
	m_pPreProcessor = new bgslibrary::PreProcessor;		
	int tab = m_auiBook->GetSelection();
	myMsgOutput("OnBookPageChanged %d\n", tab);
	switch(tab) {
		case 0:
			SavePageKDE();
			BGS_KDE();
			break;
		case 1:
			break;
		case 2:
			break;
	}

}
void MainFrame::OnBookPageChanged(wxAuiNotebookEvent& event)
{
	int tab = m_auiBook->GetSelection();
	myMsgOutput("OnBookPageChanged %d\n", tab);
}

void MainFrame::BGS_KDE()
{	
	myMsgOutput("----------------------------KDE\n");
	myMsgOutput("framesToLearn %d, SequenceLength %d, TimeWindowSize %d\n", framesToLearn, SequenceLength, TimeWindowSize);
	myMsgOutput("SDEstimationFlag %d, lUseColorRatiosFlag %d\n", SDEstimationFlag, lUseColorRatiosFlag);
	myMsgOutput("threshold %.5f, alpha %.2f\n", th, alpha);
	
	IBGS *bgs = new KDE;
	
	int64 start_time;
    int64 delta_time;
    double freq;
    double fps;	
	int frameNumber = 0;
	
	cv::Mat img_input;
	cv::Mat img_prep;
   do
    {
		frameNumber++;

		m_vidCap >> img_input;
		if (img_input.empty()) break;

		cv::imshow("Input", img_input);

		m_pPreProcessor->process(img_input, img_prep);

		start_time = cv::getTickCount();

		bgs->process(img_prep, m_mMask, m_mbkgmodel); // by default, it shows automatically the foreground mask image

		delta_time = cv::getTickCount() - start_time;
		freq = cv::getTickFrequency();
		fps = freq / delta_time;
		//std::cout << "FPS: " << fps << std::endl;
		if(cv::waitKey(20) >= 0) break;
		
	}while(1);

	delete bgs;
//	m_bgs = NULL;

}