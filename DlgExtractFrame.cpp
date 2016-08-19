#include "DlgExtractFrame.h"

DlgExtractFrame::DlgExtractFrame(wxWindow* parent)
    : DlgExtractFrameBase(parent)
{
}

DlgExtractFrame::~DlgExtractFrame()
{
}

void DlgExtractFrame::getParam(long &fromMM, long &fromSS, long &toMM, long &toSS)
{
	wxString str = m_textCtrlFromMM->GetValue();
	str.ToLong(&fromMM);	
	
	str = m_textCtrlFromSS->GetValue();
	str.ToLong(&fromSS);	
	
	str = m_textCtrlToMM->GetValue();
	str.ToLong(&toMM);	
	
	str = m_textCtrlToSS->GetValue();
	str.ToLong(&toSS);
}