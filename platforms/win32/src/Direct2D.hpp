/*
	GUIDO Library
	Copyright (C) 2019 Michael Chourdakis, Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/


#pragma warning(disable:4100)


#include "GUIDOParse.h"
#include "GDeviceWin32.h"
#include "GSystemWin32.h"
#include "VGFont.h"

#include <any>


#ifndef TURBO_PLAY
/*
bool DebugOutput = 0;
template<typename ... many>
void safe_printf2(const wchar_t* s, many ... args)
{
	using namespace std;
	vector<any> a = { &args ... };
	wstringstream d;

	while (*s) {
		if (*s == '%') {
			if (*(s + 1) == '%') {
				++s;
			}
			else {

				if (a.empty())
					throw std::logic_error("Fewer arguments provided to printf");

				if (a[0].type() == typeid(wstring*)) d << *any_cast<wstring*>(a[0]);
				if (a[0].type() == typeid(int*)) d << *any_cast<int*>(a[0]);
				if (a[0].type() == typeid(unsigned long long*)) d << *any_cast<unsigned long long*>(a[0]);
				if (a[0].type() == typeid(double*)) d << *any_cast<double*>(a[0]);
				if (a[0].type() == typeid(float*)) d << *any_cast<float*>(a[0]);

				a.erase(a.begin());
				s++;
			}
		}
		d << *s++;
	}
	if (DebugOutput)
		wprintf(d.str().c_str());
	else
		OutputDebugString(d.str().c_str());
}
*/
#endif
class yystring : public std::wstring
{
private:
	mutable std::string asc_str_st;
public:

	// Constructors
	yystring::yystring() : std::wstring()
	{
	}
	yystring(const char* v, int CP = CP_UTF8)
	{
		EqChar(v, CP);
	}
	yystring(const std::string& v, int CP = CP_UTF8)
	{
		EqChar(v.c_str(), CP);
	}
	yystring(const std::wstring& v)
	{
		assign(v.c_str());
	}
	yystring(const wchar_t* f)
	{
		if (!f)
			return;
		assign(f);
	}
	yystring& Format(const wchar_t* f, ...)
	{
		va_list args;
		va_start(args, f);

		int len = _vscwprintf(f, args) + 100;
		if (len < 8192)
			len = 8192;
		std::vector<wchar_t> b(len);
		vswprintf_s(b.data(), len, f, args);
		assign(b.data());
		va_end(args);
		return *this;
	}

	// operator =
	void operator=(const char* v)
	{
		EqChar(v);
	}
	void operator=(const wchar_t* v)
	{
		assign(v);
	}
	void operator=(const std::wstring& v)
	{
		assign(v.c_str());
	}
	void operator=(const yystring& v)
	{
		assign(v.c_str());
	}
	void operator=(const std::string& v)
	{
		EqChar(v.c_str());
	}
	operator const wchar_t* ()
	{
		return c_str();
	}
	const std::string& asc_str(int CP = CP_UTF8) const
	{
		const wchar_t* s = c_str();
		int sz = WideCharToMultiByte(CP, 0, s, -1, 0, 0, 0, 0);
		std::vector<char> d(sz + 100);
		WideCharToMultiByte(CP, 0, s, -1, d.data(), sz + 100, 0, 0);
		asc_str_st = d.data();
		return asc_str_st;
	}
	operator const char* () const
	{
		return a_str();
	}
	const char* a_str(int CP = CP_UTF8) const
	{
		asc_str(CP);
		return asc_str_st.c_str();
	}
	// Internal Convertor
	void EqChar(const char* v, int CP = CP_UTF8)
	{
		clear();
		if (!v)
			return;
		int sz = MultiByteToWideChar(CP, 0, v, -1, 0, 0);
		std::vector<wchar_t> d(sz + 100);
		MultiByteToWideChar(CP, 0, v, -1, d.data(), sz + 100);
		assign(d.data());
	}
};


