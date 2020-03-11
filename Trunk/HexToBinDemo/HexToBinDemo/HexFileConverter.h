#pragma once

#define HEX_MAX_LENGTH		256
#define HEX_MIN_LENGTH		11

typedef enum {
	RES_OK = 0,
	RES_DATA_TOO_LONG,
	RES_DATA_TOO_SHORT,
	RES_NO_COLON,
	RES_TYPE_ERROR,
	RES_LENGTH_ERROR,
	RES_CHECK_ERROR,
	RES_HEX_FILE_NOT_EXIST,
	RES_HEX_FILE_NO_END
}HEX_CONVERTER_STATUS;

typedef enum {
	hrt_DR = 0x0,	// Data Record
	hrt_EFR = 0x1,	// End of File Record
	hrt_ESAR = 0x2,	// Extended Segment Address Record
	hrt_SSAR = 0x3,	// Start Segment Address Record
	hrt_ELAR = 0x4,	// Extended Linear Address Record
	hrt_SLAR = 0x5	// Start Linear Address Record
}HEX_RECORD_TYPE;

typedef struct {
	BYTE len;
	WORD addr;
	BYTE type;
	PBYTE data;
}HEX_FILE_FORMAT, *PHEX_FILE_FORMAT;

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

