//---------------------------------------------------------------------------
#ifndef RoundedPanelH
#define RoundedPanelH
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Gdiplus.h>
#include "UserMsgs.h"
using namespace Gdiplus;
//---------------------------------------------------------------------------
enum eBorderType {
  btNone,
	btNormal,
	btHorizontal,
	btVertical,
	btTopOnly,
	btBottomOnly,
	btLeftOnly,
  btRightOnly
};
//---------------------------------------------------------------------------
enum eRC : bool { LT, RT, LB, RB };
typedef System::Set<eRC, eRC::LT, eRC::RB> TRoundedCorner;
//---------------------------------------------------------------------------
class PACKAGE TRoundedPanel : public TCustomPanel
{
private:
	TColor FColor;
	TColor FBorderColor;
	unsigned int FRadius;
	eBorderType FBorderType;
	TRoundedCorner FRoundedCorner;
	TImageList* FImages;
	TChangeLink* FImagesChangeLink;
	TColor FShadowColorStart;
	TColor FShadowColorEnd;

	int FBodyX;
	int FBodyY;
	int FBodyWidth;
	int FBodyHeight;
	int FShadowWidth;
  int FBorderWidth;

	int FLastRoundedCorner;

  bool FTransparent;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR  gdiplusToken;

protected:
	void __fastcall Paint();
	void __fastcall PanelResize(TObject *Sender);
	void __fastcall OnImageChange(TObject *Sender);
	void __fastcall ConstrainedResize(int &MinWidth, int &MinHeight, int &MaxWidth, int &MaxHeight);
	void __fastcall DrawBound(int x1, int y1, int x2, int y2);

	void __fastcall DrawImageByIndex(int Index, int x, int y);

	void GetTopBoundPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Radius);
	void GetLeftBoundPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Radius);
	void GetRightBoundPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Radius);
	void GetBottomBoundPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Radius);

	void GetRectPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect);
	void GetRoundRectPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Dia);
	void FillRoundRect(Gdiplus::Graphics& graph,
										 Gdiplus::Rect Rect,
										 Gdiplus::Color BodyColor,
										 Gdiplus::Color BorderColor,
										 eBorderType BT,
										 int BorderWidth,
										 int Radius,
										 bool Gradient);

	void CheckMinSize();

	void __fastcall OnWmEraseBackground(TMessage& Msg);

	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_ERASE_BKGND, TMessage, OnWmEraseBackground)
	END_MESSAGE_MAP(TCustomPanel)

public:
	__fastcall TRoundedPanel(TComponent* Owner);
	__fastcall ~TRoundedPanel();

	void __fastcall SetBorderColor(TColor Value);
	void __fastcall SetColor(TColor Value);
	void __fastcall SetRadius(unsigned int Radius);
	void __fastcall SetBorderType(eBorderType Value);
	void __fastcall SetRoundedCorner(TRoundedCorner Value);
	void __fastcall SetImages(TImageList* Value);
  void __fastcall SetBorderWidth(int Value);
	void __fastcall SetShadowWidth(int Value);

	void __fastcall SetShadowColorStart(TColor Value);
	void __fastcall SetShadowColorEnd(TColor Value);

  void __fastcall SetTransparent(bool Value);

	TCanvas* __fastcall GetCanvas() {return Canvas;}

__published:

	__property TColor BorderColor = {read=FBorderColor, write=SetBorderColor};
	__property TColor Color = {read=FColor, write=SetColor};
	__property unsigned int Radius = {read=FRadius, write=SetRadius};
	__property eBorderType BorderType = {read=FBorderType, write=SetBorderType};
	__property TRoundedCorner RoundedCorner = {read=FRoundedCorner, write=SetRoundedCorner};
	__property TImageList* Images = {read=FImages, write=SetImages};
	__property int ShadowWidth = {read=FShadowWidth, write=SetShadowWidth};
	__property Vcl::Controls::TBorderWidth BorderWidth = {read=FBorderWidth, write=SetBorderWidth, default=0};

	__property  TColor ShadowColorStart = {read=FShadowColorStart, write=SetShadowColorStart, default=clBlack};
	__property  TColor ShadowColorEnd = {read=FShadowColorEnd, write=SetShadowColorEnd, default=clWhite};
  __property  bool Transparent = {read=FTransparent, write=SetTransparent, default=false};

};
//---------------------------------------------------------------------------
#endif