inline D2D1_COLOR_F VGColorX(const VGColor& inColor)
{
	D2D1_COLOR_F c;
	c.r = inColor.mRed / 255.0f; c.g = inColor.mGreen / 255.0f, c.b = inColor.mBlue / 255.0f, c.a = inColor.mAlpha / 255.0f;
	return c;
}

inline VGColor VGColorX(const D2D1_COLOR_F& f)
{
	return VGColor((unsigned char)(f.r * 255.0f), (unsigned char)(f.g * 255.0f), (unsigned char)(f.b * 255.0f), (unsigned char)(f.a * 255.0f));
}


class Direct2DSystem;

inline std::tuple<float, float> MeasureString(IDWriteFactory* pWriteFactory, IDWriteTextFormat* ffo, const wchar_t* txt, int l = -1)
{
	CComPtr<IDWriteTextLayout> lay = 0;
	pWriteFactory->CreateTextLayout(txt, l == -1 ? (UINT32)wcslen(txt) : l, ffo, 1000, 1000, &lay);
	if (!lay)
		return std::make_tuple<float, float>(0, 0);
	DWRITE_TEXT_METRICS m = { 0 };
	lay->GetMetrics(&m);
	float wi = (float)m.widthIncludingTrailingWhitespace;
	if (m.widthIncludingTrailingWhitespace > (float)wi)
		wi++;
	float he = (float)m.height;
	if (m.height > (float)he)
		he++;
	return std::make_tuple<float, float>(std::forward<float>(wi), std::forward<float>(he));
}

CComPtr<ID2D1SolidColorBrush> GetD2SolidBrush(ID2D1RenderTarget* p, D2D1_COLOR_F cc)
{
	CComPtr<ID2D1SolidColorBrush> b = 0;
	p->CreateSolidColorBrush(cc, &b);
	return b;
}

inline CComPtr<IDWriteFontCollection> PrivateGuidoFonts;

class Direct2DFont : public VGFont
{

public:
	float PrevSize = 0;
	std::string PrevName;
	CComPtr<IDWriteTextFormat> Text;
	Direct2DSystem* sys = 0;
	int BaseLine = 0;
	DWRITE_TEXT_ALIGNMENT a1 = DWRITE_TEXT_ALIGNMENT_LEADING;
	DWRITE_PARAGRAPH_ALIGNMENT a2 = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

	std::string		fName;
	int				fSize;
	int				fProperties;

	Direct2DFont(Direct2DSystem* ds, std::string name = "", int size = 0, int prop = 0);
	virtual					~Direct2DFont() {}
	void From(const VGFont* font,float Sc = 1.0f);

	// - VGFont services ---------------------------------------------
	virtual const char* GetName() const { return fName.c_str(); }
	virtual int 			GetSize() const { return fSize; }
	virtual int				GetProperties() const { return fProperties; }

	// - Symbol services ---------------------------------------------
	virtual void			GetExtent(const char* s, int inCharCount, float* outWidth, float* outHeight, VGDevice* context = 0) const;
	virtual void			GetExtent(unsigned char c, float* outWidth, float* outHeight, VGDevice* context = 0) const;
	virtual void			Direct2DFont::GetExtent(int c,
		float* outWidth,
		float* outHeight,
		VGDevice* context) const;

};



class Direct2DSystem : public VGSystem
{
public:
	CComPtr<ID2D1RenderTarget> rt;
	CComPtr<IDWriteFactory> WriteFactory;
	CComPtr<ID2D1Factory> Factory;
	Direct2DSystem(ID2D1RenderTarget* trg, IDWriteFactory* wf, ID2D1Factory* f);
	virtual					~Direct2DSystem();

	virtual VGDevice* CreateDisplayDevice();
	virtual VGDevice* CreateMemoryDevice(int inWidth, int inHeight);	
	virtual VGDevice* CreateMemoryDevice(const char* inPath);
	virtual VGDevice* CreatePrinterDevice();
	virtual VGDevice* CreateAntiAliasedMemoryDevice(int inWidth, int inHeight);
	virtual const VGFont* CreateVGFont(const char* faceName, int size, int properties) const;
};



