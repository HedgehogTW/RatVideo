//#include "stdafx.h"
#include "OfeliSnake.h"
#include <ctime>         // for std::clock_t, std::clock() and CLOCKS_PER_SEC
#include <cstring>       // for std::memcpy

// an Open, Fast and Efficient Level set Implementation
// https://code.google.com/p/ofeli/

using namespace std;

COfeliSnake::COfeliSnake(cv::Mat& m)
{
	m_pAC = nullptr;

	if (m.type() == CV_8UC1)
		m.copyTo(m_mImg);
	else
		cvtColor(m, m_mImg, CV_BGR2GRAY);

	m_width = m.cols;
	m_height = m.rows;
	m_img_size = m_width * m_height;
	m_phi_init1_clean = nullptr;
	m_phi_init2_clean = nullptr;
	m_phi_init1 = nullptr;
	m_phi_init2 = nullptr;
}


COfeliSnake::~COfeliSnake()
{
	if (m_pAC != nullptr)	
		delete m_pAC;

	if (m_phi_init1_clean != nullptr)	
		delete [] m_phi_init1_clean;

	if (m_phi_init2_clean != nullptr)
		delete[] m_phi_init2_clean;

	if (m_phi_init1 != nullptr)
		delete[] m_phi_init1;

	if (m_phi_init2 != nullptr)
		delete[] m_phi_init2;
}
void COfeliSnake::create(bool hasCycle2_1, int kernel_length1,
	double sigma1, int Na1, int Ns1, int lambda_out1, int lambda_in1, int model)
{
	if (model==0)
		m_pAC = new ofeli::ACwithoutEdges(m_mImg.data, m_width, m_height, m_phi_init1_clean,
			hasCycle2_1, kernel_length1, sigma1, Na1, Ns1, lambda_out1, lambda_in1);
	else if (model ==1) // geodesic model
		m_pAC = new ofeli::GeodesicAC(m_mImg.data, m_width, m_height, m_phi_init1_clean,
			hasCycle2_1, kernel_length1, sigma1, Na1, Ns1);


}
void COfeliSnake::setInitialContour(vector<vector<cv::Point> >& contours)
{
	m_phi_init1_clean = new char[m_img_size];
	m_phi_init2_clean = new char[m_img_size];
	m_phi_init1 = new char[m_img_size];
	m_phi_init2 = new char[m_img_size];

	cv::Mat mT(m_mImg.size(), CV_8UC1, Scalar::all(0));
	
	cv::drawContours(mT, contours, 0, cv::Scalar(255), CV_FILLED, 8);

	for (int offset = 0; offset < m_img_size; offset++)
	{
		if (*(mT.data + offset) <= 100)
		{
			m_phi_init1[offset] = 1;
			m_phi_init2[offset] = 1;
		}
		else
		{
			m_phi_init1[offset] = -1;
			m_phi_init2[offset] = -1;
		}
	}

	clean_boundaries(m_phi_init1, m_phi_init1_clean);
	clean_boundaries(m_phi_init2, m_phi_init2_clean);
}

void COfeliSnake::clean_boundaries(char* phi, char* phi_clean)
{
	if (phi != nullptr && phi_clean != nullptr)
	{
		int offset, x, y; // position

		for (offset = 0; offset < m_img_size; offset++)
		{
			if (phi[offset] == 1)
			{
				// offset = x+y*m_width <==>
				y = offset / m_width;
				x = offset - y*m_width;

				// nettoyage si la condition de voisinage aux frontières n'est pas respectée
				if (isRedundantPointOfLout(phi, x, y))
				{
					phi_clean[offset] = 3;
				}
				else
				{
					phi_clean[offset] = 1;
				}
			}

			if (phi[offset] == -1)
			{
				// offset = x+y*m_width <==>
				y = offset / m_width;
				x = offset - y*m_width;

				if (isRedundantPointOfLin(phi, x, y))
				{
					phi_clean[offset] = -3;
				}
				else
				{
					phi_clean[offset] = -1;
				}
			}
		}

		// efface l'affichage des listes
		//for (int n_out = 0; Lout_2[n_out] != list_end; n_out++)
		//{
		//	offset = Lout_2[n_out] * 3;

		//	image_phi_uchar[offset] = image_filter_uchar[offset];
		//	image_phi_uchar[offset + 1] = image_filter_uchar[offset + 1];
		//	image_phi_uchar[offset + 2] = image_filter_uchar[offset + 2];

		//	image_shape_uchar[offset] = image_filter_uchar[offset];
		//	image_shape_uchar[offset + 1] = image_filter_uchar[offset + 1];
		//	image_shape_uchar[offset + 2] = image_filter_uchar[offset + 2];
		//}

		//for (int n_in = 0; Lin_2[n_in] != list_end; n_in++)
		//{
		//	offset = Lin_2[n_in] * 3;

		//	image_phi_uchar[offset] = image_filter_uchar[offset];
		//	image_phi_uchar[offset + 1] = image_filter_uchar[offset + 1];
		//	image_phi_uchar[offset + 2] = image_filter_uchar[offset + 2];

		//	image_shape_uchar[offset] = image_filter_uchar[offset];
		//	image_shape_uchar[offset + 1] = image_filter_uchar[offset + 1];
		//	image_shape_uchar[offset + 2] = image_filter_uchar[offset + 2];
		//}

		//int n_out = 0;
		//int n_in = 0;
		//for (int offset = 0; offset < m_img_size; offset++)
		//{
		//	if (phi_clean[offset] == 1)
		//	{
		//		Lout_2[n_out++] = offset;
		//	}
		//	if (phi_clean[offset] == -1)
		//	{
		//		Lin_2[n_in++] = offset;
		//	}
		//}
		//Lout_2[n_out] = list_end;
		//Lin_2[n_in] = list_end;

	}
	return;
}

