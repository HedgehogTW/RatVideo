#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

#include <wx/filehistory.h>

#include <vector>
#include <deque>
#include <opencv2/opencv.hpp>


#include "FrameProcessor.h"

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

	static MainFrame *	m_pThis;
	wxFileHistory* 		m_FileHistory;
	wxString			m_Filename;

///////////////////// config
	MyConfigData	m_configData;
	wxString 		m_strSourcePath;

///////////////////// bgs	
//    bgslibrary::VideoCapture* videoCapture;
    bgslibrary::FrameProcessor* frameProcessor;
    bool useVideo;
    std::string filename;
    bool useCamera;
    int cameraId;
    long frameToStop;	
	

protected:
    virtual void OnBookPageChanged(wxAuiNotebookEvent& event);
    virtual void OnVideoBGSubtraction(wxCommandEvent& event);
    virtual void OnViewMsgPane(wxCommandEvent& event);
    virtual void OnFileOpen(wxCommandEvent& event);
};
#endif // MAINFRAME_H