inline void			Direct2DFont::GetExtent(int c,
	float* outWidth,
	float* outHeight,
	VGDevice* context) const
{
	char a1 = (char)c;
	return GetExtent(&a1, 1, outWidth, outHeight, context);
}
inline void		Direct2DFont::GetExtent(const char* s, int inCharCount, float* outWidth, float* outHeight, VGDevice* context) const
{
	yystring y = s;
	auto strs = MeasureString(sys->WriteFactory, Text, y.c_str(), inCharCount);

	*outWidth =  std::get<0>(strs);
	*outHeight = std::get<1>(strs);
}

inline void	Direct2DFont::GetExtent(unsigned char c, float* outWidth, float* outHeight, VGDevice* context) const
{
	yystring y;
	y.Format(L"%c", c);
	auto strs = MeasureString(sys->WriteFactory, Text, y.c_str(), -1);
	*outWidth = std::get<0>(strs);
	*outHeight = std::get<1>(strs);
}


class Direct2DDevice : public VGDevice
{
public:
	Direct2DSystem* mSys;
	Direct2DFont	mCurrTextFont;
	Direct2DFont	mCurrMusicFont;


	CComPtr<ID2D1SolidColorBrush> NextColorFont,NextColorPen,NextColorFill;
	D2D1_COLOR_F NextColorFontC = { 0.0f,0.0f,0.0f,1.0f };
	D2D1_COLOR_F NextColorPenC = { 0.0f,0.0f,0.0f,1.0f };
	D2D1_COLOR_F NextColorFillC = { 0.0f,0.0f,0.0f,1.0f };


	int Width = 0;
	int Height = 0;
	std::stack<float> PenWidthPushing;
	std::stack<D2D1_COLOR_F> PenColorPushing;
	std::stack<std::tuple<D2D1_COLOR_F, float>> PenPushing;
	Direct2DDevice(Direct2DSystem* s = 0, int wi = 0, int he = 0) : mCurrTextFont(s),mCurrMusicFont(s)
	{
		Width = wi;
		Height = he;
		mSys = s;
	}

	void EnsureBrush()
	{
		if (!NextColorFont)
		{
			D2D1_COLOR_F black = NextColorFontC;
			NextColorFont = GetD2SolidBrush(mSys->rt, black);
		}
		if (!NextColorPen)
		{
			D2D1_COLOR_F black = NextColorPenC;
			NextColorPen = GetD2SolidBrush(mSys->rt, black);
		}
		if (!NextColorFill)
		{
			D2D1_COLOR_F black = NextColorFillC;
			NextColorFill = GetD2SolidBrush(mSys->rt, black);
		}

	}



	inline void PPolygon(ID2D1Factory* f, ID2D1RenderTarget* r, D2D1_POINT_2F* p, size_t num, ID2D1SolidColorBrush* b, float szx,bool Fill,bool Close)
	{
		if (!p || !num)
			return;

		CComPtr<ID2D1PathGeometry> pg = 0;
		CComPtr<ID2D1GeometrySink> pgs = 0;
		f->CreatePathGeometry(&pg);
		if (pg)
		{
			pg->Open(&pgs);
			if (pgs)
			{
				D2D1_POINT_2F fb;
				fb.x = (FLOAT)p[0].x;
				fb.y = (FLOAT)p[0].y;
				// Use D2D1_FIGURE_BEGIN_FILLED for filled
				D2D1_FIGURE_BEGIN fg = D2D1_FIGURE_BEGIN_HOLLOW;
				if (Fill)
					fg = D2D1_FIGURE_BEGIN_FILLED;
				D2D1_FIGURE_END fe;
				if (Close)
					fe = D2D1_FIGURE_END_CLOSED;
				else
					fe = D2D1_FIGURE_END_OPEN;
				pgs->BeginFigure(fb, fg);
				for (size_t i = 1; i < num; i++)
				{
					D2D1_POINT_2F& a = p[i];
					if (&a == &p[0])
						continue;
					D2D1_POINT_2F fu;
					fu.x = a.x;
					fu.y = a.y;
					pgs->AddLine(fu);
				}
				pgs->EndFigure(fe);
				pgs->Close();
			}
			if (Fill)
				r->FillGeometry(pg, b);
			else
				r->DrawGeometry(pg, b, szx);
		}
	}