bool COfeliSnake::isRedundantPointOfLin(char* phi, int x, int y)
{
	// if ∃ a neighbor ∈ Lout | ∈ Rout

	if (y - 1 >= 0)
	{
		if (phi[x + (y - 1)*m_width] >= 0)
		{
			return false; // is not redundant point of Lin
		}
	}
	if (x - 1 >= 0)
	{
		if (phi[(x - 1) + y*m_width] >= 0)
		{
			return false; // is not redundant point of Lin
		}
	}
	if (x + 1 < m_width)
	{
		if (phi[(x + 1) + y*m_width] >= 0)
		{
			return false; // is not redundant point of Lin
		}
	}
	if (y + 1 < m_height)
	{
		if (phi[x + (y + 1)*m_width] >= 0)
		{
			return false; // is not redundant point of Lin
		}
	}

	// ==> ∀ neighbors ∈ Lin | ∈ Rin
	return true; // is redundant point of Lin
}

bool COfeliSnake::isRedundantPointOfLout(char* phi, int x, int y)
{
	// if ∃ a neighbor ∈ Lin | ∈ Rin

	if (y - 1 >= 0)
	{
		if (phi[x + (y - 1)*m_width] <= 0)
		{
			return false; // is not redundant point of Lout
		}
	}
	if (x - 1 >= 0)
	{
		if (phi[(x - 1) + y*m_width] <= 0)
		{
			return false; // is not redundant point of Lout
		}
	}
	if (x + 1 < m_width)
	{
		if (phi[(x + 1) + y*m_width] <= 0)
		{
			return false; // is not redundant point of Lout
		}
	}
	if (y + 1 < m_height)
	{
		if (phi[x + (y + 1)*m_width] <= 0)
		{
			return false; // is not redundant point of Lout
		}
	}

	// ==> ∀ neighbors ∈ Lout | ∈ Rout
	return true; // is redundant point of Lout
}


void COfeliSnake::showStepResult()
{
	cv::Mat  mBGR, mResult;
	const ofeli::list<int>* Lout1;
	const ofeli::list<int>* Lin1;
	int offset;

	if (m_mImg.type() == CV_8UC3)
		mBGR = m_mImg.clone();
	else if (m_mImg.type() == CV_8UC1)
		cvtColor(m_mImg, mBGR, CV_GRAY2BGR);
	else {
		cout << "unsupport image type" << endl;
		return;
	}
	cv::namedWindow("activeContour", CV_WINDOW_AUTOSIZE);

	while (!m_pAC->get_isStopped())
	{
		mBGR.copyTo(mResult);
		
		++(*m_pAC);
		Lout1 = &m_pAC->get_Lout();
		Lin1 = &m_pAC->get_Lin();

		for (auto it = Lout1->begin(); !it.end(); ++it)
		{
			offset = *it * 3;

			mResult.data[offset] = 0;
			mResult.data[offset + 1] = 255;
			mResult.data[offset + 2] = 0;
		}
		for (auto it = Lin1->begin(); !it.end(); ++it)
		{
			offset = *it * 3;

			mResult.data[offset] = 0;
			mResult.data[offset + 1] = 0;
			mResult.data[offset + 2] = 255;
		}		
		//imshow("activeContour", mResult);
/*
		if (waitKey(5) >= 0) {
			cout << "Stopped by user" << endl;
			break;
		}
		 */ 
	}
	//MessageBeep(MB_OK);
}

void COfeliSnake::showFinalResult()
{
	m_pAC->evolve();

	const ofeli::list<int>* Lout1;
	const ofeli::list<int>* Lin1;
	Lout1 = &m_pAC->get_Lout();
	Lin1 = &m_pAC->get_Lin();

	int offset;
	cv::Mat  mResult;
	if (m_mImg.type() == CV_8UC3)
		mResult = m_mImg.clone();
	else if (m_mImg.type() == CV_8UC1)
		cvtColor(m_mImg, mResult, CV_GRAY2BGR);
	else {
		cout << "unsupport image type" << endl;
		return;
	}

	for (auto it = Lout1->begin(); !it.end(); ++it)
	{
		offset = *it * 3;

		mResult.data[offset] = 0;
		mResult.data[offset + 1] = 255;
		mResult.data[offset + 2] = 0;
	}
	for (auto it = Lin1->begin(); !it.end(); ++it)
	{
		offset = *it * 3;

		mResult.data[offset] = 0;
		mResult.data[offset + 1] = 0;
		mResult.data[offset + 2] = 255;
	}

	//MessageBeep(MB_OK);
	//imshow("activeContour", mResult);

}

void COfeliSnake::RetrieveContour(vector<vector<cv::Point> > &vecContour, bool bInContour)
{
	const ofeli::list<int>* Lcontour;

	if (bInContour)
		Lcontour = &m_pAC->get_Lin();
	else
		Lcontour = &m_pAC->get_Lout();

	int offset;

	cv::Mat  mMask = cv::Mat::zeros(m_mImg.size(), CV_8UC1);
	for (auto it = Lcontour->begin(); !it.end(); ++it){
		int offset = *it;
		mMask.data[offset] = 255;
	}

	findContours(mMask, vecContour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}