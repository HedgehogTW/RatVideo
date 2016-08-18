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
#include "package_bgs/IBGS.h"

#include "package_bgs/FrameDifferenceBGS.h"
#include "package_bgs/StaticFrameDifferenceBGS.h"
#include "package_bgs/WeightedMovingMeanBGS.h"
#include "package_bgs/WeightedMovingVarianceBGS.h"
//#include "package_bgs/MixtureOfGaussianV1BGS.h"
#include "package_bgs/MixtureOfGaussianV2BGS.h"
#include "package_bgs/AdaptiveBackgroundLearning.h"
#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >= 4 && CV_SUBMINOR_VERSION >= 3
#include "package_bgs/GMG.h"
#endif

#include "package_bgs/dp/DPAdaptiveMedianBGS.h"
#include "package_bgs/dp/DPGrimsonGMMBGS.h"
#include "package_bgs/dp/DPZivkovicAGMMBGS.h"
#include "package_bgs/dp/DPMeanBGS.h"
#include "package_bgs/dp/DPWrenGABGS.h"
#include "package_bgs/dp/DPPratiMediodBGS.h"
#include "package_bgs/dp/DPEigenbackgroundBGS.h"
#include "package_bgs/dp/DPTextureBGS.h"

#include "package_bgs/tb/T2FGMM_UM.h"
#include "package_bgs/tb/T2FGMM_UV.h"
#include "package_bgs/tb/T2FMRF_UM.h"
#include "package_bgs/tb/T2FMRF_UV.h"
#include "package_bgs/tb/FuzzySugenoIntegral.h"
#include "package_bgs/tb/FuzzyChoquetIntegral.h"

#include "package_bgs/lb/LBSimpleGaussian.h"
#include "package_bgs/lb/LBFuzzyGaussian.h"
#include "package_bgs/lb/LBMixtureOfGaussians.h"
#include "package_bgs/lb/LBAdaptiveSOM.h"
#include "package_bgs/lb/LBFuzzyAdaptiveSOM.h"

#include "package_bgs/ck/LbpMrf.h"

//#include "package_bgs/jmo/MultiLayerBGS.h" // don't works with opencv3
// The PBAS algorithm was removed from BGSLibrary because it is
// based on patented algorithm ViBE
// http://www2.ulg.ac.be/telecom/research/vibe/
//#include "package_bgs/pt/PixelBasedAdaptiveSegmenter.h"
#include "package_bgs/av/VuMeter.h"
#include "package_bgs/ae/KDE.h"
#include "package_bgs/db/IndependentMultimodalBGS.h"
#include "package_bgs/sjn/SJN_MultiCueBGS.h"
#include "package_bgs/bl/SigmaDeltaBGS.h"

#include "package_bgs/pl/SuBSENSE.h"
#include "package_bgs/pl/LOBSTER.h"

#include "package_analysis/ForegroundMaskAnalysis.h"

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
	
	m_bStopProcess = false;
//	m_bgs = NULL;
//	m_bgs = new KDE;

//	DeleteContents();
	wxString outpath, str; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	outpath = "./config";// + subpath;
#else
	outpath = ".\\config";
