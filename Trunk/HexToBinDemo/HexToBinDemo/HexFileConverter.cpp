#include "stdafx.h"
#include "HexFileConverter.h"



CHexFileConverter::CHexFileConverter()
{
}

CHexFileConverter::CHexFileConverter(const CString str)
{
	m_strHexPath = str;
}

CHexFileConverter::~CHexFileConverter()
{
}

BYTE CHexFileConverter::CharToHalfByte(const CHAR c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0xFF;
}

BYTE CHexFileConverter::CharToByte(const PCHAR p)
{
	BYTE tmp;

	tmp = CharToHalfByte(p[0]);
	tmp <<= 4;
	tmp |= CharToHalfByte(p[1]);

	return tmp;
}

BOOL CHexFileConverter::ToBin(PBYTE buff, UINT size)
{
	if (m_strHexPath.IsEmpty())
		return FALSE;

	std::unique_ptr<BYTE> dat(new BYTE[256]);
	HEX_FILE_FORMAT hex;
	hex.data = dat.get();
	BOOL bRet = FALSE;

	CStdioFile file;
	if (file.Open(m_strHexPath, CFile::modeRead))
	{
		bRet = TRUE;
		CString str;
		while (file.ReadString(str))
		{
			if (RES_OK != LineDecode(str, &hex))
			{
				bRet = FALSE;
				break;
			}

			switch (hex.type)
			{
			case HEX_RECORD_TYPE::hrt_DR:
			{
				if (hex.addr + hex.len >= size)
				{
					bRet = FALSE;
					break;
				}
				memcpy(buff + hex.addr, hex.data, hex.len);
			}
				break;
			case HEX_RECORD_TYPE::hrt_EFR:
				break;
			case HEX_RECORD_TYPE::hrt_ESAR:
			case HEX_RECORD_TYPE::hrt_SSAR:
			case HEX_RECORD_TYPE::hrt_ELAR:
			case HEX_RECORD_TYPE::hrt_SLAR:
			default:
				break;
			}
			if (!bRet)
				break;
		}

		file.Close();
	}

	return bRet;
}

HEX_CONVERTER_STATUS CHexFileConverter::LineDecode(CString line, PHEX_FILE_FORMAT p)
{
	BYTE tmp[4] = { 0 };
	BYTE check = 0;
	UINT num = 0;
	UINT offset = 0;
	UINT len = line.GetLength();

	if (len > HEX_MAX_LENGTH)
		return RES_DATA_TOO_LONG;
	if (len < HEX_MIN_LENGTH)
		return RES_DATA_TOO_SHORT;
	if (line.GetAt(0) != ':')
		return RES_NO_COLON;
	if ((len - 1) % 2 != 0)
		return RES_LENGTH_ERROR;

	UINT binLen = (len - 1) / 2;
	while (num < 4)
	{
		offset = (num << 1) + 1;
		tmp[num] = CharToByte(line.GetBuffer() + offset);
		check += tmp[num];
		num++;
	}

	p->len = tmp[0];
	p->addr = tmp[1];
	p->addr <<= 8;
	p->addr += tmp[2];
	p->type = tmp[3];

	while (num < binLen)
	{
		offset = (num << 1) + 1;
		p->data[num - 4] = CharToByte(line.GetBuffer() + offset);
		check += p->data[num - 4];
		num++;
	}

	if (p->len != binLen - 5)
		return RES_LENGTH_ERROR;

	if (check != 0)
		return RES_CHECK_ERROR;

	return RES_OK;
}
