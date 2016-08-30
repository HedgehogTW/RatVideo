#include "DlgExtractFrame.h"
#include <wx/msgdlg.h>

DlgExtractFrame::DlgExtractFrame(wxWindow* parent)
    : DlgExtractFrameBase(parent)
{
}

DlgExtractFrame::~DlgExtractFrame()
{
}

void DlgExtractFrame::getParam(std::string & outpath, int &fromMM, int &fromSS, int &toMM, int &toSS)
{
	wxString str = m_textCtrlFrom->GetValue();
	char* cstr = str.char_str();
	int n = sscanf(cstr, "%d:%d", &fromMM, &fromSS);
	if(n!=2) {
		wxMessageBox( "From: Not time format","Error", wxICON_ERROR);
		return;
	}
	
	str = m_textCtrlTo->GetValue();
	cstr = str.char_str();
	n = sscanf(cstr, "%d:%d", &toMM, &toSS);
	if(n!=2) {
		wxMessageBox( "To: Not time format","Error", wxICON_ERROR);
		return;
	}
	

	outpath = m_textCtrlOutPath->GetValue();
	if(outpath.back() != '/' || outpath.back() != '\\')
		outpath += "/";	
}