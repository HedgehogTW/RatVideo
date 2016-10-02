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
#include "OfeliSnake/OfeliSnake.h"

#include "MyUtil.h"
#include "gnuplot_i.h"

#define V_WIDTH  172
#define V_HEIGHT 230
#define PAUSE_TIME 5000

using namespace std;
using namespace cv;
using namespace bgslibrary;

Gnuplot gPlotWMM("lines");
Gnuplot gPlotFD("lines");
Gnuplot gPlotView("lines");
Gnuplot gPlotCentroid("dots");
MainFrame *	MainFrame::m_pThis=NULL;

int lickRangeLow[] = {6026, 15392, 16345, 26793, 28358, 29164, 33445, 34680, 35113, 44345, 46753};
int lickRangeUp[]  = {6084, 15404, 16362, 27048, 28604, 29360, 34164, 35098, 35449, 44414, 46898};
	
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
	int statusWidth[4] = {100, 150, 140, 150};
	m_statusBar->SetFieldsCount(4, statusWidth);	
	
	wxConfigBase *pConfig = wxConfigBase::Get();
	m_FileHistory = new wxFileHistory(9);
	m_FileHistory->UseMenu(m_menuFile);
	m_FileHistory->AddFilesToMenu(m_menuFile);
	m_FileHistory->Load(*pConfig);


	this->Connect(wxID_FILE1, wxID_FILE9, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMRUFile), NULL, this);
	
	SetSize(700, 650);
	Center();	
	
	m_OpenFileName.Clear();
	m_bStopProcess = m_bPause = m_bEnableSnake = false;
	m_bShowPreprocess = m_checkBoxShowPreprocess->GetValue();

	m_fps = 29.97;
	
//	DeleteContents();
	wxString outpath, str; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	outpath = "./config";// + subpath;
//	m_Filename = "~/tmp/1218(4).AVI";
	*m_textCtrlDataPath << "~/tmp/";
#else
	outpath = ".\\config";
//	m_Filename = "d:\\tmp\\1218(4).AVI";
	*m_textCtrlDataPath << "D:/Dropbox/Rat_Lick/data/";
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
	
	wxString fileSpec = _T("gnuploti*");
	wxArrayString  	files;
	wxDir::GetAllFiles(".", &files,  fileSpec, wxDIR_FILES  );
	for(unsigned int i=0; i<files.size(); i++ ) {
		wxFileName fileName = files[i];
		wxString  fName = fileName.GetName();
		wxRemoveFile(fName);
	}
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

	wxString wildcard = "avi, mkv, and mp4 files (*.avi;*.mkv;*.mp4)|*.avi;*.mkv;*.mp4|\
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
	m_OpenFileName = fileName;		
//	m_strSourcePath = fileName;
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

