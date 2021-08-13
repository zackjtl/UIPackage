//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PngDrawer.h"
#include <string.h>
#include <Gdiplus.h>

using namespace std;
using namespace Gdiplus;
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
void TPngDrawer::AssignImage(TGraphic* Graphic)
{
	if (typeid(*Graphic) == typeid(TPngImageEx) ||
			typeid(*Graphic) == typeid(TPngImage)) {
		PngImageAssignToBitmap(FBitmap, (TPngImage*)Graphic, false);
		////ToBitmap(
	}
	else {
		FBitmap->Assign(Graphic);
	}
	DoRepaint();
}
//---------------------------------------------------------------------------
////void TPngDrawer::Draw(TCanvas* Canvas, )
//---------------------------------------------------------------------------
void TPngDrawer::DoRepaint()
{
	if (FOnDoRepaint != NULL) {
		FOnDoRepaint(this);
	}
}
//---------------------------------------------------------------------------
TChunk* FindChunkPLTE(TPngImage* Image)
{
	for (int i = 0; i < Image->Chunks->Count; ++i) {
		if (dynamic_cast<const TChunkPLTE*>(Image->Chunks->Item[i]) != NULL) {
			return Image->Chunks->Item[i];
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
TChunk* FindChunktRNS(TPngImage* Image)
{
	for (int i = 0; i < Image->Chunks->Count; ++i) {
		if (dynamic_cast<const TChunktRNS*>(Image->Chunks->Item[i]) != NULL) {
			return Image->Chunks->Item[i];
		}
	}
  return NULL;
}
#if 0
procedure TForm1.SmoothResize(abmp:TBitmap; NuWidth,NuHeight:integer);
var
xscale, yscale : Single;
sfrom_y, sfrom_x : Single;
ifrom_y, ifrom_x : Integer;
to_y, to_x : Integer;
weight_x, weight_y : array[0..1] of Single;
weight : Single;
new_red, new_green : Integer;
new_blue : Integer;
total_red, total_green : Single;
total_blue : Single;
ix, iy : Integer;
bTmp : TBitmap;
sli, slo : pRGBArray;
begin
abmp.PixelFormat := pf24bit;
bTmp := TBitmap.Create;
bTmp.PixelFormat := pf24bit;
bTmp.Width := NuWidth;
bTmp.Height := NuHeight;
xscale := bTmp.Width / (abmp.Width-1);
yscale := bTmp.Height / (abmp.Height-1);
for to_y := 0 to bTmp.Height-1 do begin
sfrom_y := to_y / yscale;
ifrom_y := Trunc(sfrom_y);
weight_y[1] := sfrom_y - ifrom_y;
weight_y[0] := 1 - weight_y[1];
for to_x := 0 to bTmp.Width-1 do begin
sfrom_x := to_x / xscale;
ifrom_x := Trunc(sfrom_x);
weight_x[1] := sfrom_x - ifrom_x;
weight_x[0] := 1 - weight_x[1];
total_red := 0.0;
total_green := 0.0;
total_blue := 0.0;
for ix := 0 to 1 do begin
for iy := 0 to 1 do begin
sli := abmp.Scanline[ifrom_y + iy];
new_red := sli[ifrom_x + ix].rgbtRed;
new_green := sli[ifrom_x + ix].rgbtGreen;
new_blue := sli[ifrom_x + ix].rgbtBlue;
weight := weight_x[ix] * weight_y[iy];
total_red := total_red + new_red * weight;
total_green := total_green + new_green * weight;
total_blue := total_blue + new_blue * weight;
end;
end;
slo := bTmp.ScanLine[to_y];
slo[to_x].rgbtRed := Round(total_red);
slo[to_x].rgbtGreen := Round(total_green);
slo[to_x].rgbtBlue := Round(total_blue);
end;
end;
abmp.Width := bTmp.Width;
abmp.Height := bTmp.Height;
abmp.Canvas.Draw(0,0,bTmp);
bTmp.Free;
end;
#endif
//---------------------------------------------------------------------------
double Rounding (double num,int index )
{
    bool b_IsNegative = false ;
    if (num < 0 )
    {
        b_IsNegative = true;
        num = -num ;
    }
    if (index >= 0)
    {
        int temp = pow((double)10,index);
        num = (int) (num * temp + 0.5 ) / (temp);
    }
    if (b_IsNegative)
    {
        num = -num ;
    }

    return (num);
}
//---------------------------------------------------------------------------
void BitmapSmoothResize(TBitmap* OrgImage, int NewWidth, int NewHeight)
{
	float xscale, yscale;
	float sfrom_y, sfrom_x;
	int ifrom_y, ifrom_x;

	float weight_x[2];
	float weight_y[2];

	float weight;
	int new_red, new_green;
	int new_blue, new_alpha;
	float total_red, total_green;
	float total_blue, total_alpha;
	TBitmap* newBitmap = new TBitmap();

	newBitmap->SetSize(NewWidth, NewHeight);
	newBitmap->PixelFormat = PixelFormat32bppRGB;

	xscale = (float)newBitmap->Width / (float)(OrgImage->Width-1);
	yscale = (float)newBitmap->Height / (float)(OrgImage->Height-1);

	for (int to_y = 0; to_y < newBitmap->Height; ++to_y) {
		sfrom_y = to_y / yscale;
		ifrom_y = (int)sfrom_y;
		weight_y[1] = sfrom_y - ifrom_y;
		weight_y[0] = 1 - weight_y[1];

		for (int to_x = 0; to_x < newBitmap->Width; ++to_x) {
			sfrom_x = (float)to_x / xscale;
			ifrom_x = (int)sfrom_x;
			weight_x[1] = sfrom_x - ifrom_x;
			weight_x[0] = 1 - weight_x[1];

			total_red = 0.0;
			total_green = 0.0;
			total_blue = 0.0;
      total_alpha = 0;

			for (int ix = 0; ix < 2; ++ix) {
				for (int iy = 0; iy < 2; ++iy) {

					weight = weight_x[ix] * weight_y[iy];

					if ((ifrom_y + iy) >= OrgImage->Height) {
						throw  NULL;
					}

					if (OrgImage->PixelFormat == PixelFormat32bppRGB) {
						RGBQUAD* pSrc = (RGBQUAD*)OrgImage->ScanLine[ifrom_y + iy];

						new_red = pSrc[ifrom_x + ix].rgbRed;
						new_green = pSrc[ifrom_x + ix].rgbGreen;
						new_blue = pSrc[ifrom_x + ix].rgbBlue;
						new_alpha = pSrc[ifrom_x + ix].rgbReserved;

						total_red += new_red * weight;
						total_green += new_green * weight;
						total_blue += new_blue * weight;
						total_alpha += new_alpha * weight;
					}
					else {
						RGBTRIPLE* pSrc = (RGBTRIPLE*)OrgImage->ScanLine[ifrom_y + iy];

						if ((ifrom_x + ix) >= OrgImage->Width) {
							throw NULL;
						}
						new_red = pSrc[ifrom_x + ix].rgbtRed;
						new_green = pSrc[ifrom_x + ix].rgbtGreen;
						new_blue = pSrc[ifrom_x + ix].rgbtBlue;

						total_red += new_red * weight;
						total_green += new_green * weight;
						total_blue += new_blue * weight;
					}
				}
			}

			int tmpWidth = newBitmap->Width;

			if (to_y >= newBitmap->Height) {
				throw NULL;
			}
			if ((to_x) >= newBitmap->Width) {
				throw NULL;
			}
			int newBmpWidth = newBitmap->Width;

			if (to_y == 0 && to_x == 878) {
				to_y = 0;
				RGBTRIPLE* pTemp = (RGBTRIPLE*)newBitmap->ScanLine[to_y];
        Rounding(total_red, 0);
        to_y = 0;
      }

			RGBTRIPLE* pDest = (RGBTRIPLE*)newBitmap->ScanLine[to_y];
			pDest[to_x].rgbtRed = Rounding(total_red, 0);
			pDest[to_x].rgbtGreen = Rounding(total_green, 0);
			pDest[to_x].rgbtBlue = Rounding(total_blue, 0);
//			pDest[to_x].rgbReserved = Rounding(total_alpha, 0);
    }
	}
#if 0
for to_y := 0 to bTmp.Height-1 do begin
sfrom_y := to_y / yscale;
ifrom_y := trunc(sfrom_y);
weight_y[1] := sfrom_y - ifrom_y;
weight_y[0] := 1 - weight_y[1];
for to_x := 0 to bTmp.Width-1 do begin
sfrom_x := to_x / xscale;
ifrom_x := Trunc(sfrom_x);
weight_x[1] := sfrom_x - ifrom_x;
weight_x[0] := 1 - weight_x[1];
total_red := 0.0;
total_green := 0.0;
total_blue := 0.0;
for ix := 0 to 1 do begin
for iy := 0 to 1 do begin
sli := abmp.Scanline[ifrom_y + iy];
new_red := sli[ifrom_x + ix].rgbtRed;
new_green := sli[ifrom_x + ix].rgbtGreen;
new_blue := sli[ifrom_x + ix].rgbtBlue;
weight := weight_x[ix] * weight_y[iy];
total_red := total_red + new_red * weight;
total_green := total_green + new_green * weight;
total_blue := total_blue + new_blue * weight;
end;
end;
slo := bTmp.ScanLine[to_y];
slo[to_x].rgbtRed := total_red;
slo[to_x].rgbtGreen := Round(total_green);
slo[to_x].rgbtBlue := Round(total_blue);
end;
end;
#endif
}
//---------------------------------------------------------------------------
bool VCLBitmapAssignToGdipBitmap(Gdiplus::Bitmap* Bitmap, TBitmap* Image)
{
	bool available = true;

	BitmapData* bdt = new BitmapData();
	Gdiplus::Rect rc(0, 0, Image->Width, Image->Height);

	Bitmap->LockBits(&rc, ImageLockModeWrite, PixelFormat24bppRGB, bdt);

	for (int y = 0; y < bdt->Height; ++y) {
		TRGBTriple* pDest = (TRGBTriple*)((BYTE*)bdt->Scan0 + y * bdt->Stride);

		if (Image->PixelFormat == PixelFormat32bppRGB) {
 			TRGBQuad* pSrc = (TRGBQuad*)Image->ScanLine[y];
			memcpy(pDest, pSrc, bdt->Stride);
		}
		else {
			TRGBTriple* pSrc = (TRGBTriple*)Image->ScanLine[y];
			memcpy(pDest, pSrc, bdt->Stride);
		}
	}
	Bitmap->UnlockBits(bdt);

	delete bdt;

  return available;
}
//---------------------------------------------------------------------------
bool PngImageAssignToGdipBitmap(Gdiplus::Bitmap* Bitmap, TPngImage* PngImage, bool Premultiplied)
{
	bool available = true;

	BitmapData* bdt = new BitmapData();
	Gdiplus::Rect rc(0, 0, PngImage->Width, PngImage->Height);

	Bitmap->LockBits(&rc, ImageLockModeWrite, PixelFormat32bppARGB, bdt);

	TChunkPLTE* chunkPLTE = (TChunkPLTE*)FindChunkPLTE(PngImage);
	TChunktRNS* chunktRNS = (TChunktRNS*)FindChunktRNS(PngImage);

	for (int y = 0; y < bdt->Height; ++y) {
		TRGBQuad* pBitmap = (TRGBQuad*)((BYTE*)bdt->Scan0 + y * bdt->Stride);
		TRGBTriple* pPng = (TRGBTriple*)PngImage->Scanline[y];
		BYTE* pPngTable = (BYTE*)PngImage->Scanline[y];
		BYTE* pPngAlpha = (BYTE*)PngImage->AlphaScanline[y];

    PngImage->CreateAlpha();

		if (PngImage->Header->ColorType == COLOR_RGBALPHA) {
			for (int x = 0; x < bdt->Width; ++x) {
				double multiplier = Premultiplied ? (pPngAlpha[x] / 255.0) : 1;

				pBitmap[x].rgbBlue = pPng[x].rgbtBlue * multiplier;
				pBitmap[x].rgbGreen = pPng[x].rgbtGreen * multiplier;
				pBitmap[x].rgbRed = pPng[x].rgbtRed * multiplier;
				pBitmap[x].rgbReserved = pPngAlpha[x];
			}
		}
		else if (PngImage->Header->ColorType == COLOR_PALETTE) {

			if ((chunkPLTE != NULL) && (chunktRNS != NULL)) {
				TRGBQuad plteColor;
				BYTE plteAlpha;
				double multiplier;

				for (int x = 0; x < bdt->Width; ++x) {
					plteColor = chunkPLTE->Item[pPngTable[x]];
					plteAlpha = chunktRNS->PaletteValues[pPngTable[x]];

					multiplier = Premultiplied ? (plteAlpha / 255.0) : 1;

					pBitmap[x].rgbBlue = plteColor.rgbBlue * multiplier;
					pBitmap[x].rgbGreen = plteColor.rgbGreen * multiplier;
					pBitmap[x].rgbRed = plteColor.rgbRed * multiplier;
					pBitmap[x].rgbReserved = plteAlpha;
				}
			}
			else {
				available = false;
				break;
			}
		}
		else {  // GRAYSCALE
			for (int x = 0; x < bdt->Width; ++x) {
				double multiplier = Premultiplied ? (pPngAlpha[x] / 255.0) : 1;

				pBitmap[x].rgbBlue = pPng[x].rgbtBlue * multiplier;
				pBitmap[x].rgbGreen = pPng[x].rgbtBlue;
				pBitmap[x].rgbRed = pPng[x].rgbtBlue;
				pBitmap[x].rgbReserved = pPngAlpha[x];
			}
		}
	}
	Bitmap->UnlockBits(bdt);

	delete bdt;

  return available;
}
//---------------------------------------------------------------------------
void PngImageAssignToBitmap(TBitmap* Bitmap, TPngImage* PngImage, bool Premultiplied)
{
	if ((PngImage->TransparencyMode != ptmPartial) || (PngImage->Header->BitDepth != 8)) {
		Bitmap->Assign(PngImage);
		return;
	}
	Bitmap->SetSize(0, 0);

	if (Premultiplied) {
		Bitmap->AlphaFormat = afPremultiplied;
	}
	else {
		Bitmap->AlphaFormat = afDefined;
	}
	Bitmap->PixelFormat = pf32bit;
	Bitmap->SetSize(PngImage->Width, PngImage->Height);

	for (int y = 0; y < Bitmap->Height; ++y) {
		TRGBQuad* pBitmap = (TRGBQuad*)Bitmap->ScanLine[y];
		TRGBTriple* pPng = (TRGBTriple*)PngImage->Scanline[y];
		BYTE* pPngTable = (BYTE*)PngImage->Scanline[y];
		BYTE* pPngAlpha = (BYTE*)PngImage->AlphaScanline[y];

		if (PngImage->Header->ColorType == COLOR_RGBALPHA) {
			for (int x = 0; x < Bitmap->Width; ++x) {
				double multiplier = Premultiplied ? (pPngAlpha[x] / 255.0) : 1;

				pBitmap[x].rgbBlue = pPng[x].rgbtBlue * multiplier;
				pBitmap[x].rgbGreen = pPng[x].rgbtGreen * multiplier;
				pBitmap[x].rgbRed = pPng[x].rgbtRed * multiplier;
				pBitmap[x].rgbReserved = pPngAlpha[x];
			}
		}
		else if (PngImage->Header->ColorType == COLOR_PALETTE) {
			TChunkPLTE* meta = new TChunkPLTE(PngImage);
			TChunktRNS* meta2 = new TChunktRNS(PngImage);
			TRGBQuad plteColor;
			BYTE plteAlpha;

			for (int x = 0; x < Bitmap->Width; ++x) {
				plteColor = ((TChunkPLTE*)PngImage->Chunks->ItemFromClass((TChunkClass)meta))->Item[pPngTable[x]];
				plteAlpha = ((TChunktRNS*)PngImage->Chunks->ItemFromClass((TChunkClass)meta2))->PaletteValues[pPngTable[x]];

				double multiplier = Premultiplied ? (plteAlpha / 255.0) : 1;

				pBitmap[x].rgbBlue = plteColor.rgbBlue * multiplier;
				pBitmap[x].rgbGreen = plteColor.rgbGreen * multiplier;
				pBitmap[x].rgbRed = plteColor.rgbRed * multiplier;
				pBitmap[x].rgbReserved = plteAlpha;
			}
      delete meta, meta2;
		}
		else {  // GRAYSCALE
			for (int x = 0; x < Bitmap->Width; ++x) {
				double multiplier = Premultiplied ? (pPngAlpha[x] / 255.0) : 1;

				pBitmap[x].rgbBlue = pPng[x].rgbtBlue * multiplier;
				pBitmap[x].rgbGreen = pPng[x].rgbtBlue;
				pBitmap[x].rgbRed = pPng[x].rgbtBlue;
				pBitmap[x].rgbReserved = pPngAlpha[x];
			}
    }
	}
}
//---------------------------------------------------------------------------
