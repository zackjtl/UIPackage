//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "RoundedPanel.h"
#include "DrawFunc.h"
#include <Vcl.Buttons.hpp>
#pragma package(smart_init)
using namespace Gdiplus;
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TRoundedPanel *)
{
	new TRoundedPanel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TRoundedPanel::TRoundedPanel(TComponent* Owner)
	: TCustomPanel(Owner)
{
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	this->OnResize = PanelResize;
	FBorderColor = clGray;
	FRoundedCorner << eRC::LT << eRC::RT << eRC::LB << eRC::RB;
  FImagesChangeLink = new TChangeLink();
  FImagesChangeLink->OnChange = OnImageChange;
	FLastRoundedCorner = -1;

  ////this->DoubleBuffered = true;
}
//---------------------------------------------------------------------------
__fastcall TRoundedPanel::~TRoundedPanel()
{
	FImagesChangeLink->Free();
}
//---------------------------------------------------------------------------
Gdiplus::Color ColorFromTColor(TColor Val)
{
	Gdiplus::Color color;
	color.SetFromCOLORREF(Val);
	return color;
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::OnWmEraseBackground(TMessage& Msg)
{
	if (FTransparent) {
		if (Parent != NULL && Parent->DoubleBuffered) {

    }
  }

#if 0
  if FTransparent and PaintClientArea then
  begin
    if ( Parent <> nil ) and Parent.DoubleBuffered then
      PerformEraseBackground( Self, Msg.DC );
    DrawParentImage( Self, Msg.DC, True );

    // Do not call inherited -- prevents TWinControl.WMEraseBkgnd from
    // erasing background. Set Msg.Result to 1 to indicate background is painted
    // by the control.
    Msg.Result := 1;
  end
  else
    inherited;
#endif
}
//---------------------------------------------------------------------------
#define PointFrom(P, ShiftX, ShiftY) Gdiplus::Point(P.X + ShiftX, P.Y + ShiftY)
//---------------------------------------------------------------------------
void TRoundedPanel::GetTopBoundPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Radius)
{
	// diameter can't exceed width or height
	if(Radius > Rect.Width) return;

	// define a corner
	//Gdiplus::Rect corner(Rect.X, Rect.Y, Dia, Dia);

	// begin path
	Path->Reset();

	Gdiplus::Point l_t(Rect.X, Rect.Y);
	Gdiplus::Point r_t(Rect.X + Rect.Width - 1, Rect.Y);

	// top left
	Path->AddLine(PointFrom(l_t, (FRoundedCorner.Contains(eRC::LT) ? Radius : 0), 0), PointFrom(r_t, (FRoundedCorner.Contains(eRC::RT) ? -Radius : 0), 0));

	// end path
	Path->CloseFigure();
}
//---------------------------------------------------------------------------
void TRoundedPanel::GetLeftBoundPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Radius)
{
	// diameter can't exceed width or height
	if(Radius > Rect.Height) return;

	// begin path
	Path->Reset();

	Gdiplus::Point l_t(Rect.X, Rect.Y);
	Gdiplus::Point l_b(l_t.X, Rect.Y + Rect.Height - 1);

	// top left
	Path->AddLine(PointFrom(l_t, 0, (FRoundedCorner.Contains(eRC::LT) ? Radius : 0)), PointFrom(l_b, 0, (FRoundedCorner.Contains(eRC::LB) ? -Radius : 0)));

	// end path
	Path->CloseFigure();
}
//---------------------------------------------------------------------------
void TRoundedPanel::GetRightBoundPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Radius)
{
	// diameter can't exceed width or height
	if(Radius > Rect.Height) return;

	// begin path
	Path->Reset();

	Gdiplus::Point r_t(Rect.X + Rect.Width - 1, Rect.Y);
	Gdiplus::Point r_b(r_t.X, Rect.Y + Rect.Height - 1);

	// top left
	Path->AddLine(PointFrom(r_t, 0, (FRoundedCorner.Contains(eRC::RT) ? Radius : 0)), PointFrom(r_b, 0, (FRoundedCorner.Contains(eRC::RB) ? -Radius : 0)));

	// end path
	Path->CloseFigure();
}
//---------------------------------------------------------------------------
void TRoundedPanel::GetBottomBoundPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Radius)
{
	// diameter can't exceed width or height
	if(Radius > Rect.Width) return;

	// define a corner
	//Gdiplus::Rect corner(Rect.X, Rect.Y, Dia, Dia);

	// begin path
	Path->Reset();

	Gdiplus::Point r_b(Rect.X + Rect.Width - 1, Rect.Y + Rect.Height - 1);
	Gdiplus::Point l_b(Rect.X, Rect.Y + Rect.Height - 1);

	// top left
	Path->AddLine(PointFrom(l_b, (FRoundedCorner.Contains(eRC::LB) ? Radius : 0), 0), PointFrom(r_b, (FRoundedCorner.Contains(eRC::RB) ? -Radius : 0), 0));

	// end path
	Path->CloseFigure();
}
//---------------------------------------------------------------------------
void TRoundedPanel::GetRoundRectPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Dia)
{
	// diameter can't exceed width or height
	if(Dia > Rect.Width) Dia = Rect.Width;
	if(Dia > Rect.Height) Dia = Rect.Height;

	// define a corner
	Gdiplus::Rect corner(Rect.X, Rect.Y, Dia, Dia);

	// begin path
	Path->Reset();

	Gdiplus::Point l_t(Rect.X, Rect.Y);
	Gdiplus::Point r_t(Rect.X + Rect.Width - 1, Rect.Y);
	Gdiplus::Point r_b(r_t.X, Rect.Y + Rect.Height - 1);
	Gdiplus::Point l_b(l_t.X, r_b.Y);

	// top left

	if (FRoundedCorner.Contains(eRC::LT)) { // LT
		Path->AddArc(corner, 180, 90);
	}
	else {
		Path->AddLine(PointFrom(l_t, 0, -Dia), l_t);
		Path->AddLine(l_t, PointFrom(l_t, Dia, 0));
	}

	// tweak needed for Dia of 10 (dia of 20)
	if(Dia == 20)
	{
		corner.Width += 1;
		corner.Height += 1;
		Rect.Width -=1;
		Rect.Height -= 1;
	}

  // top right
	corner.X += (Rect.Width - Dia - 1);

	if (FRoundedCorner.Contains(eRC::RT)) {
		Path->AddArc(corner, 270, 90);
	}
	else {
		Path->AddLine(PointFrom(r_t, -Dia, 0), r_t);
		Path->AddLine(r_t, PointFrom(r_t, 0, Dia));
	}

	// bottom right
	corner.Y += (Rect.Height - Dia - 1);

	if (FRoundedCorner.Contains(eRC::RB)) {
		Path->AddArc(corner,   0, 90);
	}
	else {
		Path->AddLine(PointFrom(r_b, 0, -Dia), r_b);
		Path->AddLine(r_b, PointFrom(r_b, -Dia, 0));
	}

	// bottom left
	corner.X -= (Rect.Width - Dia - 1);

	if (FRoundedCorner.Contains(eRC::LB)) {
		Path->AddArc(corner,  90, 90);
	}
	else {
		Path->AddLine(PointFrom(l_b, -Dia, 0), l_b);
		Path->AddLine(l_b, PointFrom(l_b, 0, -Dia));
	}

	// end path
	Path->CloseFigure();
}
//---------------------------------------------------------------------------
void TRoundedPanel::FillRoundRect(Gdiplus::Graphics& graph,
																	Gdiplus::Rect Rect,
																	Gdiplus::Color BodyColor,
																	Gdiplus::Color BorderColor,
																	eBorderType BT,
																	int BWidth,
																	int Radius,
																	bool Gradient)
{
	if (BWidth > 0) {
		int bodyShrink = BWidth / 2;
		Rect.X += bodyShrink;
		Rect.Y += bodyShrink;
		Rect.Width -= (bodyShrink * 2);
    Rect.Height -= (bodyShrink * 2);
	}
  int dia = 2 * Radius;

	////Gdiplus::Graphics graph(canvas->Handle);
	graph.SetSmoothingMode(SmoothingModeHighQuality);

	// set to pixel mode
	int oldPageUnit = graph.SetPageUnit(UnitPixel);

	// define the pen

	Gdiplus::Pen pen(BorderColor, BWidth);
	pen.SetAlignment(PenAlignmentCenter);

	// get the corner path
	GraphicsPath path;

	// get path
	GetRoundRectPath(&path, Rect, dia);

	// fill
	//Gdiplus::SolidBrush brush(BodyColor);

	if (Gradient) {
		Gdiplus::PathGradientBrush brush(&path);
		brush.SetCenterColor(BodyColor);

		Gdiplus::PointF cent;
		brush.GetCenterPoint(&cent);

		// TODO: Allows to control the gradient center bias
		//cent.X += 60;
		//cent.Y += 80;

		brush.SetCenterPoint(cent);

		////BorderColor.SetValue(0x80000000 + BorderColor.GetValue());

		Gdiplus::Color surroundClrs[] = {BorderColor};
		int surroundColorCnt = 1;

		brush.SetSurroundColors(surroundClrs, &surroundColorCnt);
		graph.FillPath(&brush, &path);
	}
	else {
		Gdiplus::SolidBrush brush(BodyColor);
   	graph.FillPath(&brush, &path);
	}

	// draw the border last so it will be on top
	if (BT == btNormal)
		graph.DrawPath(&pen, &path);
	else {
		if (BT == btTopOnly || BT == btHorizontal) {
			GraphicsPath bound;
			GetTopBoundPath(&bound, Rect, Radius);
			graph.DrawPath(&pen, &bound);
		}
		if (BT == btBottomOnly || BT == btHorizontal) {
			GraphicsPath bound;
			GetBottomBoundPath(&bound, Rect, Radius);
			graph.DrawPath(&pen, &bound);
		}
		if (BT == btLeftOnly || BT == btVertical) {
			GraphicsPath bound;
			GetLeftBoundPath(&bound, Rect, Radius);
			graph.DrawPath(&pen, &bound);
		}
		if (BT == btRightOnly || BT == btVertical) {
			GraphicsPath bound;
			GetRightBoundPath(&bound, Rect, Radius);
			graph.DrawPath(&pen, &bound);
		}
  }
	// restore page unit
	graph.SetPageUnit((Unit)oldPageUnit);
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::Paint()
{
	int bias = Radius == 0 ? 1 : 0;

	int bodyX = 0;
	int bodyY = 0;
	int bodyWidth = ClientWidth-FShadowWidth;
  int bodyHeight = ClientHeight-FShadowWidth;

	Gdiplus::Color bodyColor = ColorFromTColor(Color);
	Gdiplus::Color borderColor = ColorFromTColor(BorderColor);
	Gdiplus::Color shadowColorStart = ColorFromTColor(FShadowColorStart);
	Gdiplus::Color shadowColorEnd = ColorFromTColor(FShadowColorEnd);

	//shadowColorStart.SetValue(shadowColorStart.GetValue() & 0x00FFFFFF);
	shadowColorEnd.SetValue(shadowColorEnd.GetValue() & 0x00FFFFFF);

#define DOUBLE_BUFFERING 1

#if defined(DOUBLE_BUFFERING)
	HDC memoryHDC = CreateCompatibleDC(Canvas->Handle);
	HBITMAP hMemoryBmp = CreateCompatibleBitmap(Canvas->Handle, Width, Height);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(memoryHDC, hMemoryBmp);

	// To copy the parent as background
	////HDC wnhdc = GetWindowDC(this->Parent->Handle);
	////BitBlt(memoryHDC, 0, 0, Width, Height, wnhdc, Left, Top, SRCCOPY);

	////DrawParentImage(this, memoryHDC, true);

	// Under trying
	int saveIdx = SaveDC(memoryHDC);
	Parent->Perform(WM_PRINTCLIENT, (NativeUInt)memoryHDC, (NativeInt)PRF_CLIENT);
	RestoreDC(memoryHDC, saveIdx);

	Gdiplus::Graphics graph(memoryHDC);
#else
	DrawParentImage(this, Canvas->Handle, true);
	Gdiplus::Graphics graph(Canvas->Handle);
#endif

	if (!FTransparent) {
		if (FShadowWidth)
			FillRoundRect(graph, Gdiplus::Rect(FShadowWidth, FShadowWidth, bodyWidth, bodyHeight), shadowColorStart, shadowColorEnd, btNone, 0, FRadius, true);

		FillRoundRect(graph, Gdiplus::Rect(bodyX, bodyY, bodyWidth, bodyHeight), bodyColor, borderColor, FBorderType, BorderWidth, FRadius, false);
	}
#if defined(DOUBLE_BUFFERING)
	::BitBlt(Canvas->Handle, 0, 0, ClientWidth, ClientHeight, memoryHDC, 0, 0, SRCCOPY);

	SelectObject(memoryHDC, hOldBmp);
	DeleteObject(hMemoryBmp);
	DeleteDC(memoryHDC);
#endif

#if 0
	Canvas->Font->Size = 12;
	Canvas->Font->Color = clYellow;
	Canvas->TextOutA(10, 10, Text);

	TRect rc;
	TTextFormats tf;
	tf << tfCenter;

	DrawImageByIndex(0, 0, 0);

	Canvas->Draw(0, 0, FImage->Picture->Graphic);
	FImage->Parent = this;

	////if (FPicture != NULL)
		////Canvas->Draw(0, 0, (TGraphic*)FPicture->Graphic);
#endif
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::DrawBound(int x1, int y1, int x2, int y2)
{
	if (FRadius == 0) {
		Canvas->Rectangle(x1, y1, x2+1, y2+1);
	}
	else {
		Canvas->RoundRect(x1, y1, x2, y2, FRadius, FRadius);
	}
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetColor(TColor Value)
{
	FColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetBorderColor(TColor Value)
{
	FBorderColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetShadowColorStart(TColor Value)
{
	FShadowColorStart = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetShadowColorEnd(TColor Value)
{
	FShadowColorEnd = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetTransparent(bool Value)
{
	FTransparent = Value;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetRadius(unsigned int Radius)
{

	FRadius = Radius;
	/*
	HRGN rgn;

	if (Radius == 0) {
		rgn = CreateRectRgn(0, 0, ClientWidth, ClientHeight);
		SetWindowRgn(Handle, rgn, true);
	}
	else {

		rgn = CreateRoundRectRgn(0, 0, ClientWidth, ClientHeight, FRadius-1 , FRadius-1);

		if (!FRoundedCorner.Contains(eRC::LT)) { // LT
			HRGN rectRgn = CreateRectRgn(0, 0, FRadius, FRadius);
			CombineRgn(rgn, rectRgn, rgn, RGN_OR);
		}
		if (!FRoundedCorner.Contains(eRC::RT)) { // RT
			HRGN rectRgn = CreateRectRgn(ClientWidth-FRadius, 0, ClientWidth-1, FRadius);
			CombineRgn(rgn, rectRgn, rgn, RGN_OR);
		}
		if (!FRoundedCorner.Contains(eRC::LB)) { // LB
			HRGN rectRgn = CreateRectRgn(0, ClientHeight-FRadius, FRadius, ClientHeight-1);
			CombineRgn(rgn, rectRgn, rgn, RGN_OR);
		}
		if (!FRoundedCorner.Contains(eRC::RB)) { // RB
			HRGN rectRgn = CreateRectRgn(ClientWidth-FRadius, ClientHeight-FRadius, ClientWidth-1, ClientHeight-1);
			CombineRgn(rgn, rectRgn, rgn, RGN_OR);
		}

		SetWindowRgn(Handle, rgn, true);
	} */


	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetBorderType(eBorderType Value)
{
	FBorderType = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetRoundedCorner(TRoundedCorner Value)
{
	FRoundedCorner = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::PanelResize(TObject *Sender)
{
	SetRadius(FRadius);
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::DrawImageByIndex(int Index, int x, int y)
{
  if (FImages == NULL) return;
	if (FImages->Count > Index) {
    FImages->Draw(Canvas, x, y, Index);
	}
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::OnImageChange(TObject *Sender)
{
	CheckMinSize();
	//DrawImageByIndex(0, 0, 0);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetImages(TImageList* Value)
{
	if (FImages != NULL) {
		FImages->UnRegisterChanges(FImagesChangeLink);
	}
	FImages = Value;

	if (FImages != NULL) {
		FImages->RegisterChanges(FImagesChangeLink);
		FImages->FreeNotification(this);
    CheckMinSize();
	}
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetBorderWidth(int Value)
{
	FBorderWidth = Value;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetShadowWidth(int Value)
{
	FShadowWidth = Value;
  Invalidate();
}
//---------------------------------------------------------------------------
void TRoundedPanel::CheckMinSize()
{
	if (FImages->Width > Width)
		Width = FImages->Width;
	if (FImages->Height > Height)
    Height = FImages->Height;
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::ConstrainedResize(int &MinWidth, int &MinHeight, int &MaxWidth, int &MaxHeight)
{
	//SetRadius(FRadius);
}
//---------------------------------------------------------------------------
namespace Roundedpanel
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TRoundedPanel)};
		RegisterComponents(L"UI Set", classes, 0);
	}
}
//---------------------------------------------------------------------------
