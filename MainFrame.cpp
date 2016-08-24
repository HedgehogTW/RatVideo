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


#include "DlgExtractFrame.h"
#include "KDEBg.h"

#include "MyUtil.h"
#include "gnuplot_i.h"

using namespace std;
using namespace cv;
using namespace bgslibrary;

Gnuplot gPlotProfile("lines");
Gnuplot gPlotFrameType("lines");
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
	int statusWidth[4] = {110, 140, 140, 140};
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
//	m_pPreProcessor = NULL;
	m_fps = 29.97;
	
//	DeleteContents();
	wxString outpath, str; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	outpath = "./config";// + subpath;
	m_Filename = "~/tmp/1218(4).AVI";
#else
	outpath = ".\\config";
	m_Filename = "d:\\tmp\\1218(4).AVI";
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
	
	
	myMsgOutput("Hello.... Cute Rat ...\n");
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
	m_bStopProcess = true;
/*	
	if(m_pPreProcessor)  {
		delete m_pPreProcessor;
		m_pPreProcessor = NULL;
	}
*/	

		
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
			
	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	myMsgOutput( "\nLoad ... " + m_Filename + " OK\n");
	
	readVideoProperties(vidCap);	
	readControlValues();
	wxString msg;
	msg.Printf("Wait time: %d ms,  startFrame: %d, sampling: %d\n", m_waitTime, m_startFrame, m_Sampling);
	myMsgOutput( msg);
	
