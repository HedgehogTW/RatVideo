#include "Profile.h"
#include <wx/msgdlg.h> 

#include "MainFrame.h"
#include "MyUtil.h"

using namespace std;
using namespace cv;

Profile::Profile()
{
}

Profile::~Profile()
{
}
bool Profile::LoadProfileData(std::string& filename)
{
	FILE *fp = fopen(filename.c_str(), "r");
	if(fp == NULL) {
		wxString msg = "cannot open "+filename + "\n";
		MainFrame::myMsgOutput( msg);		
		wxMessageBox( msg,"Error", wxICON_ERROR);
		return false;		
	}	
	
	m_vFrameNo.clear(); 
	m_vSignalWMM.clear();
	m_vSmoothWMM.clear();
	m_vSignalFD.clear();
	m_vSmoothFD.clear();
	char title [200];
	fgets(title, 200, fp );
	while(!feof(fp)) {
		int frameNumber, nonZeroWMM, nonZeroFD;
		int n = fscanf(fp, "%*d,%d,%d,%d,%*d,%*f,%*f,%*f", &frameNumber, &nonZeroFD, &nonZeroWMM);
		if(n!=3)  break;
		m_vFrameNo.push_back(frameNumber);
		m_vSignalFD.push_back(nonZeroFD);		
		m_vSignalWMM.push_back(nonZeroWMM);
	}
	fclose(fp);	
	MainFrame::myMsgOutput( "LoadProfileData(): read nonzeroPixels.csv, size %d\n", m_vSignalWMM.size() );	
	return true;
	
}

bool Profile::GaussianSmooth(int ksize)
{
	bool bRet;
	bRet = GaussianSmoothOneVariable(m_vSignalWMM, m_vSmoothWMM, ksize);
	if(! bRet) return false;
	bRet = GaussianSmoothOneVariable(m_vSignalFD, m_vSmoothFD, ksize);
	if(! bRet) return false;
	
	string filename; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	filename = "~/tmp/smoothProfile.csv";
#else
	filename = "d:\\tmp\\smoothProfile.csv";
#endif

	FILE *fp = fopen(filename.c_str(), "w");
	if(fp==NULL) {
		wxString msg = "cannot write "+filename + "\n";
		MainFrame::myMsgOutput( msg);		
		wxMessageBox( msg,"Error", wxICON_ERROR);
		return false;				
	}
	for(int i=0; i<m_vSignalWMM.size(); i++)
		fprintf(fp, "%d, %f, %f, %f, %f\n", m_vFrameNo[i], 
			m_vSignalWMM[i], m_vSmoothWMM[i], m_vSignalFD[i], m_vSmoothFD[i]);
	fclose(fp);	
	
	MainFrame::myMsgOutput( "Gaussian smooth with ksize %d\n",ksize );	
	
	return true;
}
bool Profile::GaussianSmoothOneVariable(vector<float>& vecIn, vector<float>&venOut, int ksize)
{
	Mat mGaus1D = getGaussianKernel(ksize, -1, CV_64F);
	int inSize = vecIn.size();
	
	if(inSize <=ksize) {
		wxString msg;
		msg.Printf("Not enough data, size %d", inSize);
		MainFrame::myMsgOutput(msg );
		return false;
	}
	
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
	for(int i=0; i<ksize/2; i++)
		venOut[i] = venOut[ksize/2];
	for(int i=inSize - ksize; i<inSize; i++)
		venOut[i] = venOut[inSize - ksize + ksize/2-1];
//	_OutputMat(mGaus1D, "d:\\tmp\\_gaus.csv", true);

	return true;
}

bool Profile::Classification(vector<float>& vecIn, int  minSilence, int minActive, double silenceTh, double fps)
{
	if(vecIn.size() <=0) {
		wxMessageBox( "No input data","Error", wxICON_ERROR);
		return false;
	}
	
	m_vNoMotion.clear();
	
	int start, end;
	start = end = -1;
	for(int i=0; i<vecIn.size(); i++) {
		if(vecIn[i] <silenceTh) {
			if(start < 0)
				start = i;
		}else if(start >=0) {
			end = i;
			if(end - start > minSilence) {
				FSegment ft;
				ft.start = start; //vProfile[start].frameno;
				ft.end = end; //vProfile[end].frameno;
				ft.frameType = 0;
				m_vNoMotion.push_back(ft);				
			}
			start = end = -1;
		}
	}
	// merge two silence segment
	for(int i=0; i<m_vNoMotion.size()-1; i++) {
		if(m_vNoMotion[i+1].start - m_vNoMotion[i].end < minActive){
			
			MainFrame::myMsgOutput("erase pulse %d\n", m_vNoMotion[i].end);
			
			m_vNoMotion[i].end = m_vNoMotion[i+1].end;
			m_vNoMotion.erase(m_vNoMotion.begin() +i+1);
			i--;
		}
	}
	
	string filename; 
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 
	filename = "~/tmp/frameType.csv";
#else
	filename = "d:\\tmp\\frameType.csv";
#endif

	FILE *fp = fopen(filename.c_str(), "w");
	if(fp==NULL) {
		wxString msg = "cannot write "+filename + "\n";
		MainFrame::myMsgOutput( msg);		
		wxMessageBox( msg,"Error", wxICON_ERROR);
		return false;
	}
		
	for(int i=0; i<m_vNoMotion.size(); i++) {
		float ssec = m_vFrameNo[m_vNoMotion[i].start] /fps;
		int smm = ssec / 60;
		int sss = ssec - smm*60;

		float esec = m_vFrameNo[m_vNoMotion[i].end] /fps;
		int emm = esec / 60;
		int ess = esec - emm*60;		
		
		float diffsec = esec - ssec;
		if(fp!=NULL)
			fprintf(fp, "%d, %d, %d, %d, %d, %d, %f\n", 
				m_vFrameNo[m_vNoMotion[i].start], m_vFrameNo[m_vNoMotion[i].end],
				smm, sss, emm, ess, diffsec);
			
		MainFrame::myMsgOutput("%d--%d\t", m_vFrameNo[m_vNoMotion[i].start], m_vFrameNo[m_vNoMotion[i].end]);
		MainFrame::myMsgOutput("%02d:%02d -- %02d:%02d, diff sec %.2f\n", smm, sss, emm, ess, diffsec);
	}			
	fclose(fp);
	MainFrame::myMsgOutput("OnProfileClassification ok, FrameSegment %d\n", m_vNoMotion.size());	
	return true;
}

void Profile::PlotClassificationResult(Gnuplot& gnuPlot)
{
	int  szSeg = m_vNoMotion.size();
	vector<int> vSegX(2*szSeg+1);
	vector<int> vSegY(2*szSeg+1);
	vSegX[0] = 0;
	vSegY[0] = 2000;

	
	for(int i=0; i<szSeg; i++) {
		vSegX[2*i+1] = m_vNoMotion[i].start;
		vSegY[2*i+1] = 1000;

		vSegX[2*i+2] = m_vNoMotion[i].end;
		vSegY[2*i+2] = 2000;
	}
	if(m_vNoMotion[szSeg-1].end < m_vSmoothWMM.size()) {
		vSegX.push_back(m_vSmoothWMM.size());
		vSegY.push_back(2000);
	}
//	_gnuplotInit(gPlotFType, "frame type", 1200, 300, m_nRangeYMin, m_nRangeYMax); // y min max
//	gPlotFType.set_xrange(m_nRangeXMin, m_nRangeXMax);
	_gnuplotSteps(gnuPlot, vSegX, vSegY, "#00000080", "frame type");		
}