#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

#include <wx/filehistory.h>

#include <vector>
#include <deque>
#include <opencv2/opencv.hpp>

#include "package_bgs/IBGS.h"

#include "Profile.h"


//#include "FrameProcessor.h"
//using namespace std;
//using namespace bgslibrary;

class MyConfigData{
public:	
	long 	m_frameStep;	
	
};


class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();


	IBGS * createBGSObj(wxString& strBGS);
	void PreProcessor(const cv::Mat &img_input, cv::Mat &img_output, bool bLeftSide, bool bSmooth=false);
	void PostProcess(cv::Mat& mBg, cv::Mat& mInput, std::vector<std::vector<cv::Point>>& ratContour);
	
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	
	static void myMsgOutput(wxString szFormat,...) {
		wxString strMsg;
		va_list argList;
		va_start(argList, szFormat);
		strMsg.PrintfV(szFormat, argList);
		va_end(argList); // 以上這幾行是用來處理類似printf 的參數

		m_pThis->m_richTextMsg->AppendText(strMsg);
		m_pThis->m_richTextMsg->ShowPosition(m_pThis->m_richTextMsg->GetLastPosition());
	}
	
	void DeleteContents();
	void OnMRUFile(wxCommandEvent& event);
	void openFile(wxString &dirName);
	void readVideoProperties(cv::VideoCapture& vidCap);
	void readControlValues();

	
	static MainFrame *	m_pThis;
	wxFileHistory* 		m_FileHistory;
	wxString			m_Filename;

///////////////////// config
	MyConfigData	m_configData;
	std::string 	m_DataPath;
	
/////////////////////KDE
	long	m_nKernelBW;
	double 	m_fgProb;
	
/////////////////////active contour
	long 	m_nLambdaIn;
	long  	m_nLambdaOut;
	
///////////////////// profile
	long	m_nGnuplotW;
	long	m_nGnuplotH;
	long	m_nGauKSize;
	long	m_nRangeXMin;
	long	m_nRangeXMax;	
	long	m_nRangeYMin;
	long	m_nRangeYMax;
	double	m_profileTh;
	long	m_nMinSilence;
	long	m_nMinActive;
	bool 	m_bClassWMM;	
	
	Profile	m_profile;
///////////////////// bgs	
	bool m_bShowPreprocess;
	bool m_bStopProcess;
	bool m_bPause;
	long m_waitTime;
	long m_startFrame;
	long m_Sampling;
	int  m_width;
	int  m_height;
	double m_fps;
	bool m_bLeftSide;
	
protected:
    virtual void OnVideoPause(wxCommandEvent& event);
    virtual void OnVideoCamShift(wxCommandEvent& event);
    virtual void OnTextMMSSEnter(wxCommandEvent& event);
    virtual void OnTextFrameNoEnter(wxCommandEvent& event);
    virtual void OnProfileGaussianSmooth(wxCommandEvent& event);
    virtual void OnViewShowFrameType(wxCommandEvent& event);
    virtual void OnViewShowProfile(wxCommandEvent& event);
    virtual void OnProfileClassification(wxCommandEvent& event);
    virtual void OnVideoFGPixels(wxCommandEvent& event);
    virtual void OnBackgroundKDE(wxCommandEvent& event);
    virtual void OnVideoExtractFrames(wxCommandEvent& event);
    virtual void OnVideoStop(wxCommandEvent& event);
    virtual void OnVideoFrameProcessor(wxCommandEvent& event);
    virtual void OnBookPageChanged(wxAuiNotebookEvent& event);
    virtual void OnViewMsgPane(wxCommandEvent& event);
    virtual void OnFileOpen(wxCommandEvent& event);
};
#endif // MAINFRAME_H
