//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "PaintButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TPaintButton *)
{
	new TPaintButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TPaintButton::TPaintButton(TComponent* Owner)
	: TPaintBox(Owner)
{
  FPicture = new TPicture();
}
//---------------------------------------------------------------------------
void __fastcall TPaintButton::SetPicture(TPicture* Value)
{
	delete FPicture;
	FPicture = NULL;
  FPicture = Value;
}
//---------------------------------------------------------------------------
void __fastcall TPaintButton::ConstrainedResize(int &MinWidth, int &MinHeight, int &MaxWidth, int &MaxHeight)
{
	//SetRadius(FRadius);

}
//---------------------------------------------------------------------------
namespace Paintbutton
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TPaintButton)};
		RegisterComponents(L"UI Set", classes, 0);
	}
}
//---------------------------------------------------------------------------
