#ifndef KDEBG_H
#define KDEBG_H

#define KDE_BIN_H		181
#define KDE_BIN_S		256
#define KDE_BIN_V		256

#include <vector>
#include <opencv2/opencv.hpp>

struct _BackGround {
//	float h[KDE_BIN_H];	// h
//	float s[KDE_BIN_S];	// s
	float v[KDE_BIN_V];	// v

//	char fgbg_h[KDE_BIN_H];
//	char fgbg_s[KDE_BIN_S];
	char fgbg_v[KDE_BIN_V];

};

class KDEBg
{
public:
	KDEBg();
	~KDEBg();

	void	init(int w, int h, int nBandWidth, int nBGframes, float fgProb);
	void 	BuildBackgroundModel(cv::Mat mIn);
	void	CreateBackgroundImage();
	void	DetectMovingObject(cv::Mat& matIn, cv::Mat& matOut);
	
	int		m_nWidth;
	int		m_nHeight;
	int		m_szImage;
	int		m_nKnlBW;
	int		m_nBGframes;
	float	m_fg_prob;
	std::vector<float> m_kernel;
	_BackGround * m_bgModel;
};

#endif // KDEBG_H