	virtual					~Direct2DDevice() {}

	virtual bool			IsValid() const { return true; }

	// - Drawing services ------------------------------------------------		
	virtual bool			BeginDraw() { 
#ifndef TURBO_PLAY
		mSys->rt->BeginDraw();
#endif
		D2D1_COLOR_F white = { 1.0f, 1.0f, 1.0f, 1.0f };
		mSys->rt->Clear(white);
		return true;
	}
	virtual void			EndDraw() {
#ifndef TURBO_PLAY
	mSys->rt->EndDraw();
#endif
	}
	virtual void			InvalidateRect(float left, float top, float right, float bottom) {}

	// - Standard graphic primitives -------------------------
	D2D1_POINT_2F ForMoveTo = {};
	virtual void			MoveTo(float x, float y) 
	{
		ForMoveTo.x = x;
		ForMoveTo.y = y;
	}
	virtual void			LineTo(float x, float y) 
	{
		Line(ForMoveTo.x, ForMoveTo.y, x, y);
		ForMoveTo.x = x;
		ForMoveTo.y = y;
	}

	virtual void			Line(float x1, float y1, float x2, float y2) 
	{
		EnsureBrush();
		D2D1_POINT_2F p1, p2;
		p1.x = x1;
		p1.y = y1;
		p2.x = x2;
		p2.y = y2;
		mSys->rt->DrawLine(p1, p2, NextColorPen,PenW);
	}
	virtual void Frame(float left, float top, float right, float bottom) 
	{
		D2D1_RECT_F r = { 0 };
		r.left = left;
		r.top = top;
		r.right = right;
		r.bottom = bottom;
		EnsureBrush();
		mSys->rt->DrawRectangle(r, NextColorPen);
	}

	virtual void			Arc(float left, float top, float right, float bottom, float startX, float startY, float endX, float endY) 
	{
	}

	// - Filled surfaces --------------------------------------
	virtual	void			Triangle(float x1, float y1, float x2, float y2, float x3, float y3) 
	{
		float x[3];
		float y[3];
		x[0] = x1; x[1] = x2; x[2] = x3;
		y[0] = y1; y[1] = y2; y[2] = y3;
		Polygon(x, y, 3);
	}
	virtual	void			Polygon(const float* xCoords, const float* yCoords, int count)
	{
		EnsureBrush();
		std::vector<D2D1_POINT_2F> pts(count);
		for (int i = 0; i < count; i++)
		{
			pts[i].x = xCoords[i];
			pts[i].y = yCoords[i];
		}

		PPolygon(mSys->Factory, mSys->rt, pts.data(), pts.size(), NextColorFill, PenW, true,false);
	}
	virtual void			Rectangle(float left, float top, float right, float bottom)
	{
		D2D1_RECT_F r = { 0 };
		r.left = left;
		r.top = top;
		r.right = right;
		r.bottom = bottom;
		EnsureBrush();
		mSys->rt->FillRectangle(r, NextColorFill);
	}

	// - Font services ---------------------------------------------------
	virtual	void			SetMusicFont(const VGFont* font) 
	{
		mCurrMusicFont.From(font);
	}
	virtual	const VGFont* GetMusicFont() const { return &mCurrMusicFont; }
	virtual	void			SetTextFont(const VGFont* font) 
	{
		mCurrTextFont.From(font);
	}
	virtual	const VGFont* GetTextFont() const { return &mCurrTextFont; }

