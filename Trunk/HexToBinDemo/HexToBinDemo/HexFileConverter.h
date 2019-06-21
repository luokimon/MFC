#pragma once

#define HEX_MAX_LENGTH		256
#define HEX_MIN_LENGTH		11

class CHexFileConverter
{
public:
	CHexFileConverter();
	CHexFileConverter(const CString str);
	virtual ~CHexFileConverter();

private:
	CString m_strHexPath;
private:
	BYTE CharToHalfByte(const CHAR c);
	BYTE CharToByte(const PCHAR p);
	HEX_CONVERTER_STATUS LineDecode(CString line, PHEX_FILE_FORMAT p);
public:
	BOOL ToBin(PBYTE buff, UINT size);
};

