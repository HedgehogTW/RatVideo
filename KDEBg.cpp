#include "KDEBg.h"
#include <memory.h>
#include <gsl/gsl_sort_float.h>
#include <gsl/gsl_sort.h>


KDEBg::KDEBg()
{
	//memset(gKDEBG, 0, sizeof(float)*VIDEO_W*VIDEO_H*KDE_BIN);
	m_bgModel = NULL;
}

KDEBg::~KDEBg()
{
	if(m_bgModel)
		delete [] m_bgModel;
}


void KDEBg::init(int w, int h, int nBandWidth, int nBGframes, double fgProb)
{
	m_nWidth = w;
	m_nHeight = h;

	m_nBGframes = nBGframes;
	m_nKnlBW = nBandWidth;
	m_szImage =  m_nWidth * m_nHeight;
	m_bgModel = new _BackGround[m_szImage];
	memset(m_bgModel, 0, sizeof(_BackGround)*m_szImage);

	m_kernel.resize(2*m_nKnlBW + 1); 
	for(int i=-m_nKnlBW; i<=m_nKnlBW; i++) {
		double x = (double)i/m_nKnlBW;
		m_kernel[i+m_nKnlBW] = (1.0-x*x)/ (m_nBGframes*m_nKnlBW);   // 或者後面再 /= m_frameCount*h
	}

	m_fg_prob = fgProb;
}

void KDEBg::BuildBackgroundModel(cv::Mat mIn)
{
	uchar * frame = mIn.data;
 
	for(int i=0; i<m_szImage; i++) {
		int v, pixelIdx;
		double p;

		pixelIdx = 3*i;

		v = frame[pixelIdx  ];
//		if(v-15 >=0) v-=15;
		for(int x=-m_nKnlBW; x<=m_nKnlBW; x++) {
			p = m_kernel[x+m_nKnlBW];
/*
			if((gray+x)>=0 && (gray+x) <=255)
				m_bgModel[i].h[h+x] += p;
			if((s+x)>=0 && (s+x) <=255)
				m_bgModel[i].s[s+x] += p;
*/
			if((v+x)>=0 && (v+x) <=255)
				m_bgModel[i].v[v+x] += p;
		}
	}

}

void KDEBg::CreateBackgroundImage(cv::Mat& mBg)
{

	cv::Mat bgImage(m_nHeight, m_nWidth, CV_8UC1);  // CV_8UC3
	uchar *pBGPixel = bgImage.data;
//#pragma omp parallel for private(maxH, maxS, maxV, maxIdxH, maxIdxS, maxIdxV)
//	float h[KDE_BIN_H];
//	float s[KDE_BIN_S];
	double v[KDE_BIN_V];
//	size_t hIdx[KDE_BIN_H];
//	size_t sIdx[KDE_BIN_S];
	size_t vIdx[KDE_BIN_V];	

//	float thH = 0.05;
//	float thS = 0.05;
	double thV = m_fg_prob;
		
	for(int i=0; i<m_szImage; i++) {
		double sum = 0;
/*		
		for(int j=0; j<KDE_BIN_H; j++) 
			sum += m_bgModel[i].h[j];
		for(int j=0; j<KDE_BIN_H; j++) 
			m_bgModel[i].h[j] /= sum;

		sum = 0;
		for(int j=0; j<KDE_BIN_S; j++) 
			sum += m_bgModel[i].s[j];
		for(int j=0; j<KDE_BIN_S; j++) 
			m_bgModel[i].s[j] /= sum;
*/
		sum = 0;
		for(int j=0; j<KDE_BIN_V; j++) 
			sum += m_bgModel[i].v[j];
		for(int j=0; j<KDE_BIN_V; j++) 
			m_bgModel[i].v[j] /= sum;
	}
	for(int i=0; i<m_szImage; i++) {
//		memcpy(h, m_bgModel[i].h, sizeof(h));
//		memcpy(s, m_bgModel[i].s, sizeof(s));
		memcpy(v, m_bgModel[i].v, sizeof(v));

//		gsl_sort_float_index(hIdx, h, 1, KDE_BIN_H);
//		gsl_sort_float_index(sIdx, s, 1, KDE_BIN_S);
		gsl_sort_index(vIdx, v, 1, KDE_BIN_V);

		double sum =0;
/*		
		for(int j=0; j<KDE_BIN_H; j++) {
			m_bgModel[i].fgbg_h[hIdx[j]] = 1;
			sum += m_bgModel[i].h[hIdx[j]];
			if(sum > thH) break;
		}
		sum =0;
		for(int j=0; j<KDE_BIN_S; j++) {
			m_bgModel[i].fgbg_s[sIdx[j]] = 1;
			sum += m_bgModel[i].s[sIdx[j]];
			if(sum > thS) break;
		}
		 */ 
		sum =0;
		for(int j=0; j<KDE_BIN_V; j++) {
			m_bgModel[i].fgbg_v[vIdx[j]] = 1;
			sum += m_bgModel[i].v[vIdx[j]];
			if(sum > thV) break;
		}
//		pBGPixel[3*i  ] = hIdx[KDE_BIN_H-1];
//		pBGPixel[3*i+1] = sIdx[KDE_BIN_S-1];
		pBGPixel[i] = vIdx[KDE_BIN_V-1];
	}

	bgImage.copyTo(mBg);
	// convert background image to HSV
	cv::imwrite("background.png", bgImage);
	//cv::namedWindow( "background", 1 );
//	cv::imshow( "background", bgImage );
}

void KDEBg::DetectMovingObject(cv::Mat& matIn, cv::Mat& matOut)
{
	uchar * inputFrame = matIn.data;
	uchar * maskOut = matOut.data;
//	memset(mask, 0, m_szImage);
	int  fH, fS, fV;
	
	fS = fH = 1;
//#pragma omp parallel for // useless
	for(int i=0; i<m_szImage; i++) {
//		double prob_h, prob_s, prob_v, prob_all_channels, maxP;
		double prob_v = m_bgModel[i].v[inputFrame[3*i]];
//		prob_all_channels = prob_r * prob_g * prob_b;
		//if( prob_h <= m_fg_prob && prob_s<= m_fg_prob && prob_v<= m_fg_prob)
//		fH = m_bgModel[i].fgbg_h[inputFrame[3*i]];
//		fS = m_bgModel[i].fgbg_s[inputFrame[3*i+1]];
//		fV = m_bgModel[i].fgbg_v[inputFrame[3*i+2]];

//		if(fH && fS && fV) {
		if(prob_v < m_fg_prob){
			if(matOut.type()==CV_8UC3) {
				maskOut[3*i] = 255;
				maskOut[3*i+1] = 255;
				maskOut[3*i+2] = 255;
			}else maskOut[i] = 255;
		}else{
			if(matOut.type()==CV_8UC3) {
				maskOut[3*i] = 0;
				maskOut[3*i+1] = 0;
				maskOut[3*i+2] = 0;
			}else maskOut[i] = 0;
		}

	}
}