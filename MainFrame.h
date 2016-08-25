#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

#include <wx/filehistory.h>

#include <vector>
#include <deque>
#include <opencv2/opencv.hpp>

#include "package_bgs/IBGS.h"



//#include "FrameProcessor.h"
//using namespace std;
//using namespace bgslibrary;

class MyConfigData{
public:	
	long 	m_frameStep;	
	
};

class Profile{
public:	
	Profile() { 
		motionType = 0; 
	}
	int frameno;
	int value;
	float ratio;
	int motionType;
};

class FrameType{
public:	
	FrameType() { 
		frameType = -1; 
	}	
	int start;
	int end;
	int frameType;
};
class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

	void BGS_KDE(cv::VideoCapture& vidCap);
	IBGS * createBGSObj(wxString& strBGS);
	
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
	void SetPageKDE();
	void SavePageKDE();
	void SetGlobalPara();
	void SaveGlobalPara();
	void readVideoProperties(cv::VideoCapture& vidCap);
	void readControlValues();
	void GaussianSmooth(std::vector<float>& vecIn, std::vector<float>&venOut, int ksize);
	
	static MainFrame *	m_pThis;
	wxFileHistory* 		m_FileHistory;
	wxString			m_Filename;

///////////////////// config
	MyConfigData	m_configData;
	wxString 		m_strSourcePath;

///////////////////// profile
	long	m_nGauKSize;
	long	m_nRangeXMin;
	long	m_nRangeXMax;	
	long	m_nRangeYMin;
	long	m_nRangeYMax;
	double	m_profileTh;
	long	m_nMinDuration;
	std::vector<int>  m_vFrameNo; 
	std::vector<float>  m_vProWMM;
	std::vector<float>  m_vSmoothWMM;		
	
///////////////////// bgs	
	bool m_bStopProcess;
	long m_waitTime;
	long m_startFrame;
	long m_Sampling;
	int  m_width;
	int  m_height;
	double m_fps;
	bool m_bLeftSide;
	
///////////////////// KDE
	int framesToLearn;	
	int SequenceLength;
	int TimeWindowSize;
	int SDEstimationFlag;
	int lUseColorRatiosFlag;
	double th;
	double alpha;

///////////////////// Preprocessor
    bool equalizeHist;
    bool gaussianBlur;
    bool bShowPreprocess;	

protected:
    virtual void OnProfileGaussianSmooth(wxCommandEvent& event);
    virtual void OnViewShowFrameType(wxCommandEvent& event);
    virtual void OnViewShowProfile(wxCommandEvent& event);
    virtual void OnProfileClassification(wxCommandEvent& event);
    virtual void OnVideoFGPixels(wxCommandEvent& event);
    virtual void OnBackgroundKDE(wxCommandEvent& event);
    virtual void OnVideoExtractFrames(wxCommandEvent& event);
    virtual void OnVideoStop(wxCommandEvent& event);
    virtual void OnVideoFrameProcessor(wxCommandEvent& event);
    virtual void OnVideoBGSProcess(wxCommandEvent& event);
    virtual void OnBookPageChanged(wxAuiNotebookEvent& event);
    virtual void OnViewMsgPane(wxCommandEvent& event);
    virtual void OnFileOpen(wxCommandEvent& event);
};
#endif // MAINFRAME_H
