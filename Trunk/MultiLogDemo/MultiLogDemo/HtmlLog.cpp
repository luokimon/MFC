#include "stdafx.h"
#include "HtmlLog.h"


CHtmlLog::CHtmlLog()
{
	SetCellWidth(70);
	SetTableBorderThickness(0);
}

CHtmlLog::~CHtmlLog()
{
}

void CHtmlLog::WriteLine(LPCTSTR dat, COLORREF color)
{
	CString str;
	str.Format(_T("<Font color=\"#%06X\">%s</Font><br>\r\n"), ColorToHex(color), dat);
	m_content += str;
}

void CHtmlLog::WriteData(LPCTSTR dat, COLORREF color)
{
	CString str;
	str.Format(_T("<Font color=\"#%06X\">%s</Font>"), ColorToHex(color), dat);
	m_content += str;
}

void CHtmlLog::AddBodyStart()
{
	CString str;
	str.Format(_T("<!DOCTYPE HTML>\r\n \
		<html>\r\n \
		<head>\r\n \
		<basefont face=\"Courier New\" size=\"2.5\"/>\r\n \
		<meta charset = \"utf-8\">\r\n \
		<title>TSP Inspector Log</title>\r\n \
		</head>\r\n \
		<body>\r\n \
		<a style=\"display:scroll; position:fixed; bottom:10px; right:10px; \" href=\"#\" title=Top>Top</a>\r\n"));

	m_content += str;
}

void CHtmlLog::AddBodyEnd()
{
	CString str;
	str.Format(_T("</body>\r\n</html>\r\n"));
	m_content += str;
}

void CHtmlLog::AddTableStart() 
{
	CString str;
	str.Format(_T("<table border=\"%d\">\r\n"), m_tblBorderThickness);
	m_content += str;
}
void CHtmlLog::AddTableEnd() 
{ 
	m_content += _T("</table>\r\n<br>"); 
}

void CHtmlLog::AddTableRowStart() 
{ 
	m_content += _T("<tr>\r\n"); 
}

void CHtmlLog::AddTableRowEnd()
{ 
	m_content += _T("</tr>\r\n"); 
}

UINT CHtmlLog::ColorToHex(COLORREF color)
{
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

void CHtmlLog::AddTableUnitData(CString& dat, COLORREF color)
{
	CString str;
	str.Format(_T("<td width=\"%d\"><Font color=\"#%06X\">%s</Font></td>\r\n"), m_cellWidth, ColorToHex(color),dat);
	m_content += str;
}