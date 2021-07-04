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

  this->DoubleBuffered = true;
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
void TRoundedPanel::GetRectPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect)
{
	// begin path
	Path->Reset();

	Gdiplus::Point l_t(Rect.X, Rect.Y);
	Gdiplus::Point r_t(Rect.X + Rect.Width - 1, Rect.Y);
	Gdiplus::Point r_b(r_t.X, Rect.Y + Rect.Height - 1);
	Gdiplus::Point l_b(l_t.X, r_b.Y);

	Path->AddLine(l_t, r_t);
	Path->AddLine(r_t, r_b);
	Path->AddLine(r_b, l_b);
	Path->AddLine(l_b, l_t);

	Path->CloseFigure();
}
//---------------------------------------------------------------------------
void TRoundedPanel::GetRoundRectPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Dia)
{
	// diameter can't exceed width or height
	// define a corner
	Gdiplus::Rect corner(Rect.X, Rect.Y, Dia, Dia);

	// begin path
	Path->Reset();

	Gdiplus::Point l_t(Rect.X, Rect.Y);
	Gdiplus::Point r_t(Rect.X + Rect.Width - 1, Rect.Y);
	Gdiplus::Point r_b(r_t.X, Rect.Y + Rect.Height - 1);
	Gdiplus::Point l_b(l_t.X, r_b.Y);

	int lineLen = Rect.Width > Rect.Height ? Rect.Height / 4 : Rect.Width / 4;

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
		Path->AddLine(PointFrom(r_t, -lineLen, 0), r_t);
		Path->AddLine(r_t, PointFrom(r_t, 0, lineLen));
	}

	corner.Y += (Rect.Height - Dia - 1);
	// bottom right
	if (FRoundedCorner.Contains(eRC::RB)) {
		Path->AddArc(corner,   0, 90);
	}
	else {
		Path->AddLine(PointFrom(r_b, 0, -lineLen), r_b);
		Path->AddLine(r_b, PointFrom(r_b, -lineLen, 0));
	}

	corner.X -= (Rect.Width - Dia - 1);
	// bottom left
	if (FRoundedCorner.Contains(eRC::LB)) {
		Path->AddArc(corner,  90, 90);
	}
	else {
		Path->AddLine(PointFrom(l_b, -lineLen, 0), l_b);
		Path->AddLine(l_b, PointFrom(l_b, 0, -lineLen));
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
	if (dia > 0) {
		GetRoundRectPath(&path, Rect, dia);
	}
	else {
    GetRectPath(&path, Rect);
  }

	// fill
	if (Gradient) {
		Gdiplus::PathGradientBrush brush(&path);
		brush.SetCenterColor(BodyColor);

		Gdiplus::PointF cent;
		brush.GetCenterPoint(&cent);

		// TODO: Allows to control the gradient center bias
		cent.X += (-Rect.X) + Rect.Width * (FShadowCenterX / 100.00);
		cent.Y += (-Rect.Y) + Rect.Height * (FShadowCenterY / 100.0);

		brush.SetCenterPoint(cent);

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
	if (dia != 0) {
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
	}
	// restore page unit
	graph.SetPageUnit((Unit)oldPageUnit);
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::Paint()
{
	if (!FShadowEnabled) {
		FShadowWidthX = 0;
		FShadowWidthY = 0;
  }

	int bodyX = 0;
	int bodyY = 0;
	int bodyWidth = ClientWidth-FShadowWidthX;
	int bodyHeight = ClientHeight-FShadowWidthY;

	int trueRadius = FRadius;

	if (trueRadius > (bodyWidth / 2)) trueRadius = bodyWidth / 2;
	if (trueRadius > (bodyHeight / 2)) trueRadius = bodyHeight / 2;

	int bias = trueRadius == 0 ? 1 : 0;

	bool doubleBuffering = false;

	Gdiplus::Color bodyColor = ColorFromTColor(Color);
	Gdiplus::Color borderColor = ColorFromTColor(BorderColor);
	Gdiplus::Color shadowColorStart = ColorFromTColor(FShadowColorStart);
	Gdiplus::Color shadowColorEnd = ColorFromTColor(FShadowColorEnd);

	shadowColorEnd.SetValue(shadowColorEnd.GetValue() & 0x00FFFFFF);

	Gdiplus::Graphics* graph = NULL;

	HDC memoryHDC;
	HBITMAP hMemoryBmp;
	HBITMAP hOldBmp;
#if 1
	if ((Parent != Owner)) {
	//if (false) {
    doubleBuffering = true;
		memoryHDC = CreateCompatibleDC(Canvas->Handle);
		hMemoryBmp = CreateCompatibleBitmap(Canvas->Handle, Width, Height);
		hOldBmp = (HBITMAP)SelectObject(memoryHDC, hMemoryBmp);

		// To copy the parent as background
		////HDC wnhdc = GetWindowDC(this->Parent->Handle);
		////BitBlt(memoryHDC, 0, 0, Width, Height, wnhdc, Left, Top, SRCCOPY);

		////DrawParentImage(this, memoryHDC, true);

		// Under trying
		graph = new Gdiplus::Graphics(memoryHDC);

		int saveIdx = SaveDC(memoryHDC);
		TPoint p;
		GetViewportOrgEx(memoryHDC, &p);
		SetViewportOrgEx(memoryHDC, p.x - this->Left, p.y - this->Top, NULL);
		IntersectClipRect(memoryHDC, 0, 0, Parent->ClientWidth, Parent->ClientHeight);
		Parent->Perform(WM_ERASEBKGND, (NativeUInt)memoryHDC, (NativeInt)0);
		Parent->Perform(WM_PRINTCLIENT, (NativeUInt)memoryHDC, (NativeInt) PRF_CLIENT);

		RestoreDC(memoryHDC, saveIdx);
	}
	else {
		graph = new Gdiplus::Graphics(Canvas->Handle);
	}
#endif
	if (!FTransparent) {
		if (FShadowEnabled) {
			FillRoundRect(*graph, Gdiplus::Rect(FShadowWidthX, FShadowWidthY, bodyWidth, bodyHeight), shadowColorStart, shadowColorEnd, btNone, 0, trueRadius, true);
		}
		FillRoundRect(*graph, Gdiplus::Rect(bodyX, bodyY, bodyWidth, bodyHeight), bodyColor, borderColor, FBorderType, BorderWidth, trueRadius, false);
	}
	////delete graph;
	if (doubleBuffering) {
		::BitBlt(Canvas->Handle, 0, 0, ClientWidth, ClientHeight, memoryHDC, 0, 0, SRCCOPY);

		SelectObject(memoryHDC, hOldBmp);
		DeleteObject(hMemoryBmp);
		DeleteDC(memoryHDC);
	}

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
void __fastcall TRoundedPanel::SetShadowEnabled(bool Value)
{
	FShadowEnabled = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetShadowWidthX(int Value)
{
	if (Value > (Width / 2)) Value = Width / 2;

	FShadowWidthX = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetShadowWidthY(int Value)
{
	if (Value > (Height / 2)) Value = Height / 2;
	FShadowWidthY = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetShadowCenterX(float Value)
{
	if (Value > 100) Value = 100;
	FShadowCenterX = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRoundedPanel::SetShadowCenterY(float Value)
{
	if (Value > 100) Value = 100;
	FShadowCenterY = Value;
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
