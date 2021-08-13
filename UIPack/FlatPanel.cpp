//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "FlatPanel.h"
#include "DrawFunc.h"
#include <Vcl.Imaging.jpeg.hpp>

#include <typeinfo>
#include <string>
#include <Vcl.Buttons.hpp>
#pragma package(smart_init)
using namespace Gdiplus;
using namespace std;
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TFlatPanel *)
{
	new TFlatPanel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TFlatPanel::TFlatPanel(TComponent* Owner)
	: TCustomPanel(Owner)
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	this->OnResize = PanelResize;
	FBorderColor = clGray;
	FRoundedCorner << eRC::LT << eRC::RT << eRC::LB << eRC::RB;
	////FImagesChangeLink = new TChangeLink();
	////FImagesChangeLink->OnChange = OnImageChange;
	FLastRoundedCorner = -1;

	FImage = new TPngImage();

  FPicture = new TPicture();

	this->DoubleBuffered = true;
}
//---------------------------------------------------------------------------
__fastcall TFlatPanel::~TFlatPanel()
{
	////FImagesChangeLink->Free();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::Loaded()
{
	////if (FImage != NULL)
		////PngDrawer->AssignImage((TPngImage*)FImage);
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::DoRepaint(TObject* Obj)
{
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::OnPictureChange(TObject* Obj)
{
	////PngDrawer->AssignImage(FImage);
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetImage(TPngImage* Image)
{
	FImage->Assign(Image);
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetPicture(TPicture* Picture)
{
  FPicture->Assign(Picture);
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
void TFlatPanel::GetRectPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect)
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
void TFlatPanel::GetRoundRectPath(Gdiplus::GraphicsPath *Path, Gdiplus::Rect Rect, int Dia)
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
// TODO:
// Node about user image drawing:
// We draw user image with following modes
// 1: Texture mode: Draw image as the texture of the shape, it can fit the shape.
// 2: Central mode: Draw image on the central of the shape,
//    but will overlay shape when it is bigger then the shape
// 3: Stetch mode: Draw image from original point (0,0) and fillup whole face,
//    the image will be stretch when it is smaller than the shape.
//    This is not proper for rounded shape too.
//
// The other image arguments:
// Autosize: Let the object to resize to fit the selected image (need to consider the shadow also)
// Opacity: Opacity of the image (minor feature)


//---------------------------------------------------------------------------
// From rectangle to the shape path (maybe rounded or rectangle)
//---------------------------------------------------------------------------
void TFlatPanel::GenerateShapePath(Gdiplus::Rect Rect, int Radius, int BorderWidth, Gdiplus::GraphicsPath *Path)
{


	if (BorderWidth > 0) {
		int bodyShrink = BorderWidth / 2;
		Rect.X += bodyShrink;
		Rect.Y += bodyShrink;
		Rect.Width -= (bodyShrink * 2);
    Rect.Height -= (bodyShrink * 2);
	}
  int dia = 2 * Radius;

	// get the corner path
	GraphicsPath path;

	// get path
	if (dia > 0) {
		GetRoundRectPath(Path, Rect, dia);
	}
	else {
		GetRectPath(Path, Rect);
	}
}
//---------------------------------------------------------------------------
void TFlatPanel::GetShadowAndBodyRect(Gdiplus::Rect& Rect, Gdiplus::Rect& ShadowRect, Gdiplus::Rect& BodyRect)
{
	if (ShadowShiftX >= 0) {
		ShadowRect.X = Rect.X + ShadowShiftX;
		ShadowRect.Width = Rect.Width - ShadowShiftX;

		BodyRect.X = Rect.X;
		BodyRect.Width = ShadowRect.Width;
	}
	else {
		BodyRect.X = Rect.X - ShadowShiftX;
		BodyRect.Width = Rect.Width - ShadowShiftX;

		ShadowRect.X = Rect.X;
		ShadowRect.Width = BodyRect.Width;
  }

	if (ShadowShiftY >= 0) {
		ShadowRect.Y = Rect.Y + ShadowShiftY;
		ShadowRect.Height = Rect.Height - ShadowShiftY;

		BodyRect.Y = Rect.Y;
		BodyRect.Height = ShadowRect.Height;
	}
	else {
		BodyRect.Y = Rect.Y - ShadowShiftY;
		BodyRect.Height = Rect.Height - ShadowShiftY;

		ShadowRect.Y = Rect.Y;
		ShadowRect.Height = BodyRect.Height;
  }
}
//---------------------------------------------------------------------------
void TFlatPanel::DrawBorder(Gdiplus::Graphics& Graph,
													Gdiplus::GraphicsPath& Path,
													Gdiplus::Color BorderColor,
													int BorderWidth)
{
	// set to pixel mode
	int oldPageUnit = Graph.SetPageUnit(UnitPixel);

	// define the pen
	Gdiplus::Pen pen(BorderColor, BorderWidth);
	pen.SetAlignment(PenAlignmentCenter);

	Graph.DrawPath(&pen, &Path);

	// restore page unit
	Graph.SetPageUnit((Unit)oldPageUnit);
}
//---------------------------------------------------------------------------
void TFlatPanel::DrawBodyImage(Gdiplus::Graphics& Graph,
															 Gdiplus::Rect& Rect,
															 Gdiplus::GraphicsPath& Path,
															 Gdiplus::Bitmap& Bitmap)
{
	if (FImage == NULL)
    return;

	// set to pixel mode
	int oldPageUnit = Graph.SetPageUnit(UnitPixel);

	Graph.SetClip(&Path);

	if (FImage != NULL) {
		// Draw image at the center
		Gdiplus::Rect imgRect;

		if (Bitmap.GetWidth() < Rect.Width)
			imgRect.X = Rect.X + (Rect.Width - Bitmap.GetWidth()) / 2;
		else
			imgRect.X = Rect.X;

		if (Bitmap.GetHeight() < Rect.Height)
			imgRect.Y = Rect.Y + (Rect.Height - Bitmap.GetHeight()) / 2;
		else
			imgRect.Y = Rect.Y;

		imgRect.Width = Bitmap.GetWidth();
		imgRect.Height = Bitmap.GetHeight();
		Graph.DrawImage(&Bitmap, imgRect);
	}

	// restore page unit
	Graph.SetPageUnit((Unit)oldPageUnit);
}
//---------------------------------------------------------------------------
void TFlatPanel::DrawBodyFillColor(Gdiplus::Graphics& Graph,
													Gdiplus::Rect& Rect,
													Gdiplus::GraphicsPath& Path,
													Gdiplus::Color Color)
{
	// set to pixel mode
	int oldPageUnit = Graph.SetPageUnit(UnitPixel);

	Gdiplus::SolidBrush brush(Color);
	Graph.SetClip(&Path);
	Graph.FillRectangle((const Gdiplus::Brush*)&brush, Rect);

	// restore page unit
	Graph.SetPageUnit((Unit)oldPageUnit);
}
//---------------------------------------------------------------------------
void TFlatPanel::DrawShadow(Gdiplus::Graphics& Graph,
													Gdiplus::Rect& Rect,
													Gdiplus::GraphicsPath& Path,
													Gdiplus::Color ShadowColorStart,
													Gdiplus::Color ShadowColorEnd)
{
	// set to pixel mode
	int oldPageUnit = Graph.SetPageUnit(UnitPixel);

	// fill
	Gdiplus::PathGradientBrush brush(&Path);

	brush.SetCenterColor(ShadowColorStart);

	Gdiplus::PointF cent;
	brush.GetCenterPoint(&cent);

	// TODO: Allows to control the gradient center bias
	cent.X += (-Rect.X) + Rect.Width * (FShadowCenterX / 100.00);
	cent.Y += (-Rect.Y) + Rect.Height * (FShadowCenterY / 100.0);

	brush.SetCenterPoint(cent);

	Gdiplus::Color surroundClrs[] = {ShadowColorEnd};
	int surroundColorCnt = 1;

	brush.SetSurroundColors(surroundClrs, &surroundColorCnt);

	Graph.FillPath(&brush, &Path);

	// restore page unit
	Graph.SetPageUnit((Unit)oldPageUnit);
}
//---------------------------------------------------------------------------
void TFlatPanel::ClipParentDrawing(HDC DC)
{
	int saveIdx = SaveDC(DC);
	TPoint p;
	GetViewportOrgEx(DC, &p);
	SetViewportOrgEx(DC, p.x - this->Left, p.y - this->Top, NULL);
	IntersectClipRect(DC, 0, 0, Parent->ClientWidth, Parent->ClientHeight);
	Parent->Perform(WM_ERASEBKGND, (NativeUInt)DC, (NativeInt)0);
	Parent->Perform(WM_PRINTCLIENT, (NativeUInt)DC, (NativeInt) PRF_CLIENT);

	RestoreDC(DC, saveIdx);
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::Paint()
{
	if (!FShadowEnabled) {
		FShadowShiftX = 0;
		FShadowShiftY = 0;
  }
	Gdiplus::Rect bodyRect, shadowRect;
	Gdiplus::Rect orgRect(ClientRect.Left, ClientRect.Top, ClientRect.Width(), ClientRect.Height());

	if (FShadowEnabled)
		GetShadowAndBodyRect(orgRect, shadowRect, bodyRect);
	else
		bodyRect = orgRect;

	int trueRadius = FRadius;

	if (trueRadius > (bodyRect.Width / 2)) trueRadius = bodyRect.Width / 2;
	if (trueRadius > (bodyRect.Height / 2)) trueRadius = bodyRect.Height / 2;

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

	string imageType = typeid(FImage).name();

	if ((Parent != Owner)) {
    /* The double buffering implementation */
		doubleBuffering = true;
		memoryHDC = CreateCompatibleDC(Canvas->Handle);
		hMemoryBmp = CreateCompatibleBitmap(Canvas->Handle, Width, Height);
		hOldBmp = (HBITMAP)SelectObject(memoryHDC, hMemoryBmp);

		/* Following method has no effect, just written for notice */
		////HDC wnhdc = GetWindowDC(this->Parent->Handle);
		////BitBlt(memoryHDC, 0, 0, Width, Height, wnhdc, Left, Top, SRCCOPY);
		////DrawParentImage(this, memoryHDC, true);

		/* Generate graphics with the memory DC */
		graph = new Gdiplus::Graphics(memoryHDC);
		ClipParentDrawing(memoryHDC);
	}
	else {
		/* Parent is a form, can't use double buffering method */
		graph = new Gdiplus::Graphics(Canvas->Handle);
	}

	graph->SetSmoothingMode(SmoothingModeHighQuality);

	/* Generate the path of the shape with radius and border arguments */
	Gdiplus::GraphicsPath bodyShape;
	GenerateShapePath(bodyRect, trueRadius, BorderWidth, &bodyShape);

	if (FShadowEnabled) {
    /* Draw the shadow with the same shape that shifted from body */
		Gdiplus::GraphicsPath shadowPath;
		GenerateShapePath(shadowRect, trueRadius, BorderWidth, &shadowPath);
		DrawShadow(*graph, shadowRect, shadowPath, shadowColorStart, shadowColorEnd);
	}
	/* Fill color on the body shape */
	DrawBodyFillColor(*graph, bodyRect, bodyShape, bodyColor);

	if (FPicture->Graphic != NULL) {
		/* Draw image within the body shape */
		Gdiplus::Bitmap bitmap(FPicture->Graphic->Width, FPicture->Graphic->Height, PixelFormat32bppARGB);

		if (typeid(*FPicture->Graphic) == typeid(TPngImageEx) ||
				typeid(*FPicture->Graphic) == typeid(TPngImage)) {

			PngImageAssignToGdipBitmap(&bitmap, (TPngImage*)FPicture->Graphic);
			DrawBodyImage(*graph, bodyRect, bodyShape, bitmap);
			imageType = "";
		}
		else if (typeid(*FPicture->Graphic) == typeid(TJPEGImage)) {
			TJPEGImage* jpg = (TJPEGImage*)FPicture->Graphic;
      ////jpg->DIBNeeded();
			TBitmap* bmp = new TBitmap();
			bmp->Assign(jpg);

			if (!ComponentState.Contains(csDesigning)) {
        // Enable this for testing resize
				////BitmapSmoothResize(bmp, bodyRect.Width, bodyRect.Height);
			}

			VCLBitmapAssignToGdipBitmap(&bitmap, bmp);
      DrawBodyImage(*graph, bodyRect, bodyShape, bitmap);

			imageType = "";
		}
		else {
      imageType = "";
		}
	}
	else if (FImage != NULL) {
		Gdiplus::Bitmap bitmap(FImage->Width, FImage->Height, PixelFormat32bppARGB);
		PngImageAssignToGdipBitmap(&bitmap, FImage);
		DrawBodyImage(*graph, bodyRect, bodyShape, bitmap);
	}

	if (FBorderWidth > 0) {
    /* Draw border after body finished */
		DrawBorder(*graph, bodyShape, borderColor, FBorderWidth);
	}

  /* Delete dynamic elements */
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
void __fastcall TFlatPanel::SetColor(TColor Value)
{
	FColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetBorderColor(TColor Value)
{
	FBorderColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetShadowColorStart(TColor Value)
{
	FShadowColorStart = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetShadowColorEnd(TColor Value)
{
	FShadowColorEnd = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetTransparent(bool Value)
{
	FTransparent = Value;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetRadius(unsigned int Radius)
{
	FRadius = Radius;

	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetBorderType(eBorderType Value)
{
	FBorderType = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetRoundedCorner(TRoundedCorner Value)
{
	FRoundedCorner = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::PanelResize(TObject *Sender)
{
	SetRadius(FRadius);
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::DrawImageByIndex(int Index, int x, int y)
{
  if (FImages == NULL) return;
	if (FImages->Count > Index) {
    FImages->Draw(Canvas, x, y, Index);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::OnImageChange(TObject *Sender)
{
	CheckMinSize();
	//DrawImageByIndex(0, 0, 0);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetImages(TImageList* Value)
{
	/*
	if (FImages != NULL) {
		FImages->UnRegisterChanges(FImagesChangeLink);
	}
	FImages = Value;

	if (FImages != NULL) {
		FImages->RegisterChanges(FImagesChangeLink);
		FImages->FreeNotification(this);
		CheckMinSize();
	}
  Invalidate();*/
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetBorderWidth(int Value)
{
	FBorderWidth = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetShadowEnabled(bool Value)
{
	FShadowEnabled = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetShadowShiftX(int Value)
{
	if (Value > (Width / 2)) Value = Width / 2;

	FShadowShiftX = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetShadowShiftY(int Value)
{
	if (Value > (Height / 2)) Value = Height / 2;
	FShadowShiftY = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetShadowCenterX(float Value)
{
	if (Value > 100) Value = 100;
	FShadowCenterX = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::SetShadowCenterY(float Value)
{
	if (Value > 100) Value = 100;
	FShadowCenterY = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void TFlatPanel::CheckMinSize()
{
	if (FImages->Width > Width)
		Width = FImages->Width;
	if (FImages->Height > Height)
    Height = FImages->Height;
}
//---------------------------------------------------------------------------
void __fastcall TFlatPanel::ConstrainedResize(int &MinWidth, int &MinHeight, int &MaxWidth, int &MaxHeight)
{
	//SetRadius(FRadius);
}
//---------------------------------------------------------------------------
namespace Flatpanel
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TFlatPanel)};
		RegisterComponents(L"UI Set", classes, 0);
	}
}
//---------------------------------------------------------------------------