void MainFrame::PreProcessor(const cv::Mat &img_input, cv::Mat &img_output, int LeftSide, bool bSmooth)
{
	cv::Mat mROI;
	
	switch(LeftSide) {
		case 0: // whole image
			img_input.copyTo(img_output);
			break;
		case 1:  // left
			mROI = img_input(cv::Range(0, V_HEIGHT), cv::Range(0, V_WIDTH));
			mROI.copyTo(img_output);
			break;
		case 2: // right
			mROI = img_input(cv::Range(0, V_HEIGHT), cv::Range(V_WIDTH, img_input.cols));
			mROI.copyTo(img_output);
			break;
	}

	if(bSmooth)
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
	readControlValues();
	
	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	readVideoProperties(vidCap);


//	destroyAllWindows();
	wxString strBGS = m_listBoxBGS->GetString(selBGS);	
	wxString msg;
	msg << "\nLoad ... " << m_Filename << " OK\nDo " << strBGS << ", whole image " << m_nLeftSide << "\n";
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
	cv::Mat mMedianObj;
    cv::Mat mbkgmodel;
	m_bStopProcess = m_bPause = false;

	while(frameNumber < m_startFrame){
		frameNumber++;	
		vidCap >> img_input;
		if (img_input.empty()) break;
	}
	bool firstTime = true;
    do
    {
		if(m_bPause)  {
			m_bPause = false;
			waitKey(PAUSE_TIME);
		}		
		if(m_bStopProcess)  break;

		vidCap >> img_input;
		if (img_input.empty()) break;
		if(frameNumber % m_Sampling) continue;
		
		cv::imshow("Input", img_input);
		PreProcessor(img_input, img_prep, m_nLeftSide);// m_bLeftSide);
		bgs->process(img_prep, mMovingObj, mbkgmodel);
		if(firstTime) {
			frameNumber++;
			firstTime = false;
			continue;
		}
		cv::imshow("mMovingObj", mMovingObj);	
		int nonZeroFD = countNonZero(mMovingObj);	
//		cv::medianBlur(mMovingObj, mMedianObj, 3);
//		cv::imshow("mMedianObj", mMedianObj);
		
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

void MainFrame::OnVideoPause(wxCommandEvent& event)
{
	m_bPause = true;
	myMsgOutput( "OnVideoPause: wait %d milliseconds.\n", PAUSE_TIME);
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
	str = m_textCtrlLambdaIn->GetValue();
	str.ToLong(&m_nLambdaIn);
	
	str = m_textCtrlLambdaOut->GetValue();
	str.ToLong(&m_nLambdaOut);	
	
	str = m_textCtrlGausKSize->GetValue();
	str.ToLong(&m_nGauKSize);
	
	str = m_textCtrlFrameWait->GetValue();
	str.ToLong(&m_waitTime);	
	
	str = m_textCtrlStartFrame->GetValue();
	str.ToLong(&m_startFrame);
	
	str = m_textCtrlSampling->GetValue();
	str.ToLong(&m_Sampling);
	if(m_Sampling<=0) m_Sampling = 1;	
	
	str = m_textCtrlGnuplotW->GetValue();
	str.ToLong(&m_nGnuplotW);

	str = m_textCtrlGnuplotH->GetValue();
	str.ToLong(&m_nGnuplotH);	
	
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
	
	str = m_textCtrlMinSilence->GetValue();
	str.ToLong(&m_nMinSilence);
	
	str = m_textCtrlMinActive->GetValue();
	str.ToLong(&m_nMinActive);

	str = m_textCtrlKernelBW->GetValue();
	str.ToLong(&m_nKernelBW);

	str = m_textCtrlFgProb->GetValue();
	str.ToDouble(&m_fgProb);
	
	m_bClassWMM = m_radioButtonClassWMM->GetValue();
	m_nLeftSide = m_radioBoxWholeRegion->GetSelection();
	m_bShowPreprocess = m_checkBoxShowPreprocess->GetValue();
	m_bEnableSnake = m_checkBoxEnableSnake->GetValue();
	
	m_DataPath = m_textCtrlDataPath->GetValue();
	if(m_DataPath.back() != '/' || m_DataPath.back() != '\\')
		m_DataPath += "/";
		
	if(m_OpenFileName.IsEmpty())
		m_Filename = m_DataPath + "1218(4).AVI";
	else
		m_Filename = m_OpenFileName;
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
		
	readControlValues();
	
	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	
	double fps = vidCap.get(CV_CAP_PROP_FPS);
	
	std::string outpath;
	int fromMM, toMM, fromSS, toSS, fromFrame, toFrame;
	
	dlg.getParam(outpath, fromMM, fromSS, toMM, toSS);
	
	fromFrame = (fromMM*60+fromSS)*fps;
	toFrame = (toMM*60+toSS)*fps;
	
	int frameNumber = 0;
	cv::Mat img_input;
	while(frameNumber < fromFrame){
		vidCap >> img_input;
		if (img_input.empty()) break;
		frameNumber++;	
	}
	
	wxString destPath, strFramename, outName, str; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	destPath.Printf("%sframe_%02d%02d_%02d%02d/", outpath, fromMM, fromSS, toMM, toSS);
#else
	destPath.Printf("%sframe_%02d%02d_%02d%02d/", outpath, fromMM, fromSS, toMM, toSS);
#endif	

	if(wxDirExists(destPath)==false) {
		if(wxMkdir(destPath)==false) {
			str.Printf("Create frame directory error, %s", destPath);
			wxLogMessage(str);
			return;
		}
	}
/*
	VideoWriter outputVideo; // For writing the video
	int width = img_input.cols; // Declare width here
    int height = img_input.rows; // Declare height here
    cv::Size S = Size(width, height); // Declare Size structure

    // Open up the video for writing
    const string filename = outpath + "_xxx.mp4"; // Declare name of file here

    // Declare FourCC code
    int fourcc = CV_FOURCC('H','2','6','4');

    // Declare FPS here
    bool bVideoWriter = outputVideo.open(filename, fourcc, fps, S);
	if (!bVideoWriter) 
		myMsgOutput("outputVideo.open Failed\n");
*/	
	while(frameNumber < toFrame){
		vidCap >> img_input;
		if (img_input.empty()) break;
		strFramename.Printf("f%02d%02d_%02d%02d_%06d.png", fromMM, fromSS, toMM, toSS, frameNumber);
		outName  = destPath + strFramename;
		imwrite(outName.ToStdString(), img_input);
		
//		if (bVideoWriter) 
//			outputVideo << img_input; 
		frameNumber++;	
	}	
//	outputVideo.release();
	wxString msg;
	msg.Printf("Extract frame from %02d:%02d (%d) to  %02d:%02d (%d)\n",fromMM, fromSS, fromFrame, toMM, toSS, toFrame );
	myMsgOutput(msg);
	wxBell();
}

void MainFrame::OnBackgroundKDE(wxCommandEvent& event)
{
	readControlValues();	
	//////// Profile Gaussian smooth
	std::string filename = m_DataPath + "_nonzeroPixels.csv";
	if(m_profile.LoadProfileData(filename)==false) 
		return;
	
	int ksize = m_nGauKSize;  // should be odd	
	if(m_profile.GaussianSmooth(ksize)==false) 
		return;
/*	
	myMsgOutput("X range: %d..%d, Y range: %d..%d\n", m_nRangeXMin, m_nRangeXMax, m_nRangeYMin, m_nRangeYMax);	
	
	char strTitle[100];
	sprintf(strTitle, "FD Gaussian smooth (ksize %d)", ksize);
	_gnuplotInit(gPlotFD, strTitle, m_nGnuplotW, m_nGnuplotH, m_nRangeYMin, m_nRangeYMax); // y min max
	gPlotFD.set_xrange(m_nRangeXMin, m_nRangeXMax);
	_gnuplotLine(gPlotFD, "FD Profile", m_profile.m_vSignalFD, "#00D2691E");
	_gnuplotLine(gPlotFD, "Smooth", m_profile.m_vSmoothFD, "#000000ff");
*/
////////////////////////////////////////////////////////	
	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}

	readVideoProperties(vidCap);

	
	wxString msg;
	msg.Printf("\nLoad %s, w%d x h%d, do KDE background ...\n", m_Filename, m_width, m_height);
	myMsgOutput(msg );
	

	cv::Mat img_input, mBg;
	KDEBg kdeModel;	
	int frameNumber = 0;
	
	int sampling = 20;
	int nTrainingFrames = 100; //nTrainMin*60*m_fps/sampling;	
	float thSmoothFD = 100;
	
	myMsgOutput("KDE training: %d frames, sampling %d, KernelBW %d, fgProb %f\n", nTrainingFrames, sampling, m_nKernelBW, m_fgProb);
	kdeModel.init(m_width, m_height, m_nKernelBW, nTrainingFrames, m_fgProb);

	int counter = 0;
	bool bAbort = false;	
	m_bStopProcess = m_bPause = false;
	do {
		if(m_bStopProcess)  {
			bAbort = true;
			break;			
		}
		vidCap >> img_input;
		if (img_input.empty()) break;
		if((frameNumber % sampling ==0) && (m_profile.m_vSmoothFD[frameNumber]>thSmoothFD)) {
			//PreProcessor(img_input, img_prep, m_bLeftSide);
			//int imgSize = img_prep.rows * img_prep.cols;
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
	
	wxString str;
	float sec = frameNumber /m_fps;
	int mm = sec / 60;
	float ss = sec - mm*60;
	str.Printf("Training stop at frame %d, %02d:%05.02f, collect frame %d, snake: lambdaOut %d, lambdaIn %d\n", 
		frameNumber, mm, ss, counter, m_nLambdaOut, m_nLambdaIn);	
	myMsgOutput(str);
	
	kdeModel.CreateBackgroundImage(mBg);
	cv::imshow( "background", mBg );
	
	if(bAbort) return;
	
	vidCap.release();
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	frameNumber = 0;
	while(frameNumber < m_startFrame){
		frameNumber++;	
		vidCap >> img_input;
		if (img_input.empty()) break;
	}
	
	m_bStopProcess = m_bPause = false;
	cv::Mat matOut(img_input.size(), CV_8UC1);
	cv::Mat mBgMask = Mat::zeros(img_input.size(), CV_8UC1);
	
	Mat kernel = Mat::ones(5, 5, CV_8U);
	Mat mMedian, mSub, mGray, mOtsu, mInGray;
	vector<vector<cv::Point> > ratContour;
	Scalar centroid, oldCentroid(0);
	FILE* fp = fopen("_centroid.csv", "w");
	do {
		if(m_bPause)  {
			m_bPause = false;
			waitKey(PAUSE_TIME);
		}
		if(m_bStopProcess)  break;	
		
		
		vidCap >> img_input;
		if (img_input.empty()) break;
		cv::cvtColor(img_input, mInGray, CV_BGR2GRAY);
//		kdeModel.DetectMovingObject(img_input, matOut);
		
		
		Mat  mROIBG, mROIIn;//, mROIBgMask;
		switch(m_nLeftSide) {
		case 0: // whole image
			mROIBG = mBg;
			mROIIn = img_input;
//			mROIBgMask = mBgMask;
			break;
		case 1:  // left
			mROIBG = mBg(cv::Range(0, V_HEIGHT), cv::Range(0, V_WIDTH));
			mROIIn = img_input(cv::Range(0, V_HEIGHT), cv::Range(0, V_WIDTH));
//			mROIBgMask = mBgMask(cv::Range(0, V_HEIGHT), cv::Range(0, V_WIDTH));
			break;
		case 2: // right
			mROIBG = mBg(cv::Range(0, V_HEIGHT), cv::Range(V_WIDTH, mBg.cols));
			mROIIn = img_input(cv::Range(0, V_HEIGHT), cv::Range(V_WIDTH, img_input.cols));
//			mROIBgMask = mBgMask(cv::Range(0, V_HEIGHT), cv::Range(V_WIDTH, img_input.cols));
			break;
		}
	
		PostProcess(mROIBG, mROIIn, ratContour);
		if(ratContour.size()==0) continue;
		centroid = cv::mean(ratContour[0]);
		if(m_nLeftSide ==2)  {
			centroid(0) += V_WIDTH;
			vector<cv::Point> &cc = ratContour[0];
			for(int i=0; i<cc.size(); i++)
				cc[i].x += V_WIDTH;
		}
		
//		UpdateBackground(mBg, mBgMask, mInGray, ratContour);
//		cv::imshow( "background", mBg );
//		cv::imshow("mask", mBgMask);
		
		fprintf(fp, "%.1f, %.1f\n", centroid(0), centroid(1));
		
		if(oldCentroid !=Scalar(0)) {
			Scalar sDist = centroid - oldCentroid;
			double dist = sqrt(sDist(0)*sDist(0) + sDist(1)*sDist(1));
			if(dist > V_HEIGHT/2)  {
				circle(img_input, Point(centroid(0), centroid(1)), 2, Scalar(0, 255, 0), 2);
				circle(img_input, Point(oldCentroid(0), oldCentroid(1)), 2, Scalar(0, 0, 255), 2);
				cv::imshow("Input", img_input);
				wxString str;
				str.Printf("_noOverlap_%d.bmp", frameNumber);
				imwrite(str.ToStdString(), img_input);
				//cv::waitKey(4000);
			}
		}
		

		cv::imshow("Input", img_input);
//		cv::imshow( "MovingObject", matOut );
		
		float sec = frameNumber /m_fps;
		int mm = sec / 60;
		int ss = sec - mm*60;
		wxString str;
		str.Printf("Frame No. %d, %02d:%02d", frameNumber, mm, ss);
		m_statusBar->SetStatusText(str, 3);
		
		frameNumber ++;
		if(cv::waitKey(m_waitTime) >= 0) break;
		oldCentroid = centroid;
	}while(1);
	fclose(fp);
	imwrite("_median.bmp", mMedian);
	imwrite("_otsu.bmp", mOtsu);
}

void MainFrame::UpdateBackground(Mat& mBg, Mat& mBgMask, Mat& mInput, vector<vector<cv::Point>>& ratContour)
{
	Mat kernel = Mat::ones(5, 5, CV_8U);
	mBgMask = 0;
	cv::drawContours(mBgMask, ratContour, 0, cv::Scalar(255,255,255), FILLED );	
	cv::dilate(mBgMask, mBgMask, kernel, Point(-1,-1), 4);
	mBgMask = 255 - mBgMask;
	
	int size = mBg.rows* mBg.cols;
	for(int i=0; i<size; i++) {
		uchar* pBg = mBg.data;
		uchar* pMask = mBgMask.data;
		uchar* pIn = mInput.data;
		if(*(pMask+i) != 0)
			*(pBg+i) = *(pBg+i)* 0.8 + *(pIn+i)* 0.2;
	}
}
void MainFrame::PostProcess(Mat& mBg, Mat& mInput, vector<vector<cv::Point>>& ratContour)
{
	ratContour.clear();
	
	Mat mMedian, mSub, mGray, mOtsu, mResult;
	
	cv::cvtColor(mInput, mGray, CV_BGR2GRAY);
	mSub = mGray - mBg;
	threshold(mSub, mSub, 0, 255, THRESH_TOZERO);
	cv::medianBlur(mSub, mMedian, 5);	
	int th = threshold(mMedian, mOtsu, 15, 255, THRESH_BINARY);//|THRESH_OTSU
	
	//// find maxima CC
	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	int mode = CV_RETR_LIST;
	int method = CV_CHAIN_APPROX_NONE;
	cv::Mat m = mOtsu.clone();
	findContours(m, contours, hierarchy, mode, method);
	
	int areaLower = 100;
	int areaUpper = 1300;
	double compactUpper = 120; // 50 or 60
	int maxIdx = -1;
	double maxArea = 0;
	float sumArea = 0;
	int numCont = contours.size();
	for(int j=0; j<contours.size(); j++) {
		
		float area = cv::contourArea(contours[j]);
		sumArea += area;
/*		
		if(area < areaLower || area > areaUpper) {
			contours.erase (contours.begin()+j);
			j--;
			continue;			
		}
		double compact = contours[j].size() * contours[j].size() / area;
		if(compact > compactUpper) {
			contours.erase (contours.begin()+j);
			j--;
			continue;			
		}
*/		
		if(area >= maxArea) {
			maxArea = area;
			maxIdx = j;
		}

	}	

	if(maxIdx < 0)  {
		myMsgOutput( "no contour\n");
		return;
	}
	
	float compact = contours[maxIdx].size() * contours[maxIdx].size() / maxArea;	
	float areaRatio = sumArea/ (mBg.rows*mBg.cols);
	wxString str;
	str.Printf("%.2f, %.2f, %.2f, %d", maxArea, compact, areaRatio, th);
	m_statusBar->SetStatusText(str, 1);
 
	cv::cvtColor(mOtsu, mResult, CV_GRAY2BGR);
	ratContour.push_back(contours[maxIdx]);
	
	vector<vector<cv::Point> > snakeContour;	
	if(m_bEnableSnake) {
		bool hasSmoothingCycle = true;
		int nNa = 30;  // 30 iterations
		int nNs = 3;
		int nNg = 5; // kernel_curve1
		double stddev = 2; // stddev of Gaussian Kernel
		int nModel = 0; // 0: graylevel
		bool bInnerCon = true;	
		COfeliSnake snake(mMedian);
		snake.setInitialContour(ratContour);
		snake.create(hasSmoothingCycle, nNg, stddev, nNa, nNs, m_nLambdaOut, m_nLambdaIn, nModel);
		snake.EvolveNoshow();
		snake.RetrieveContour(snakeContour, bInnerCon);
		cv::drawContours(mInput, snakeContour, 0, cv::Scalar(0, 0, 255), 2);	
		cv::drawContours(mResult, snakeContour, 0, cv::Scalar(0, 0, 255), 2);		
	}
	

	cv::drawContours(mInput, ratContour, 0, cv::Scalar(0, 255, 0), 1);	
	cv::drawContours(mResult, ratContour, 0, cv::Scalar(0, 255, 0), 1);
	
	cv::imshow("median", mMedian);
	cv::imshow("mResult", mResult);	
}
void MainFrame::OnVideoFGPixels(wxCommandEvent& event)
{
	DeleteContents();
	readControlValues();	
	
	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	readVideoProperties(vidCap);


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
	cv::Mat mMedianObj;
	cv::Mat mMovingObj;

    cv::Mat mbkgmodel;
	m_bStopProcess = m_bPause = false;

	while(frameNumber < m_startFrame){
		frameNumber++;	
		vidCap >> img_input;
		if (img_input.empty()) break;
	}
	
	string outFilename = m_DataPath + "_nonzeroPixels.csv"; 
	
	FILE *fp = fopen(outFilename.c_str(), "w");
	if(fp==NULL) {
		wxString str;
		str = "cannot write to " + outFilename;
		wxMessageBox( str);
		return;	
	}
	myMsgOutput(outFilename);
	fprintf(fp, "imgSize, frameNumber, FD, WMM, ABL, rFD, rWMM, rABL\n");
    do
    {
		if(m_bStopProcess)  break;


		vidCap >> img_input;
		if (img_input.empty()) break;
		if(frameNumber % m_Sampling) continue;
		
//		cv::imshow("Input", img_input);
		PreProcessor(img_input, img_prep, m_nLeftSide);
		int imgSize = img_prep.rows * img_prep.cols;
		
		bgsFD->process(img_prep, mMovingObj, mbkgmodel);
//		cv::medianBlur(mMovingObj, mMedianObj, 3);
		int nonZeroFD = countNonZero(mMovingObj);	

		bgsWMM->process(img_prep, mMovingObj, mbkgmodel);
//		cv::medianBlur(mMovingObj, mMovingObj, 3);
		int nonZeroWMM = countNonZero(mMovingObj);	
	
		bgsABL->process(img_prep, mMovingObj, mbkgmodel);
//		cv::medianBlur(mMovingObj, mMovingObj, 3);
		int nonZeroABL = countNonZero(mMovingObj);
		
		if(frameNumber<5) {
			 nonZeroFD = nonZeroWMM = nonZeroABL = 0;
		}
		
		float sec = frameNumber /m_fps;
		int mm = sec / 60;
		float ss = sec - mm*60;
		wxString str;
		str.Printf("Frame %d, %02d:%05.02f", frameNumber, mm, ss);
		m_statusBar->SetStatusText(str, 3);
		
		float ratio = (float)nonZeroWMM/imgSize;
		if( ratio > 0.22) {
			myMsgOutput( "\ntoo many nonZero in frame %d,  %02d:%05.02f, ratio %f, stop process\n", frameNumber, mm, ss, ratio);
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
	wxBell();
}

void MainFrame::OnViewShowFrameType(wxCommandEvent& event)
{
	string filename = m_DataPath + "frameType.csv"; 
	
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
	readControlValues();
	string filename = m_DataPath + "nonzeroPixels.csv";
	
	FILE *fp = fopen(filename.c_str(), "r");
	if(fp == NULL) {
		myMsgOutput( "cannot open nonzeroPixels.csv\n");		
		wxMessageBox( "cannot open nonzeroPixels.csv","Error", wxICON_ERROR);
		return;		
	}	

	
	vector<int>  vWMM, vFD;
	char title [200];
	fgets(title, 200, fp );
	while(!feof(fp)) {
		int nonZeroWMM, frameNumber, fd;
		int n = fscanf(fp, "%*d,%d,%d,%d,%*d,%*f,%*f,%*f", &frameNumber, &fd, &nonZeroWMM);
		if(n!=3)  break;
		vFD.push_back(fd);
//		vWMM.push_back(nonZeroWMM);
	}
	fclose(fp);	
	myMsgOutput( "nonzeroPixels.csv  Profile size %d\n",vWMM.size() );
	
	_gnuplotInit(gPlotView, "Profile", m_nGnuplotW, m_nGnuplotH, m_nRangeYMin, m_nRangeYMax); // y min max
	gPlotView.set_xrange(m_nRangeXMin, m_nRangeXMax);
//	_gnuplotLine(gPlotView, "Weighted Moving Mean", vWMM, "#00ff0000");
	_gnuplotLine(gPlotView, "FrameDiff", vFD, "#000000ff");		
}

void MainFrame::OnProfileGaussianSmooth(wxCommandEvent& event)
{
	readControlValues();
	std::string filename = m_DataPath + "_nonzeroPixels.csv";

	if(m_profile.LoadProfileData(filename)==false) 
		return;
	

	int ksize = m_nGauKSize;  // should be odd	
	if(m_profile.GaussianSmooth(ksize)==false) 
		return;
	
	myMsgOutput("X range: %d..%d, Y range: %d..%d\n", m_nRangeXMin, m_nRangeXMax, m_nRangeYMin, m_nRangeYMax);	
	
	char str[100];
	sprintf(str, "WMM Gaussian smooth (ksize %d)", ksize);
	_gnuplotInit(gPlotWMM, str, m_nGnuplotW, m_nGnuplotH, m_nRangeYMin, m_nRangeYMax); // y min max
	gPlotWMM.set_xrange(m_nRangeXMin, m_nRangeXMax);
	_gnuplotLine(gPlotWMM, "WMM Profile", m_profile.m_vSignalWMM, "#00DC143C");
	_gnuplotLine(gPlotWMM, "Smooth", m_profile.m_vSmoothWMM, "#000000ff");	
	
	sprintf(str, "FD Gaussian smooth (ksize %d)", ksize);
	_gnuplotInit(gPlotFD, str, m_nGnuplotW, m_nGnuplotH, m_nRangeYMin, m_nRangeYMax); // y min max
	gPlotFD.set_xrange(m_nRangeXMin, m_nRangeXMax);
	_gnuplotLine(gPlotFD, "FD Profile", m_profile.m_vSignalFD, "#00D2691E");
	_gnuplotLine(gPlotFD, "Smooth", m_profile.m_vSmoothFD, "#000000ff");		
}



void MainFrame::OnProfileClassification(wxCommandEvent& event)
{
	bool bRet;
	
	readControlValues();
	if(m_bClassWMM) {
		bRet = m_profile.Classification(m_profile.m_vSmoothWMM, m_nMinSilence, m_nMinActive, m_profileTh, m_fps);
		if(! bRet) return;
		m_profile.PlotClassificationResult(gPlotWMM);		
	}else {
		bRet = m_profile.Classification(m_profile.m_vSmoothFD, m_nMinSilence, m_nMinActive, m_profileTh, m_fps);
		if(! bRet) return;
		m_profile.PlotClassificationResult(gPlotFD);
	}
	
	myMsgOutput("Classification %d, min silence %d, min active %d, threshold %.2f\n", 
		m_bClassWMM, m_nMinSilence, m_nMinActive, m_profileTh);
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
	str.Printf("%02d:%05.02f", mm, ss);	
	
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
	if(n!=2) {
		wxMessageBox( "Not time format","Error", wxICON_ERROR);
		return;
	}
	long frameno = (mm * 60 + ss)*m_fps;
	
	str1.Printf("%d", frameno);
	m_textCtrlFrameNo->SetValue(str1);
}

void MainFrame::OnVideoCamShift(wxCommandEvent& event)
{

}
void MainFrame::OnProfileCentroid(wxCommandEvent& event)
{

	readControlValues();
	string filename = m_DataPath + "_centroid.csv";
	string str = "Load " + filename + "\n";
	myMsgOutput(str);
	FILE* fp = fopen(filename.c_str(), "r");
	if(fp==NULL) {
		wxMessageBox( "cannot open _centroid.csv","Error", wxICON_ERROR);
		return;				
	}
	int frame = 1;
	vector<Point2f> vecPoint;
	vector<Point2f> vecPointLick;
	while(!feof(fp)) {
		float x, y;
		int n = fscanf(fp, "%f,%f\n", &x, &y);
		if(n!=2) break;
		Point2f pt(x , 240- y);
		bool bLick = false;
		for(int i=0; i<11; i++) {
			if(frame >=lickRangeLow[i] && frame <= lickRangeUp[i]) {				
				bLick = true;
				break;
			}
		}
		if(bLick) 
			vecPointLick.push_back(pt);
		else 
			vecPoint.push_back(pt);
		
		frame++;
	}
	fclose(fp); 
	
	_gnuplotInit(gPlotCentroid, "Centroid", 600, 400, 0, 240); // y min max
	gPlotCentroid.set_xrange(0, 320);
	_gnuplotPoint(gPlotCentroid, vecPoint, "#000000ff", "", "dots ");	
	_gnuplotPoint(gPlotCentroid, vecPointLick, "#00ff0000", "", "dots ");	
	
	
	filename = m_DataPath + "background.png";
	Mat img = imread(filename);
	if(img.data==NULL) {
		wxMessageBox( "cannot open background.png","Error", wxICON_ERROR);
		return;			
	}
	myMsgOutput("lick frame %d, non-lick %d\n", vecPointLick.size(), vecPoint.size());
	for(int i=0; i<vecPoint.size(); i++) 
		circle(img, Point(vecPoint[i].x, 240-vecPoint[i].y), 0, Scalar(255, 0, 0));
	for(int i=0; i<vecPointLick.size(); i++) 
		circle(img, Point(vecPointLick[i].x, 240-vecPointLick[i].y), 0, Scalar(0, 0, 255));
	
	imshow("lick location", img);
	imwrite("lick_location.bmp", img);
}
void MainFrame::OnTrainData(wxCommandEvent& event)
{
	OnProfileGaussianSmooth(event);

	cv::VideoCapture vidCap;
	vidCap.open(m_Filename.ToStdString());
	if(vidCap.isOpened()==false) {
		myMsgOutput( "Load ... " + m_Filename + " ERROR\n");
		return;
	}
	readVideoProperties(vidCap);
	vidCap.release();
	
	
	cv::Point center;
	switch(m_nLeftSide) {
	case 0: // whole image
		center = Point(m_width/2, m_height/2);
		break;
	case 1:  // left
		center = Point(V_WIDTH/2, V_HEIGHT/2);
		break;
	case 2: // right
		center = Point((m_width-V_WIDTH)/2 + V_WIDTH, V_HEIGHT/2);
		break;
	}	
	
	myMsgOutput("video w %d, h%d, center (%d, %d)\n", m_width, m_height, center.x, center.y);
	
	string filename = "_centroid.csv";
	m_profile.generateTrainData(m_DataPath, filename, center);
	

}
