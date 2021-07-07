//---------------------------------------------------------------------------
#ifndef PngImageExH
#define PngImageExH
//---------------------------------------------------------------------------
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TPngImageEx : public TPngImage
{
protected:
	void __fastcall SetWidth(int Value);
	void __fastcall SetHeight(int Value);

public:
	void __fastcall LoadFromFile(const System::UnicodeString Filename);
	void __fastcall LoadFromStream(System::Classes::TStream* Stream);
	void __fastcall LoadFromClipboardFormat(System::Word AFormat, NativeUInt AData, HPALETTE APalette);
	void __fastcall Assign(TPersistent* Source);
};
//---------------------------------------------------------------------------
#endif
