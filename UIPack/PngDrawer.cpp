//---------------------------------------------------------------------------

#pragma hdrstop

#include "PngDrawer.h"
#include <string.h>
using namespace std;
//---------------------------------------------------------------------------
TPngDrawer::TPngDrawer()
{
  FBitmap = new TBitmap();
}
//---------------------------------------------------------------------------
__fastcall TPngDrawer::~TPngDrawer()
{
  FBitmap->Free();
}
//---------------------------------------------------------------------------
void __fastcall TPngDrawer::SetBitmap(TBitmap* Value)
{
	FBitmap->Assign(Value);
	DoRepaint();
}
//---------------------------------------------------------------------------
void TPngDrawer::SetImage(TGraphic* Graphic)
{
	if (typeid(*Graphic) == typeid(TPngImageEx) ||
			typeid(*Graphic) == typeid(TPngImage)) {
		//PngImageAssignToBitmap(FBitmap, (TPngImage*)G)
	}
	else {
		FBitmap->Assign(Graphic);
	}
	DoRepaint();
}
//---------------------------------------------------------------------------
void TPngDrawer::DoRepaint()
{
	if (FOnDoRepaint != NULL) {
		FOnDoRepaint(this);
	}
}
//---------------------------------------------------------------------------
