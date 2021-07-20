//---------------------------------------------------------------------------
#include <vcl.h>
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
		////PngImageAssignToBitmap(FBitmap, (TPngImage*)Graphic, false);
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
