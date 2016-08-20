#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

#include <wx/filehistory.h>

#include <vector>
#include <deque>
#include <opencv2/opencv.hpp>

#include "package_bgs/IBGS.h"
#include "PreProcessor.h"
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
	void readProperties(cv::VideoCapture& vidCap);
	
	static MainFrame *	m_pThis;
	wxFileHistory* 		m_FileHistory;
	wxString			m_Filename;

///////////////////// config
	MyConfigData	m_configData;
	wxString 		m_strSourcePath;

///////////////////// bgs	
	cv::Mat m_mMask;
    cv::Mat m_mbkgmodel;
    bgslibrary::PreProcessor* m_pPreProcessor;

	bool m_bStopProcess;
	long m_waitTime;
	long m_startFrame;
	long m_Sampling;
	int  m_width;
	int  m_height;
	double m_fps;
	
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