	// - Pen & brush services --------------------------------------------
	virtual	void			SelectPen(const VGColor& inColor, float width) 
	{
		NextColorPenC = VGColorX(inColor);
		NextColorPen = 0;
		PenW = width;
	}
	virtual	void			SelectFillColor(const VGColor& c) 
	{
		NextColorFillC = VGColorX(c);
		NextColorFill = 0;
	}
	virtual	void			PushPen(const VGColor& inColor, float inWidth) 
	{
		PenPushing.push(std::make_tuple<D2D1_COLOR_F, float>(std::forward<D2D1_COLOR_F>(NextColorPenC),std::forward<float>(PenW)));
		NextColorPenC = VGColorX(inColor);
		NextColorPen = 0;
		PenW = inWidth;
	}
	virtual	void			PopPen() 
	{
		auto t = PenPushing.top();
		NextColorPenC = std::get<0>(t);
		NextColorPen = 0;
		PenW = std::get<1>(t);
		PenPushing.pop();
	}
	virtual	void			PushFillColor(const VGColor& inColor) 
	{
	}
	virtual	void			PopFillColor() 
	{
	}
	virtual	void			SetRasterOpMode(VRasterOpMode ROpMode) 
	{
	}
	virtual	VRasterOpMode	GetRasterOpMode() const { return kOpCopy; }

	// - Bitmap services (bit-block copy methods) --------------------------
	virtual bool			CopyPixels(VGDevice* pSrcDC, float alpha = -1.0) { return true; }
	virtual bool			CopyPixels(int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0) { return true; }
	virtual bool			CopyPixels(int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha = -1.0) { return true; }
	virtual bool			CopyPixels(int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0) { return true; }

	// - Coordinate services ------------------------------------------------
	D2D1_POINT_2F Origin = { 0,0 };
	D2D1_POINT_2F Scl = { 1.0f,1.0f };

	void Trs()
	{
		mSys->rt->SetTransform(D2D1::Matrix3x2F::Identity());
		D2D1::Matrix3x2F scl = D2D1::Matrix3x2F::Scale(Scl.x,Scl.y);
		D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(Origin.x*Scl.x,Origin.y*Scl.y);
		mSys->rt->SetTransform(scl*trans);
	}

	virtual	void			SetScale(float x, float y) 
	{ 
		Scl.x = x;
		Scl.y = y;
		Trs();
	}
	virtual	void			SetOrigin(float x, float y) 
	{
		Origin.x = x;
		Origin.y = y;
		Trs();
	}
	virtual	void OffsetOrigin(float x, float y) 
	{
		Origin.x += x;
		Origin.y += y;
		Trs();
	}

	virtual	void			LogicalToDevice(float* x, float* y) const {}
	virtual	void			DeviceToLogical(float* x, float* y) const {}
	virtual float			GetXScale() const { return Scl.x; }
	virtual	float			GetYScale() const { return Scl.y; }
	virtual	float			GetXOrigin() const { return Origin.x; }
	virtual	float			GetYOrigin() const { return Origin.y; }

	virtual	void			NotifySize(int inWidth, int inHeight) 
	{
	}
	virtual	int				GetWidth() const { return Width; }
	virtual	int				GetHeight() const { return Height; }

	// - Text and music symbols services -------------------------------------


	virtual void Draw(const wchar_t* s, Direct2DFont& F, float x, float y)
	{
		EnsureBrush();
		CComPtr< IDWriteTextLayout> tl;

		auto ty = MeasureString(mSys->WriteFactory, F.Text, s,(UINT32)wcslen(s));
		D2D1_RECT_F ly = {};
		ly.left = x;
		ly.top = y;
		ly.right = ly.left + std::get<0>(ty);
		ly.bottom = ly.top + std::get<1>(ty);

		if (F.BaseLine)
		{
			mSys->WriteFactory->CreateTextLayout(s, (UINT32)wcslen(s), F.Text, ly.right, ly.bottom, &tl);
			if (!tl)
				return;
			DWRITE_LINE_METRICS lm[1] = {};
			UINT lineCount = 0;
			tl->GetLineMetrics(lm, 5, &lineCount);
			D2D1_POINT_2F pp;
			pp.x = x;
			pp.y = y;
			pp.y -= lm[0].baseline;
			DWRITE_TEXT_METRICS tm = {};
			tl->GetMetrics(&tm);
//			pp.x /= 1.7;
			mSys->rt->DrawTextLayout(pp, tl, NextColorFont, D2D1_DRAW_TEXT_OPTIONS_NONE);
		}
		else
		{
			ly.left = x;
			ly.top = y;
			ly.right = ly.left + std::get<0>(ty);
			ly.bottom = ly.top + std::get<1>(ty);
			mSys->rt->DrawTextW(s,(UINT32)wcslen(s),F.Text, ly, NextColorFont);
		}
	}