#endif	

	if(wxDirExists(outpath)==false) {
		if(wxMkdir(outpath)==false) {
			str.Printf("Create config directory error, %s", outpath);
			wxLogMessage(str);
			return;
		}
	}
	
	wxFont font(wxFontInfo(10).FaceName("Helvetica").Italic());
	m_auimgr21->GetArtProvider()->SetFont(wxAUI_DOCKART_CAPTION_FONT, font);
	SetPageKDE();
	SetGlobalPara();
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
	m_bStopProcess = false;
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
void MainFrame::SetGlobalPara()
{
    CvFileStorage* fs = cvOpenFileStorage("./config/PreProcessor.xml", 0, CV_STORAGE_READ);

    equalizeHist = cvReadIntByName(fs, 0, "equalizeHist", false);
    gaussianBlur = cvReadIntByName(fs, 0, "gaussianBlur", false);
    bShowPreprocess = cvReadIntByName(fs, 0, "enableShow", true);

    cvReleaseFileStorage(&fs);	
	
	m_checkBoxShowPreprocess->SetValue(bShowPreprocess);
}
void MainFrame::SaveGlobalPara()
{
	bShowPreprocess = m_checkBoxShowPreprocess->GetValue();	
	
    CvFileStorage* fs = cvOpenFileStorage("./config/PreProcessor.xml", 0, CV_STORAGE_WRITE);

    cvWriteInt(fs, "equalizeHist", equalizeHist);
    cvWriteInt(fs, "gaussianBlur", gaussianBlur);
    cvWriteInt(fs, "enableShow", bShowPreprocess);

    cvReleaseFileStorage(&fs);	
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

	wxString str = m_textCtrlFrameWait->GetValue();
	str.ToLong(&m_waitTime);
	
	str = m_textCtrlStartFrame->GetValue();
	str.ToLong(&m_startFrame);	

	myMsgOutput( "\nLoad ... " + m_Filename + " OK\n");
	myMsgOutput( "Wait time: " + str + " ms\n");
	
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
	m_bStopProcess = false;
	do{
		frameNumber++;	
		m_vidCap >> img_input;
		if (img_input.empty()) break;
	}while(frameNumber <= m_startFrame);
	
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
		
		if(m_bStopProcess)  break;
		if(cv::waitKey(m_waitTime) >= 0) break;
		
	}while(1);

	delete bgs;
