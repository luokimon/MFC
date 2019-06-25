#pragma once

class AFX_EXT_CLASS CUnicodeConverter
{
public:
	CUnicodeConverter();
	virtual ~CUnicodeConverter();

private:
	char* m_char;
	wchar_t* m_wchar;

private:
	void Release();

public:
	char* WcharToChar(const wchar_t* wc); 
	wchar_t* CharToWchar(const char* c);
};

