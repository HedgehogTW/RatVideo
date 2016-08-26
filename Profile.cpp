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
//	vector<float>  vProABL, vSmoothABL;
	char title [200];
	fgets(title, 200, fp );
	while(!feof(fp)) {
		int nonZeroWMM, frameNumber, nonZeroABL;
		int n = fscanf(fp, "%*d,%d,%*d,%d,%*d,%*f,%*f,%*f", &frameNumber, &nonZeroWMM);
		if(n!=2)  break;
		m_vFrameNo.push_back(frameNumber);
		m_vSignalWMM.push_back(nonZeroWMM);
//		vProABL.push_back(nonZeroABL);
	}
	fclose(fp);	
	MainFrame::myMsgOutput( "read nonzeroPixels.csv  Profile size %d\n", m_vSignalWMM.size() );	
	return true;
	
}

void Profile::GaussianSmooth(int ksize)
{
	GaussianSmoothOneVariable(m_vSignalWMM, m_vSmoothWMM, ksize);

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
		return ;				
	}
	for(int i=0; i<m_vSignalWMM.size(); i++)
		fprintf(fp, "%d, %f, %f\n", m_vFrameNo[i], m_vSignalWMM[i], m_vSmoothWMM[i]);
	fclose(fp);	
	
	MainFrame::myMsgOutput( "Gaussian smooth with ksize %d\n",ksize );	
}
void Profile::GaussianSmoothOneVariable(vector<float>& vecIn, vector<float>&venOut, int ksize)
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
	for(int i=0; i<ksize/2; i++)
		venOut[i] = venOut[ksize/2];
	for(int i=inSize - ksize; i<inSize; i++)
		venOut[i] = venOut[inSize - ksize + ksize/2-1];
//	_OutputMat(mGaus1D, "d:\\tmp\\_gaus.csv", true);
}

void Profile::Classification(int  minSegment, double silenceTh, double fps)
{
	m_vSegment.clear();
		
	int start, end;
	start = end = -1;
	for(int i=0; i<m_vSmoothWMM.size(); i++) {
		if(m_vSmoothWMM[i] <silenceTh) {
			if(start < 0)
				start = i;
		}else if(start >=0) {
			end = i;
			if(end - start > minSegment) {
				FSegment ft;
				ft.start = start; //vProfile[start].frameno;
				ft.end = end; //vProfile[end].frameno;
				ft.frameType = 0;
				m_vSegment.push_back(ft);				
			}
			start = end = -1;
		}
	}
/*	
	for(int i=0; i<vFrameType.size(); i++) {
		myMsgOutput("%d--%d\n", vProfile[vFrameType[i].start].frameno, vProfile[vFrameType[i].end].frameno);
	}
	myMsgOutput("before merge %d\n", vFrameType.size());
*/	
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
	}
		
	for(int i=0; i<m_vSegment.size(); i++) {
		float ssec = m_vFrameNo[m_vSegment[i].start] /fps;
		int smm = ssec / 60;
		int sss = ssec - smm*60;

		float esec = m_vFrameNo[m_vSegment[i].end] /fps;
		int emm = esec / 60;
		int ess = esec - emm*60;		
		
		float diffsec = esec - ssec;
		if(fp!=NULL)
			fprintf(fp, "%d, %d, %d, %d, %d, %d, %f\n", 
				m_vFrameNo[m_vSegment[i].start], m_vFrameNo[m_vSegment[i].end],
				smm, sss, emm, ess, diffsec);
			
		MainFrame::myMsgOutput("%d--%d\t", m_vFrameNo[m_vSegment[i].start], m_vFrameNo[m_vSegment[i].end]);
		MainFrame::myMsgOutput("%02d:%02d -- %02d:%02d, diff sec %.2f\n", smm, sss, emm, ess, diffsec);
	}			
	fclose(fp);
	MainFrame::myMsgOutput("OnProfileClassification ok, FrameSegment %d\n", m_vSegment.size());	

}

void Profile::PlotClassificationResult(Gnuplot& gnuPlot)
{
	int  szSeg = m_vSegment.size();
	vector<int> vSegX(2*szSeg+1);
	vector<int> vSegY(2*szSeg+1);
	vSegX[0] = 0;
	vSegY[0] = 1500;

	
	for(int i=0; i<szSeg; i++) {
		vSegX[2*i+1] = m_vSegment[i].start;
		vSegY[2*i+1] = 1000;

		vSegX[2*i+2] = m_vSegment[i].end;
		vSegY[2*i+2] = 1500;
	}
	if(m_vSegment[szSeg-1].end < m_vSmoothWMM.size()) {
		vSegX.push_back(m_vSmoothWMM.size());
		vSegY.push_back(1500);
	}
//	_gnuplotInit(gPlotFType, "frame type", 1200, 300, m_nRangeYMin, m_nRangeYMax); // y min max
//	gPlotFType.set_xrange(m_nRangeXMin, m_nRangeXMax);
	_gnuplotSteps(gnuPlot, vSegX, vSegY, "#0000FF00", "frame type");		
}