//	m_bgs = NULL;

}
void MainFrame::OnVideoFrameProcessor(wxCommandEvent& event)
{
	DeleteContents();
	
	int selBGS = m_listBoxBGS->GetSelection();
	if(selBGS == wxNOT_FOUND) {
		myMsgOutput( "No BGS algorithm selection\n");
		return;		
	}
	wxString strBGS = m_listBoxBGS->GetString(selBGS);
	
	m_Filename = "d:\\tmp\\1218(4).AVI";
	m_vidCap.open(m_Filename.ToStdString());
	if(m_vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	double fps = m_vidCap.get(CV_CAP_PROP_FPS);
	
	wxString str = m_textCtrlFrameWait->GetValue();
	str.ToLong(&m_waitTime);	
	
	str = m_textCtrlStartFrame->GetValue();
	str.ToLong(&m_startFrame);
	
	SaveGlobalPara();
	
	wxString msg;
	msg << "\nLoad ... " << m_Filename << " OK\nDo " << strBGS << ", wait " << m_waitTime << " ms\n";
	msg << fps << " fps, start frame: " << m_startFrame << "\n";
	myMsgOutput(msg );
	
	IBGS *bgs = NULL;
	if(strBGS.IsSameAs("FrameDifferenceBGS", false))
		bgs = new FrameDifferenceBGS;
	else if(strBGS.IsSameAs("StaticFrameDifferenceBGS", false))
		bgs = new StaticFrameDifferenceBGS;
	else if(strBGS.IsSameAs("WeightedMovingMeanBGS", false))
		bgs = new WeightedMovingMeanBGS;
	else if(strBGS.IsSameAs("WeightedMovingVarianceBGS", false))
		bgs = new WeightedMovingVarianceBGS;
	else if(strBGS.IsSameAs("MixtureOfGaussianV2BGS", false))
		bgs = new MixtureOfGaussianV2BGS;
	else if(strBGS.IsSameAs("AdaptiveBackgroundLearning", false))
		bgs = new AdaptiveBackgroundLearning;
	else if(strBGS.IsSameAs("DPAdaptiveMedianBGS", false))
		bgs = new DPAdaptiveMedianBGS;		
	else if(strBGS.IsSameAs("DPGrimsonGMMBGS", false))
		bgs = new DPGrimsonGMMBGS;
	else if(strBGS.IsSameAs("DPZivkovicAGMMBGS", false))
		bgs = new DPZivkovicAGMMBGS;
	else if(strBGS.IsSameAs("DPMeanBGS", false))
		bgs = new DPMeanBGS;
	else if(strBGS.IsSameAs("DPWrenGABGS", false))
		bgs = new DPWrenGABGS;
	else if(strBGS.IsSameAs("DPPratiMediodBGS", false))
		bgs = new DPPratiMediodBGS;
	else if(strBGS.IsSameAs("DPEigenbackgroundBGS", false))
		bgs = new DPEigenbackgroundBGS;
	else if(strBGS.IsSameAs("DPTextureBGS", false))
		bgs = new DPTextureBGS;
	else if(strBGS.IsSameAs("T2FGMM_UM", false))
		bgs = new T2FGMM_UM;
	else if(strBGS.IsSameAs("T2FGMM_UV", false))
		bgs = new T2FGMM_UV;		
	else if(strBGS.IsSameAs("T2FMRF_UM", false))
		bgs = new T2FMRF_UM;
	else if(strBGS.IsSameAs("T2FMRF_UV", false))
		bgs = new T2FMRF_UV;
	else if(strBGS.IsSameAs("FuzzySugenoIntegral", false))
		bgs = new FuzzySugenoIntegral;
	else if(strBGS.IsSameAs("FuzzyChoquetIntegral", false))
		bgs = new FuzzyChoquetIntegral;
	else if(strBGS.IsSameAs("LBSimpleGaussian", false))
		bgs = new LBSimpleGaussian;
	else if(strBGS.IsSameAs("LBFuzzyGaussian", false))
		bgs = new LBFuzzyGaussian;		
	else if(strBGS.IsSameAs("LBMixtureOfGaussians", false))
		bgs = new LBMixtureOfGaussians;
	else if(strBGS.IsSameAs("LBAdaptiveSOM", false))
		bgs = new LBAdaptiveSOM;		
	else if(strBGS.IsSameAs("LBFuzzyAdaptiveSOM", false))
		bgs = new LBFuzzyAdaptiveSOM;	
	else if(strBGS.IsSameAs("LbpMrf", false))
		bgs = new LbpMrf;
	else if(strBGS.IsSameAs("VuMeter", false))
		bgs = new VuMeter;		
	else if(strBGS.IsSameAs("KDE", false))
		bgs = new KDE;
	else if(strBGS.IsSameAs("IndependentMultimodalBGS", false))
		bgs = new IndependentMultimodalBGS;
	else if(strBGS.IsSameAs("MultiCueBGS", false))
		bgs = new SJN_MultiCueBGS;
	else if(strBGS.IsSameAs("SigmaDeltaBGS", false))
		bgs = new SigmaDeltaBGS;		
	else if(strBGS.IsSameAs("SuBSENSEBGS", false))
		bgs = new SuBSENSEBGS;
	else if(strBGS.IsSameAs("LOBSTERBGS", false))
		bgs = new LOBSTERBGS;
	
	if(bgs==NULL) {
		myMsgOutput( "No BGS algorithm create\n");
		return;	
	}
	
	m_pPreProcessor = new bgslibrary::PreProcessor;	
	
//	frameProcessor = new FrameProcessor;
//	frameProcessor->init();
/*
	int64 start_time;
    int64 delta_time;
    double freq;
    double fps;	
	 */ 
	int frameNumber = 0;
	
	cv::Mat img_input;
	cv::Mat img_prep;
	m_bStopProcess = false;
	do{
		frameNumber++;	
		m_vidCap >> img_input;
		if (img_input.empty()) break;
	}while(frameNumber <= m_startFrame);
	
    do
    {
		frameNumber++;

		m_vidCap >> img_input;
		if (img_input.empty()) break;

		cv::imshow("Input", img_input);
		m_pPreProcessor->process(img_input, img_prep);

//		start_time = cv::getTickCount();

		bgs->process(img_prep, m_mMask, m_mbkgmodel);
/*
		delta_time = cv::getTickCount() - start_time;
		freq = cv::getTickFrequency();
		fps = freq / delta_time;
		 */ 
		//std::cout << "FPS: " << fps << std::endl;
		float sec = frameNumber /fps;
		int mm = sec / 60;
		int ss = sec - mm*60;
		wxString str;
		str.Printf("Frame No. %d, %02d:%02d", frameNumber, mm, ss);
		m_statusBar->SetStatusText(str, 3);
		
		if(m_bStopProcess)  break;
		if(cv::waitKey(m_waitTime) >= 0) break;
		
	}while(1);	
//	delete frameProcessor;
	delete bgs;
}
void MainFrame::OnVideoStop(wxCommandEvent& event)
{
	m_bStopProcess = true;
	myMsgOutput( "OnVideoStop\n");
}
