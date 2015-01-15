#include "Raster.h"
#include "afx.h"
#include <atlimage.h>

class CJPGRaster: public CRaster
{
private:
	CBitmap *map;
    CImage Image;
	CString FileName;
public:
    
    bool LoadFile(CString FileName="");
    void Paint(CPaintDC *dc);

	CJPGRaster(CString FileName);
	CJPGRaster(void);
	~CJPGRaster(void);
};