	virtual void			DrawMusicSymbol(float x, float y, unsigned int inSymbolID) 
	{
		wchar_t yy[20] = { 0 };
		swprintf_s(yy,20,L"%c", inSymbolID);
		Draw(yy, mCurrMusicFont, x, y);
	}
	virtual	void			DrawString(float x, float y, const char* s, int inCharCount) 
	{
		yystring yy = s;
		Draw(yy.c_str(), mCurrTextFont, x, y);
	}

	void nop() {}

#pragma warning(disable:4244)
	virtual	void			SetFontColor(const VGColor& inColor) { NextColorFontC = VGColorX(inColor); NextColorFont = 0; }
	virtual	VGColor			GetFontColor() const { return VGColorX(NextColorFontC); }
#pragma warning(default:4244)
	virtual	void			SetFontBackgroundColor(const VGColor& inColor) {}
	virtual	VGColor			GetFontBackgroundColor() const { return VGColor(50); }
	virtual	void			SetFontAlign(unsigned int inAlign) 
	{
		DWRITE_TEXT_ALIGNMENT a1 = DWRITE_TEXT_ALIGNMENT_LEADING;
		DWRITE_PARAGRAPH_ALIGNMENT a2 = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		int BaseLine = 0;

		if (inAlign & kAlignBase)
			BaseLine = 1;
		if (inAlign & kAlignRight)
			a1 = DWRITE_TEXT_ALIGNMENT_TRAILING;
		if (inAlign & kAlignLeft)
			a1 = DWRITE_TEXT_ALIGNMENT_LEADING;

		if (inAlign & kAlignCenter)
			a2 = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		if (inAlign & kAlignTop)
			a2 = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		if (inAlign & kAlignBottom)
			a2 = DWRITE_PARAGRAPH_ALIGNMENT_FAR;


		mCurrTextFont.BaseLine = BaseLine;
		mCurrTextFont.a1 = a1;
		mCurrTextFont.a2 = a2;
		mCurrMusicFont.BaseLine = BaseLine;
		mCurrMusicFont.a1 = a1;
		mCurrMusicFont.a2 = a2;

		if (mCurrMusicFont.Text && mCurrTextFont.Text)
		{
			mCurrTextFont.Text->SetTextAlignment(a1);
			mCurrMusicFont.Text->SetTextAlignment(a1);
			mCurrTextFont.Text->SetParagraphAlignment(a2);
			mCurrMusicFont.Text->SetParagraphAlignment(a2);
		}
	}
	virtual	unsigned int	GetFontAlign() const 
	{ 
		return kAlignBase; 
	}

	// - Printer informations services ----------------------------------------
	virtual	void			SetDPITag(float inDPI = 72.0f) {}
	virtual	float			GetDPITag() const { return 72.f; }
	virtual void* GetBitMapPixels() { return 0; }
	virtual void			ReleaseBitMapPixels() {}

	/// Gives the current device data and returns the data associated mime type.
	virtual const char* GetImageData(const char*& outDataPtr, int& outLength) { outLength = 0; return 0; }
	/// Release the pointer returned by GetImageData
	virtual void			ReleaseImageData(const char*) const {}

	/// temporary hack - must be removed asap
	virtual	VGSystem* getVGSystem() const { return mSys; }

protected:
	/// Returns the platform-specific device context object.
	virtual void* GetNativeContext() const { return 0; }

private:

	float PenW = 1.0f;

public:
	// VGDevice extension
	virtual	void			SelectPenColor(const VGColor& inColor) 
	{
		NextColorPenC = VGColorX(inColor);
		NextColorPen = 0;
	}
	virtual	void			SelectPenWidth(float width) 
	{
		PenW = width;
	}

