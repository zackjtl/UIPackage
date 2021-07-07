//---------------------------------------------------------------------------
#ifndef PngDrawerH
#define PngDrawerH
//---------------------------------------------------------------------------
#include "PngImageEx.h"
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

		void SetImage(TGraphic* Graphic);

__published:
	__property TBitmap* Bitmap = {read=FBitmap, write=SetBitmap, default=NULL};
	__property TNotifyEvent OnDoRepaint = {read=FOnDoRepaint, write=FOnDoRepaint, default=NULL};


};
//---------------------------------------------------------------------------
#endif
