#ifndef DLGEXTRACTFRAME_H
#define DLGEXTRACTFRAME_H
#include "extract_frame.h"

class DlgExtractFrame : public DlgExtractFrameBase
{
public:
    DlgExtractFrame(wxWindow* parent);
    virtual ~DlgExtractFrame();
	
	void getParam(long &fromMM, long &fromSS, long &toMM, long &toSS);
};
#endif // DLGEXTRACTFRAME_H