	virtual	void			PushPenColor(const VGColor& inColor) 
	{
		PenColorPushing.push(NextColorPenC);
		NextColorPenC = VGColorX(inColor);
		NextColorPen = 0;
	}
	virtual	void			PopPenColor() 
	{
		NextColorPenC = PenColorPushing.top();
		PenColorPushing.pop();
		NextColorPen = 0;
	}

	virtual	void			PushPenWidth(float width) {
		PenWidthPushing.push(PenW);
		PenW = width;
	}
	virtual	void			PopPenWidth() 
	{
		PenW = PenWidthPushing.top();
		PenWidthPushing.pop();
	}


	/// Draws an ellipse
	virtual void			FrameEllipse(float x, float y, float width, float height)
	{
		D2D1_ELLIPSE r = { 0 };
		r.point.x = x;
		r.point.y = y;
		r.radiusX = width;
		r.radiusY = height;
		EnsureBrush();
		mSys->rt->DrawEllipse(r, NextColorPen);
	}

	// - Filled surfaces --------------------------------------
	// The raster op mode for color filling should be specified
	// with SetRasterOpMode() before using one of these.

/// Draws a filled ellipse
	virtual void			Ellipse(float x, float y, float width, float height, const VGColor& color)
	{
		D2D1_ELLIPSE r = { 0 };
		r.point.x = x;
		r.point.y = y;
		r.radiusX = width;
		r.radiusY = height;
		EnsureBrush();
		mSys->rt->FillEllipse(r, NextColorFill);
	}

};


Direct2DSystem::Direct2DSystem(ID2D1RenderTarget* trg,IDWriteFactory* wf, ID2D1Factory* fa)
	{
		Factory = fa;
		rt = trg;
		WriteFactory = wf;
	}
Direct2DSystem::~Direct2DSystem() {}

	// - VGDevice services -------------------------------------------
VGDevice* Direct2DSystem::CreateDisplayDevice()
	{
		return new Direct2DDevice(this);
	}
VGDevice* Direct2DSystem::CreateMemoryDevice(int inWidth, int inHeight)
	{
		return new Direct2DDevice(this, inWidth, inHeight);
	}
VGDevice* Direct2DSystem::CreateMemoryDevice(const char* inPath)
	{
		return new Direct2DDevice(this);
	}
VGDevice*  Direct2DSystem::CreatePrinterDevice()
	{
		return new Direct2DDevice(this);
	}

VGDevice*  Direct2DSystem::CreateAntiAliasedMemoryDevice(int inWidth, int inHeight)
	{
		return new Direct2DDevice(this, inWidth, inHeight);
	}

// - Font services -----------------------------------------------
const  VGFont*  Direct2DSystem:: CreateVGFont(const char* faceName, int size, int properties) const
	{
		return new Direct2DFont((Direct2DSystem*)this,faceName, size, properties);
	}


void Direct2DFont::From(const VGFont* font,float Scale)
{
	FLOAT fs = (FLOAT)abs(font->GetSize());
	fs *= Scale;
	
	auto n = font->GetName();
	if (fabs(fs - PrevSize) < 0.1f && PrevName == n || n == std::string(""))
		return;

	PrevSize = fs;
	PrevName = n;
	Text = 0;
	sys->WriteFactory->CreateTextFormat(yystring(n).c_str(), PrevName == std::string("guido2") ? PrivateGuidoFonts : 0, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fs, L"", &Text);

	Text->SetTextAlignment(a1);
	Text->SetParagraphAlignment(a2);
	
}

Direct2DFont :: Direct2DFont(Direct2DSystem* ds, std::string name, int size, int prop)
	: fName(name), fSize(size), fProperties(prop)
{
	sys = ds;
	if (size == 0 || name == "")
		return;
	FLOAT fs = (FLOAT)abs(size);
	Text = 0;
	ds->WriteFactory->CreateTextFormat(yystring(name.c_str()).c_str(),name == std::string("guido2") ? PrivateGuidoFonts : 0, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fs, L"", &Text);

	Text->SetTextAlignment(a1);
	Text->SetParagraphAlignment(a2);
}
