//---------------------------------------------------------------------------
#ifndef DrawFuncH
#define DrawFuncH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <Gdiplus.h>
using namespace Gdiplus;
//---------------------------------------------------------------------------
void DrawParentImage(TControl* Control, HDC DC, bool InvalidateParent = false);
//---------------------------------------------------------------------------
#endif
