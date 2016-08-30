#ifndef DLGEXTRACTFRAME_H
#define DLGEXTRACTFRAME_H
#include "extract_frame.h"
#include <string>
class DlgExtractFrame : public DlgExtractFrameBase
{
public:
    DlgExtractFrame(wxWindow* parent);
    virtual ~DlgExtractFrame();
	
	void getParam(std::string & outpath, int &fromMM, int &fromSS, int &toMM, int &toSS);
};
#endif // DLGEXTRACTFRAME_H
