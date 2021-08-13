//---------------------------------------------------------------------------
#ifndef PngDrawerH
#define PngDrawerH
//---------------------------------------------------------------------------
#include "PngImageEx.h"
#include <Gdiplus.h>
using namespace Gdiplus;
//---------------------------------------------------------------------------
class TPngDrawer : public TObject
{
protected:
	TBitmap* FBitmap;
	TNotifyEvent FOnDoRepaint;

public:
		TPngDrawer();

    // Callback functions
		__fastcall ~TPngDrawer();
		void __fastcall SetBitmap(TBitmap* Value);

		// Normal functions
		void DoRepaint();

		void AssignImage(TGraphic* Graphic);

__published:
	__property TBitmap* Bitmap = {read=FBitmap, write=SetBitmap, default=NULL};
	__property TNotifyEvent OnDoRepaint = {read=FOnDoRepaint, write=FOnDoRepaint, default=NULL};


};
//---------------------------------------------------------------------------
bool PngImageAssignToGdipBitmap(Gdiplus::Bitmap* Bitmap, TPngImage* PngImage, bool Premultiplied=true);
void PngImageAssignToBitmap(TBitmap* Bitmap, TPngImage* PngImage, bool Premultiplied=true);
void BitmapSmoothResize(TBitmap* OrgImage, int NewWidth, int NewHeight);
bool VCLBitmapAssignToGdipBitmap(Gdiplus::Bitmap* Bitmap, TBitmap* Image);
//---------------------------------------------------------------------------
#endif
