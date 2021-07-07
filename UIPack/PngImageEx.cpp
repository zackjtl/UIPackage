//---------------------------------------------------------------------------
#pragma hdrstop
#include "PngImageEx.h"
//---------------------------------------------------------------------------
void __fastcall TPngImageEx::SetWidth(int Value)
{
	TPngImage::SetWidth(Value);
    Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TPngImageEx::SetHeight(int Value)
{
	TPngImage::SetHeight(Value);
	Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TPngImageEx::LoadFromFile(const System::UnicodeString Filename)
{
	TPngImage::LoadFromFile(Filename);
	Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TPngImageEx::LoadFromStream(System::Classes::TStream* Stream)
{
	TPngImage::LoadFromStream(Stream);
	Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TPngImageEx::LoadFromClipboardFormat(System::Word AFormat, NativeUInt AData, HPALETTE APalette)
{
	TPngImage::LoadFromClipboardFormat(AFormat, AData, APalette);
	Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TPngImageEx::Assign(TPersistent* Source)
{
	Assign(Source);
	Changed(this);
}
//---------------------------------------------------------------------------