//	destroyAllWindows();
		
	int tab = m_auiBook->GetSelection();
	myMsgOutput("OnBookPageChanged %d\n", tab);
	switch(tab) {
		case 0:
			SavePageKDE();
			BGS_KDE(vidCap);
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

void MainFrame::BGS_KDE(cv::VideoCapture& vidCap)
{	
	myMsgOutput("----------------------------KDE\n");
	myMsgOutput("framesToLearn %d, SequenceLength %d, TimeWindowSize %d\n", framesToLearn, SequenceLength, TimeWindowSize);
	myMsgOutput("SDEstimationFlag %d, lUseColorRatiosFlag %d\n", SDEstimationFlag, lUseColorRatiosFlag);
	myMsgOutput("threshold %.5f, alpha %.2f\n", th, alpha);


	bgslibrary::PreProcessor* pPreProcessor = new bgslibrary::PreProcessor;	
	IBGS *bgs = new KDE;
	
	int64 start_time;
    int64 delta_time;
    double freq;
    double fps;	
	int frameNumber = 0;
	
	cv::Mat img_input;
	cv::Mat img_prep;
	cv::Mat mMovingObj;
    cv::Mat mbkgmodel;
	
	m_bStopProcess = false;
	while(frameNumber < m_startFrame){
		frameNumber++;	
		vidCap >> img_input;
		if (img_input.empty()) break;
	}
	
   do
    {
		if(m_bStopProcess)  break;		
		frameNumber++;

		vidCap >> img_input;
		if (img_input.empty()) break;

		cv::imshow("Input", img_input);

		pPreProcessor->process(img_input, img_prep, m_bLeftSide);

		start_time = cv::getTickCount();

		bgs->process(img_prep, mMovingObj, mbkgmodel); // by default, it shows automatically the foreground mask image

		delta_time = cv::getTickCount() - start_time;
		freq = cv::getTickFrequency();
		fps = freq / delta_time;
		//std::cout << "FPS: " << fps << std::endl;
		

		if(cv::waitKey(m_waitTime) >= 0) break;
		
	}while(1);

	delete bgs;
	delete pPreProcessor;
//	m_bgs = NULL;


}
void MainFrame::OnVideoFrameProcessor(wxCommandEvent& event)
{
	DeleteContents();
	
	int selBGS = m_listBoxBGS->GetSelection();
	if(selBGS == wxNOT_FOUND) {
		myMsgOutput( "No BGS algorithm selection\n");		
		wxMessageBox( "No BGS algorithm selection","Error", wxICON_ERROR);

		return;		
	}

	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	readVideoProperties(vidCap);
	readControlValues();
	
	SaveGlobalPara();
//	destroyAllWindows();
	wxString strBGS = m_listBoxBGS->GetString(selBGS);	
	wxString msg;
	msg << "\nLoad ... " << m_Filename << " OK\nDo " << strBGS << "\n";
	myMsgOutput(msg );
		
	IBGS *bgs = createBGSObj(strBGS);
	if(bgs==NULL) {
		myMsgOutput( "No BGS algorithm create\n");
		return;	
	} 
	
	bgslibrary::PreProcessor* pPreProcessor = new bgslibrary::PreProcessor;	
	
/*
	int64 start_time;
    int64 delta_time;
    double freq;
    double fps;	
	 */ 
	int frameNumber = 0;
	
	cv::Mat img_input;
	cv::Mat img_prep;
	cv::Mat mMovingObj;
    cv::Mat mbkgmodel;
	m_bStopProcess = false;

	while(frameNumber < m_startFrame){
		frameNumber++;	
		vidCap >> img_input;
		if (img_input.empty()) break;
	}
	
    do
    {
		if(m_bStopProcess)  break;


		vidCap >> img_input;
		if (img_input.empty()) break;
		if(frameNumber % m_Sampling) continue;
		
		cv::imshow("Input", img_input);
		pPreProcessor->process(img_input, img_prep, m_bLeftSide);

		bgs->process(img_prep, mMovingObj, mbkgmodel);

/*		
		double msec = vidCap.get(CV_CAP_PROP_POS_MSEC );
		int fmm = msec / (1000*60);
		int fss = (msec - fmm*60*1000 )/1000;
		int fms =  msec - fmm*60*1000 - fss*1000 ;
		wxString str;
		str.Printf("%02d:%02d.%03d", fmm, fss, fms);
		m_statusBar->SetStatusText(str, 1);
*/
		
		float sec = frameNumber /m_fps;
		int mm = sec / 60;
		int ss = sec - mm*60;
		wxString str;
		str.Printf("Frame No. %d, %02d:%02d", frameNumber, mm, ss);
		m_statusBar->SetStatusText(str, 3);
		
		frameNumber++;
		if(cv::waitKey(m_waitTime) >= 0) break;
		
	}while(1);	

	delete bgs;
	delete pPreProcessor;
	
}
void MainFrame::OnVideoStop(wxCommandEvent& event)
{
	m_bStopProcess = true;
	myMsgOutput( "OnVideoStop\n");
}
IBGS * MainFrame::createBGSObj(wxString& strBGS)
{
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
	
	return bgs;

}
void MainFrame::readVideoProperties(cv::VideoCapture& vidCap)
{
	m_fps = vidCap.get(CV_CAP_PROP_FPS);
	m_width = vidCap.get(CV_CAP_PROP_FRAME_WIDTH );
	m_height = vidCap.get(CV_CAP_PROP_FRAME_HEIGHT );
	
	wxString str;	
	str.Printf("W%d x H%d, fps %.2f", m_width, m_height, m_fps);
	m_statusBar->SetStatusText(str, 2);	
}
void MainFrame::readControlValues()
{	
	wxString str;
	str = m_textCtrlGausKSize->GetValue();
	str.ToLong(&m_nGauKSize);
	
	str = m_textCtrlFrameWait->GetValue();
	str.ToLong(&m_waitTime);	
	
	str = m_textCtrlStartFrame->GetValue();
	str.ToLong(&m_startFrame);
	
	str = m_textCtrlSampling->GetValue();
	str.ToLong(&m_Sampling);
	if(m_Sampling<=0) m_Sampling = 1;	
	
	m_bLeftSide = m_radioButtonLeftSide->GetValue();
	
	wxString msg;
	msg << "readControlValues ... \n";
	msg << "\t wait time: " << m_waitTime << " ms\n";
	msg << "\t start frame: " << m_startFrame << ", sampling: " << m_Sampling << "\n";
	msg << "\t LeftSide " << m_bLeftSide << "\n";
	msg << "\t GauKSize " << m_nGauKSize << "\n";
	myMsgOutput(msg );	
}
void MainFrame::OnVideoExtractFrames(wxCommandEvent& event)
{
	DlgExtractFrame dlg(this);
	if(dlg.ShowModal()== wxID_CANCEL ) return;
		
	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	
	double fps = vidCap.get(CV_CAP_PROP_FPS);
	
	long fromMM, fromSS, toMM, toSS, fromFrame, toFrame;
	
	dlg.getParam(fromMM, fromSS, toMM, toSS);
	
	fromFrame = (fromMM*60+fromSS)*fps;
	toFrame = (toMM*60+toSS)*fps;
	
	int frameNumber = 0;
	cv::Mat img_input;
	while(frameNumber < fromFrame){
		vidCap >> img_input;
		if (img_input.empty()) break;
		frameNumber++;	
	}
	
	wxString outpath, strFramename, outName, str; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	outpath.Printf("~/tmp/frame_%02d%02d_%02d%02d/", fromMM, fromSS, toMM, toSS);
#else
	outpath.Printf("d:\\tmp\\frame_%02d%02d_%02d%02d\\", fromMM, fromSS, toMM, toSS);
#endif	

	if(wxDirExists(outpath)==false) {
		if(wxMkdir(outpath)==false) {
			str.Printf("Create frame directory error, %s", outpath);
			wxLogMessage(str);
			return;
		}
	}

	while(frameNumber < toFrame){
		vidCap >> img_input;
		if (img_input.empty()) break;
		strFramename.Printf("f%02d%02d_%02d%02d_%06d.png", fromMM, fromSS, toMM, toSS, frameNumber);
		outName  = outpath + strFramename;
		imwrite(outName.ToStdString(), img_input);
		frameNumber++;	
	}	
	
	wxString msg;
	msg.Printf("Extract frame from %02d:%02d (%d) to  %02d:%02d (%d)\n",fromMM, fromSS, fromFrame, toMM, toSS, toFrame );
	myMsgOutput(msg);
}

void MainFrame::OnBackgroundKDE(wxCommandEvent& event)
{
	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}

	readVideoProperties(vidCap);
	readControlValues();
	
	wxString msg;
	msg.Printf("\nLoad %s, w%d x h%d, do KDE background ...\n", m_Filename, m_width, m_height);
	myMsgOutput(msg );
	

	cv::Mat img_input;
	KDEBg kdeModel;	
	int frameNumber = 0;
	int counter = 0;
	int nTrainMin = 10;
	int sampling = 200;
	int	nKernelBW = 10;
	double fgProb = 0.001;
	int nTrainingFrames = nTrainMin*60*m_fps/sampling;	

	
	myMsgOutput("training %d min, %d frames, sampling %d\n", nTrainMin, nTrainingFrames, sampling);
	kdeModel.init(m_width, m_height, nKernelBW, nTrainingFrames, fgProb);
	while(frameNumber < m_startFrame){
		frameNumber++;	
		vidCap >> img_input;
		if (img_input.empty()) break;
	}
	bool bAbort = false;	
	m_bStopProcess = false;
	do {
		if(m_bStopProcess)  {
			bAbort = true;
			break;			
		}
		vidCap >> img_input;
		if (img_input.empty()) break;
		if(frameNumber % sampling ==0) {
			kdeModel.BuildBackgroundModel(img_input);
			counter++;
		}
		float sec = frameNumber /m_fps;
		int mm = sec / 60;
		int ss = sec - mm*60;
		wxString str;
		str.Printf("BG Frame No. %d, %02d:%02d", frameNumber, mm, ss);
		m_statusBar->SetStatusText(str, 3);
		
		frameNumber ++;
	}while(counter < nTrainingFrames);
	kdeModel.CreateBackgroundImage();
	
	if(bAbort) return;
	
	vidCap.release();
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	
	m_bStopProcess = false;
	cv::Mat matOut(m_height, m_width, CV_8UC1);
	
	do {
		if(m_bStopProcess)  break;	
		
		vidCap >> img_input;
		if (img_input.empty()) break;
		kdeModel.DetectMovingObject(img_input, matOut);
		
		cv::imshow("Input", img_input);
		cv::imshow( "MovingObject", matOut );
		
		float sec = frameNumber /m_fps;
		int mm = sec / 60;
		int ss = sec - mm*60;
		wxString str;
		str.Printf("Frame No. %d, %02d:%02d", frameNumber, mm, ss);
		m_statusBar->SetStatusText(str, 3);
		
		frameNumber ++;
		if(cv::waitKey(m_waitTime) >= 0) break;
		
	}while(1);
}
void MainFrame::OnVideoFGPixels(wxCommandEvent& event)
{
	DeleteContents();
	
	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	readVideoProperties(vidCap);
	readControlValues();
	
	SaveGlobalPara();

	wxString msg;
	msg << "\nLoad ... " << m_Filename << " OK\n";
	myMsgOutput(msg );
	
	wxString strAlgo = "FrameDifferenceBGS";
	IBGS *bgsFD = createBGSObj(strAlgo);
	if(bgsFD==NULL) {
		myMsgOutput( "No FrameDifferenceBGS algorithm create\n");
		return;	
	} 
	
	strAlgo = "WeightedMovingMeanBGS";
	IBGS *bgsWMM = createBGSObj(strAlgo);
	if(bgsWMM==NULL) {
		myMsgOutput( "No WeightedMovingMeanBGS algorithm create\n");
		return;	
	} 	

	strAlgo = "AdaptiveBackgroundLearning";
	IBGS *bgsABL = createBGSObj(strAlgo);
	if(bgsWMM==NULL) {
		myMsgOutput( "No AdaptiveBackgroundLearning algorithm create\n");
		return;	
	} 
	
	bgslibrary::PreProcessor* pPreProcessor = new bgslibrary::PreProcessor;

	int frameNumber = 0;
	
	cv::Mat img_input;
	cv::Mat img_prep;
	cv::Mat mMovingObjFD;
	cv::Mat mMovingObjWMM;
	cv::Mat mMovingObjABL;
    cv::Mat mbkgmodel;
	m_bStopProcess = false;

	while(frameNumber < m_startFrame){
		frameNumber++;	
		vidCap >> img_input;
		if (img_input.empty()) break;
	}
	FILE *fp = fopen("d:\\tmp\\nonzeroPixels.csv", "w");
	fprintf(fp, "imgSize, frameNumber, FD, WMM, ABL, rFD, rWMM, rABL\n");
    do
    {
		if(m_bStopProcess)  break;


		vidCap >> img_input;
		if (img_input.empty()) break;
		if(frameNumber % m_Sampling) continue;
		
//		cv::imshow("Input", img_input);
		pPreProcessor->process(img_input, img_prep, m_bLeftSide);

		bgsFD->process(img_prep, mMovingObjFD, mbkgmodel);
		bgsWMM->process(img_prep, mMovingObjWMM, mbkgmodel);
		bgsABL->process(img_prep, mMovingObjABL, mbkgmodel);
		
		int imgSize = img_prep.rows * img_prep.cols;
		int nonZeroFD = countNonZero(mMovingObjFD);
		int nonZeroWMM = countNonZero(mMovingObjWMM);
		int nonZeroABL = countNonZero(mMovingObjABL);
		
		float sec = frameNumber /m_fps;
		int mm = sec / 60;
		int ss = sec - mm*60;
		wxString str;
		str.Printf("Frame No. %d, %02d:%02d", frameNumber, mm, ss);
		m_statusBar->SetStatusText(str, 3);
		
		float ratio = (float)nonZeroWMM/imgSize;
		if( ratio > 0.22) {
			myMsgOutput( "too many nonZero in frame %d,  %02d:%02d, ratio %f\n", frameNumber, mm, ss, ratio);
			break;
		}
		fprintf(fp, "%d, %d, %d, %d, %d, %f, %f, %f\n", imgSize, frameNumber, nonZeroFD, nonZeroWMM, nonZeroABL,
			(float)nonZeroFD/imgSize, (float)nonZeroWMM/imgSize, (float)nonZeroABL/imgSize);	
	
		frameNumber++;
		if(cv::waitKey(m_waitTime) >= 0) break;
		
	}while(1);	
	fclose(fp);
	
	delete bgsABL;	
	delete bgsFD;	
	delete bgsWMM;	
	delete pPreProcessor;

	myMsgOutput( "generate nonzeroPixels.csv ok\n");
}
void MainFrame::OnProfileClassification(wxCommandEvent& event)
{
	int  silenceLen = 300;
	double silenceTh = 0.00015;
	
	FILE *fp = fopen("d:\\tmp\\nonzeroPixels.csv", "r");
	if(fp == NULL) {
		myMsgOutput( "cannot open nonzeroPixels.csv\n");		
		wxMessageBox( "cannot open nonzeroPixels.csv","Error", wxICON_ERROR);
		return;		
	}	
	
	vector<Profile>  vProfile;
	char title [200];
	fgets(title, 200, fp );
	while(!feof(fp)) {
		int nonZeroWMM, frameNumber;
		float ratioWMM;
		int n = fscanf(fp, "%*d,%d,%*d,%d,%*d,%*f,%f,%*f", &frameNumber, &nonZeroWMM, &ratioWMM);
		if(n!=3)  break;
		
		Profile p;
		p.frameno = frameNumber;
		p.value = nonZeroWMM;
		p.ratio = ratioWMM;
		vProfile.push_back(p);
	}
	fclose(fp);
	
	vector<FrameType>  vFrameType;		
	int start, end;
	start = end = -1;
	for(int i=0; i<vProfile.size()-silenceLen; i++) {
		double mean = 0;
		for(int k=0; k< silenceLen; k++) {
			mean += vProfile[i+k].ratio;
		}
		mean /= silenceLen;
		if(mean <silenceTh) {
			if(start < 0)
				start = i;
		}else if(start >=0) {
			end = i+silenceLen-1;
			FrameType ft;
			ft.start = start; //vProfile[start].frameno;
			ft.end = end; //vProfile[end].frameno;
			ft.frameType = 0;
			vFrameType.push_back(ft);
			start = -1;
		}
	}
/*	
	for(int i=0; i<vFrameType.size(); i++) {
		myMsgOutput("%d--%d\n", vProfile[vFrameType[i].start].frameno, vProfile[vFrameType[i].end].frameno);
	}
	myMsgOutput("before merge %d\n", vFrameType.size());
*/	
	// merge
	for(int i=1; i<vFrameType.size(); i++) {
		if(vFrameType[i].start<=vFrameType[i-1].end+1){
			vFrameType[i-1].end = vFrameType[i].end;
			vFrameType.erase(vFrameType.begin() +i);
			i--;
		}
	}
	
	// find lower point
	for(int i=0; i<vFrameType.size(); i++) {
		while(vProfile[vFrameType[i].start+1].value <  vProfile[vFrameType[i].start].value)
			vFrameType[i].start++;
			
		while(vProfile[vFrameType[i].end-1].value <  vProfile[vFrameType[i].end].value)
			vFrameType[i].end--;	
	}
	myMsgOutput ("after merge, and find lower ...\n");
	FILE* fpw = fopen("d:\\tmp\\frameType.csv", "w");
	
	for(int i=0; i<vFrameType.size(); i++) {
		float ssec = vProfile[vFrameType[i].start].frameno /m_fps;
		int smm = ssec / 60;
		int sss = ssec - smm*60;

		float esec = vProfile[vFrameType[i].end].frameno /m_fps;
		int emm = esec / 60;
		int ess = esec - emm*60;		
		
		float diffsec = esec - ssec;
		fprintf(fpw, "%d, %d, %d, %d, %d, %d, %f\n", 
			vProfile[vFrameType[i].start].frameno, vProfile[vFrameType[i].end].frameno,
			smm, sss, emm, ess, diffsec);
			
		myMsgOutput("%d--%d\t", vProfile[vFrameType[i].start].frameno, vProfile[vFrameType[i].end].frameno);
		myMsgOutput("%02d:%02d -- %02d:%02d, diff sec %.2f\n", smm, sss, emm, ess, diffsec);
	}			
	fclose(fpw);
	myMsgOutput("after merge %d\n", vFrameType.size());	
	myMsgOutput( "OnProfileClassification ok, size %d\n", vProfile.size());
	
}
void MainFrame::OnViewShowFrameType(wxCommandEvent& event)
{
	FILE *fp = fopen("d:\\tmp\\frameType.csv", "r");
	if(fp == NULL) {
		myMsgOutput( "cannot open frameType.csv\n");		
		wxMessageBox( "cannot open frameType.csv","Error", wxICON_ERROR);
		return;		
	}	
	vector<float>  vFrameDiffsec;
	char title [200];
	fgets(title, 200, fp );
	while(!feof(fp)) {
		float diffsec;
		int n = fscanf(fp, "%*d, %*d, %*d, %*d, %*d, %*d, %f", &diffsec);
		if(n!=1)  break;
		vFrameDiffsec.push_back(diffsec);
	}
	fclose(fp);	
	myMsgOutput( "frameType.csv.csv  frameType size %d\n",vFrameDiffsec.size() );
	
	_gnuplotInit(gPlotFrameType, "FrameDiffsec", -10, 150); // y min max
	
//	gPlotFrameType.set_xrange(0, 3000);
	_gnuplotLine(gPlotFrameType, "FrameDiffsec", vFrameDiffsec, "#000000ff");
}
void MainFrame::OnViewShowProfile(wxCommandEvent& event)
{
	FILE *fp = fopen("d:\\tmp\\nonzeroPixels.csv", "r");
	if(fp == NULL) {
		myMsgOutput( "cannot open nonzeroPixels.csv\n");		
		wxMessageBox( "cannot open nonzeroPixels.csv","Error", wxICON_ERROR);
		return;		
	}	
	vector<int>  vProfile;
	char title [200];
	fgets(title, 200, fp );
	while(!feof(fp)) {
		int nonZeroWMM, frameNumber;
		float ratioWMM;
		int n = fscanf(fp, "%*d,%d,%*d,%d,%*d,%*f,%f,%*f", &frameNumber, &nonZeroWMM, &ratioWMM);
		if(n!=3)  break;
		vProfile.push_back(nonZeroWMM);
	}
	fclose(fp);	
	myMsgOutput( "nonzeroPixels.csv  Profile size %d\n",vProfile.size() );
	
	_gnuplotInit(gPlotProfile, "Profile", -500, 4500); // y min max
	
	gPlotProfile.set_xrange(0, 3000);
	_gnuplotLine(gPlotProfile, "Profile", vProfile, "#000000ff");
}
void MainFrame::OnProfileGaussianSmooth(wxCommandEvent& event)
{
	FILE *fp = fopen("d:\\tmp\\nonzeroPixels.csv", "r");
	if(fp == NULL) {
		myMsgOutput( "cannot open nonzeroPixels.csv\n");		
		wxMessageBox( "cannot open nonzeroPixels.csv","Error", wxICON_ERROR);
		return;		
	}	
	
	vector<int>  vFrameNo; 
	vector<float>  vProWMM, vSmoothWMM;
	vector<float>  vProABL, vSmoothABL;
	char title [200];
	fgets(title, 200, fp );
	while(!feof(fp)) {
		int nonZeroWMM, frameNumber, nonZeroABL;
		int n = fscanf(fp, "%*d,%d,%*d,%d,%d,%*f,%*f,%*f", &frameNumber, &nonZeroWMM, &nonZeroABL);
		if(n!=3)  break;
		vFrameNo.push_back(frameNumber);
		vProWMM.push_back(nonZeroWMM);
		vProABL.push_back(nonZeroABL);
	}
	fclose(fp);	
	myMsgOutput( "read nonzeroPixels.csv  Profile size %d\n",vProWMM.size() );	
	
	readControlValues();
	
	int ksize = m_nGauKSize;  // should be odd
	GaussianSmooth(vProWMM, vSmoothWMM, ksize);
	GaussianSmooth(vProABL, vSmoothABL, ksize);
	
	fp = fopen("d:\\tmp\\smoothProfile.csv", "w");
	for(int i=0; i<vProWMM.size(); i++)
		fprintf(fp, "%d, %f, %f, %f, %f\n", vFrameNo[i], vProWMM[i], vSmoothWMM[i], vProABL[i], vSmoothABL[i]);
	fclose(fp);
	
	myMsgOutput( "read nonzeroPixels.csv  Profile size %d, smooth ok\n",vProWMM.size() );	
	_gnuplotInit(gPlotProfile, "ProfileSmooth", -500, 4500); // y min max
	gPlotProfile.set_xrange(0, 3000);
	_gnuplotLine(gPlotProfile, "Profile", vProWMM, "#00ff0000");
	_gnuplotLine(gPlotProfile, "Smooth", vSmoothWMM, "#000000ff");
}


void MainFrame::GaussianSmooth(vector<float>& vecIn, vector<float>&venOut, int ksize)
{
	Mat mGaus1D = getGaussianKernel(ksize, -1, CV_64F);
	int inSize = vecIn.size();
	
	venOut.resize(inSize);
	float* pOut = venOut.data();
	memset(pOut, 0, sizeof(float)*inSize );
	double *gau = (double*)mGaus1D.data;
	for(int i=0; i<inSize - ksize; i++) {
		double avg = 0;
		for(int k=0; k<ksize; k++)
			avg += vecIn[i+k] * gau[k];
		//avg /= ksize;
		venOut[i+ksize/2] = avg;
	}
	
//	_OutputMat(mGaus1D, "d:\\tmp\\_gaus.csv", true);
}