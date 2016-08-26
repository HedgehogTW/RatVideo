#ifndef PROFILE_H
#define PROFILE_H

#include <vector>
#include <deque>
#include <opencv2/opencv.hpp>

#include "gnuplot_i.h"

class FSegment{
public:	
	FSegment() { 
		frameType = -1; 
	}	
	int start;
	int end;
	int frameType;
};

class Profile
{
public:
	Profile();
	~Profile();

	bool LoadProfileData(std::string& filename);
	void GaussianSmoothOneVariable(std::vector<float>& vecIn, std::vector<float>&venOut, int ksize);
	void GaussianSmooth(int ksize);
	void Classification(int minSegment, double silenceTh, double fps);
	void PlotClassificationResult(Gnuplot& gnuPlot);
	
	
	std::vector<int>  		m_vFrameNo; 
	std::vector<float>  	m_vSignalWMM;
	std::vector<float>  	m_vSmoothWMM;	
	std::vector<FSegment>  	m_vSegment;
};

#endif // PROFILE_H
