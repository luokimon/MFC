#pragma once
class AFX_EXT_CLASS CUnicodeHelper
{
public:
	CUnicodeHelper();
	virtual ~CUnicodeHelper();

private:
	char* m_char;
	wchar_t* m_wchar;

private:
	void Release();

public:
	static CStringA ToCStringA(const CStringW& str);
	char* WcharToChar(const wchar_t* wc);
	wchar_t* CharToWchar(const char* c);
};

