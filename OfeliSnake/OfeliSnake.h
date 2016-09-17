#pragma once
#include "ac_withoutedges.hpp"
#include "geodesic_ac.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;
class COfeliSnake
{
public:
	COfeliSnake(cv::Mat& m);
	~COfeliSnake();

	void setInitialContour(vector<vector<cv::Point> >& contours);
	void clean_boundaries(char* phi, char* phi_clean);
	bool isRedundantPointOfLin(char* phi, int x, int y);
	bool isRedundantPointOfLout(char* phi, int x, int y);
	void create(bool hasCycle2_1, int kernel_length1, double sigma1, int Na1, int Ns1, int lambda_out1, int lambda_in1, int model);
	void showFinalResult();
	void showStepResult();
	void RetrieveContour(vector<vector<cv::Point> >& vecContour, bool bInContour = true);

	ofeli::ActiveContour* m_pAC;
	cv::Mat		m_mImg;
	int			m_width;
	int			m_height;
	int			m_img_size;
	char*		m_phi_init1_clean;
	char*		m_phi_init2_clean;
	char*		m_phi_init1;
	char*		m_phi_init2;
};

