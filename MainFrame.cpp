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

#define V_WIDTH  172
#define V_HEIGHT 230

using namespace std;
using namespace cv;
using namespace bgslibrary;

Gnuplot gPlotProfile("lines");
Gnuplot gPlotFType("lines");
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
	int statusWidth[4] = {200, 140, 140, 140};
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
	m_bShowPreprocess = m_checkBoxShowPreprocess->GetValue();

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

void MainFrame::OnBookPageChanged(wxAuiNotebookEvent& event)
{
	int tab = m_auiBook->GetSelection();
	myMsgOutput("OnBookPageChanged %d\n", tab);
}

void MainFrame::PreProcessor(const cv::Mat &img_input, cv::Mat &img_output, bool bLeftSide)
{
	cv::Mat mROI;
	
	if(bLeftSide) {
		mROI = img_input(cv::Range(0, V_HEIGHT), cv::Range(0, V_WIDTH));		
	}else 
		mROI = img_input(cv::Range(0, V_HEIGHT), cv::Range(V_WIDTH, img_input.cols));
		
    cv::cvtColor(mROI, img_output, CV_BGR2GRAY);
    cv::GaussianBlur(img_output, img_output, cv::Size(3, 3), 1.5);
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
		PreProcessor(img_input, img_prep, m_bLeftSide);

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
	
	
	str = m_textCtrlRangeXMin->GetValue();
	str.ToLong(&m_nRangeXMin);

	str = m_textCtrlRangeXMax->GetValue();
	str.ToLong(&m_nRangeXMax);	
	
	str = m_textCtrlRangeYMin->GetValue();
	str.ToLong(&m_nRangeYMin);

	str = m_textCtrlRangeYMax->GetValue();
	str.ToLong(&m_nRangeYMax);	
	
	str = m_textCtrlProfileTh->GetValue();
	str.ToDouble(&m_profileTh);
	
	str = m_textCtrlMinDuration->GetValue();
	str.ToLong(&m_nMinDuration);
	
	m_bLeftSide = m_radioButtonLeftSide->GetValue();
	m_bShowPreprocess = m_checkBoxShowPreprocess->GetValue();
/*	
	wxString msg;
	msg << "readControlValues ... \n";
	msg << "\t wait time: " << m_waitTime << " ms\n";
	msg << "\t start frame: " << m_startFrame << ", sampling: " << m_Sampling << "\n";
	msg << "\t LeftSide " << m_bLeftSide << "\n";
	msg << "\t GauKSize " << m_nGauKSize << "\n";
	myMsgOutput(msg );	
	 */ 
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
	
	string outFilename; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	outFilename = "~/tmp/nonzeroPixels.csv";
#else
	outFilename = "d:\\tmp\\nonzeroPixels.csv";
#endif
	
	FILE *fp = fopen(outFilename.c_str(), "w");
	fprintf(fp, "imgSize, frameNumber, FD, WMM, ABL, rFD, rWMM, rABL\n");
    do
    {
		if(m_bStopProcess)  break;


		vidCap >> img_input;
		if (img_input.empty()) break;
		if(frameNumber % m_Sampling) continue;
		
//		cv::imshow("Input", img_input);
		PreProcessor(img_input, img_prep, m_bLeftSide);

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

	myMsgOutput( "generate nonzeroPixels.csv ok\n");
}

void MainFrame::OnViewShowFrameType(wxCommandEvent& event)
{
	string filename; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	filename = "~/tmp/frameType.csv";
#else
	filename = "d:\\tmp\\frameType.csv";
#endif
	
	FILE *fp = fopen(filename.c_str(), "r");
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
	
//	_gnuplotInit(gPlotFrameType, "FrameDiffsec", -10, 150); // y min max
	
//	gPlotFrameType.set_xrange(0, 3000);
//	_gnuplotLine(gPlotFrameType, "FrameDiffsec", vFrameDiffsec, "#000000ff");
}
void MainFrame::OnViewShowProfile(wxCommandEvent& event)
{
	string filename; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	filename = "~/tmp/nonzeroPixels.csv";
#else
	filename = "d:\\tmp\\nonzeroPixels.csv";
#endif
	
	FILE *fp = fopen(filename.c_str(), "r");
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
	std::string filename;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	filename = "/Users/CCLee/tmp/nonzeroPixels.csv";
#else
	filename = "d:\\tmp\\nonzeroPixels.csv";
#endif

	if(m_profile.LoadProfileData(filename)==false) return;
	
	readControlValues();
	
	int ksize = m_nGauKSize;  // should be odd
	m_profile.GaussianSmooth(ksize);
	
	myMsgOutput("X range: %d..%d, Y range: %d..%d\n", m_nRangeXMin, m_nRangeXMax, m_nRangeYMin, m_nRangeYMax);	
	
	char str[100];
	sprintf(str, "Gaussian smooth with ksize %d", ksize);
	_gnuplotInit(gPlotProfile, str, 1200, 300, m_nRangeYMin, m_nRangeYMax); // y min max
	gPlotProfile.set_xrange(m_nRangeXMin, m_nRangeXMax);
	_gnuplotLine(gPlotProfile, "Profile", m_profile.m_vSignalWMM, "#00ff0000");
	_gnuplotLine(gPlotProfile, "Smooth", m_profile.m_vSmoothWMM, "#000000ff");	
}



void MainFrame::OnProfileClassification(wxCommandEvent& event)
{
	readControlValues();

	m_profile.Classification(m_nMinDuration, m_profileTh, m_fps);
	m_profile.PlotClassificationResult(gPlotProfile);

}

void MainFrame::OnTextFrameNoEnter(wxCommandEvent& event)
{
	long frameno;
	wxString str;
	str = m_textCtrlFrameNo->GetValue();
	str.ToLong(&frameno);	
	
	float sec = frameno /m_fps;
	int mm = sec / 60;
	float ss = sec - mm*60;
	str.Printf("%02d:%05.2f", mm, ss);	
	
	m_textCtrlMMSS->SetValue(str);	
}
void MainFrame::OnTextMMSSEnter(wxCommandEvent& event)
{
	wxString str, str1;
	str = m_textCtrlMMSS->GetValue();
	char* cstr = str.char_str();
	int mm;
	float ss;
	int n = sscanf(cstr, "%d:%f", &mm, &ss);
	long frameno = (mm * 60 + ss)*m_fps;
	
	str1.Printf("%d", frameno);
	m_textCtrlFrameNo->SetValue(str1);
}
