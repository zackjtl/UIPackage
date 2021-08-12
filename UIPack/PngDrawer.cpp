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
//---------------------------------------------------------------------------
bool VCLBitmapAssignToGdipBitmap(Gdiplus::Bitmap* Bitmap, TBitmap* Image)
{
	bool available = true;

	BitmapData* bdt = new BitmapData();
	Gdiplus::Rect rc(0, 0, Image->Width, Image->Height);

	Bitmap->LockBits(&rc, ImageLockModeWrite, PixelFormat24bppRGB, bdt);

	for (int y = 0; y < bdt->Height; ++y) {
		TRGBQuad* pBitmap = (TRGBQuad*)((BYTE*)bdt->Scan0 + y * bdt->Stride);
		TRGBTriple* pSrc = (TRGBTriple*)Image->ScanLine[y];
		
		for (int x = 0; x < bdt->Width; ++x) {

			pBitmap[x].rgbBlue = pSrc[x].rgbtBlue;
			pBitmap[x].rgbGreen = pSrc[x].rgbtGreen;
			pBitmap[x].rgbRed = pSrc[x].rgbtRed;
			pBitmap[x].rgbReserved = 0xFF;
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
