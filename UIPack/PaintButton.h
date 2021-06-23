//---------------------------------------------------------------------------

#ifndef PaintButtonH
#define PaintButtonH
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TPaintButton : public TPaintBox
{
private:
  TPicture* FPicture;

protected:
  void __fastcall ConstrainedResize(int &MinWidth, int &MinHeight, int &MaxWidth, int &MaxHeight);

public:
	__fastcall TPaintButton(TComponent* Owner);
	void __fastcall SetPicture(TPicture* Value);

__published:
  __property TPicture* Picture = {read=FPicture, write = SetPicture};

};
//---------------------------------------------------------------------------
#endif
