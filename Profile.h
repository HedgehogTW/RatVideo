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

class Centroid{
public:		
	cv::Point2f cen;
	int 		lick;
};

class Profile
{
public:
	Profile();
	~Profile();

	bool LoadProfileData(std::string& filename);
	bool GaussianSmoothOneVariable(std::vector<float>& vecIn, std::vector<float>&venOut, int ksize);
	bool GaussianSmooth(int ksize);
	bool Classification(std::vector<float>& vecIn, int minSilence, int minActive, double silenceTh, double fps);
	void PlotClassificationResult(Gnuplot& gnuPlot);
	void generateTrainData(std::string& path, std::string& filename, cv::Point center, int trainNum);
	
	std::string 			m_DataPath;
	
	std::vector<int>  		m_vFrameNo; 
	std::vector<float>  	m_vSignalWMM;
	std::vector<float>  	m_vSmoothWMM;	
	std::vector<float>  	m_vSignalFD;
	std::vector<float>  	m_vSmoothFD;	
	std::vector<FSegment>  	m_vNoMotion;
	std::vector<Centroid>  	m_vCentroid;
};

#endif // PROFILE_H
