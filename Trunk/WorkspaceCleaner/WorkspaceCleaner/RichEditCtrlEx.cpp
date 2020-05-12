#include "stdafx.h"
#include "RichEditCtrlEx.h"


CRichEditCtrlEx::CRichEditCtrlEx()
{
	m_pColorList = new CList<TEXT_COLOR_MAPPING, TEXT_COLOR_MAPPING>();
	m_coloredLength = 0;
}


CRichEditCtrlEx::~CRichEditCtrlEx()
{
	if (NULL != m_pColorList)
		delete m_pColorList;
}

void CRichEditCtrlEx::AddColorText(const CString& str, COLORREF col)
{
	
	CHARFORMAT cf;

	//GetSelectionCharFormat(cf);
	GetDefaultCharFormat(cf);
	cf.dwEffects = 0;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = col;
	
	SetFocus();
	SetSel(-1, -1);

	SetSelectionCharFormat(cf);
	HideSelection(FALSE, TRUE);
	ReplaceSel(str);
	HideSelection(TRUE, TRUE);

	//if(-1 != str.Find(_T("\n")))
	//	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CRichEditCtrlEx::AddColorListItem(const CString& str, COLORREF col)
{
	m_coloredText += str;

	TEXT_COLOR_MAPPING txt;
	//txt.start = GetTextLength() + m_coloredLength;
	txt.start = GetTextLengthEx(GTL_DEFAULT) + m_coloredLength;
	//txt.stop = txt.start + str.GetLength()-1;
	txt.stop = txt.start + str.GetLength();
	txt.col = col;

	if (!m_pColorList->IsEmpty())
	{
		if ((m_pColorList->GetTail().stop == txt.start) && (txt.col == m_pColorList->GetTail().col))
			m_pColorList->GetTail().stop = txt.stop;
		else
			m_pColorList->AddTail(txt);
	}
	else
		m_pColorList->AddTail(txt);

	m_coloredLength += str.GetLength();
}

void CRichEditCtrlEx::AddBlackText(const CString& str, BOOL delay)
{
	if (delay)
	{
		CString tmp(str);
		tmp.Replace(_T("\r"), _T(""));
		m_coloredText += tmp;
		m_coloredLength += tmp.GetLength();
	}
	else
		AddColorText(str, RGB(0, 0, 0));
}

void CRichEditCtrlEx::AddRedText(const CString& str, BOOL delay)
{
	if (delay)
	{
		CString tmp(str);
		tmp.Replace(_T("\r"), _T(""));
		AddColorListItem(tmp, RGB(255, 0, 0));
	}
	else
		AddColorText(str, RGB(255, 0, 0));
}

void CRichEditCtrlEx::AddGreenText(const CString& str, BOOL delay)
{
	if (delay)
	{
		CString tmp(str);
		tmp.Replace(_T("\r"), _T(""));
		AddColorListItem(tmp, RGB(0, 255, 0));
	}
	else
		AddColorText(str, RGB(0, 255, 0));
}

void CRichEditCtrlEx::AddBlueText(const CString& str, BOOL delay)
{
	if (delay)
	{
		CString tmp(str);
		tmp.Replace(_T("\r"), _T(""));
		AddColorListItem(tmp, RGB(0, 0, 255));
	}
	else
		AddColorText(str, RGB(0, 0, 255));
}

static DWORD CALLBACK StreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CFile* pFile = (CFile*)dwCookie;

	pFile->Write(pbBuff, cb);
	*pcb = cb;

	return 0;
}

void CRichEditCtrlEx::SaveTo(const CString path)
{
	CFile cFile(path, CFile::modeCreate | CFile::modeWrite);
	EDITSTREAM es;
	es.dwCookie = (DWORD)&cFile;
	es.pfnCallback = StreamOutCallback;
	StreamOut(SF_TEXT, es);
}

static DWORD CALLBACK StreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CFile* pFile = (CFile*)dwCookie;

	*pcb = pFile->Read(pbBuff, cb);

	return 0;
}

void CRichEditCtrlEx::LoadFrom(const CString path)
{
	CFile cFile(path, CFile::modeRead);
	EDITSTREAM es;
	es.dwCookie = (DWORD)&cFile;
	es.pfnCallback = StreamInCallback;
	StreamIn(SF_TEXT, es);
}


void CRichEditCtrlEx::DisplayColoredText()
{
	SetSel(-1, -1);
	ReplaceSel(m_coloredText);

	while (!m_pColorList->IsEmpty())
	{
		TEXT_COLOR_MAPPING txt = m_pColorList->RemoveHead();
		CHARFORMAT cf;
		SetSel(txt.start, txt.stop);
		GetSelectionCharFormat(cf);
		cf.crTextColor = txt.col;
		cf.dwEffects = CFE_AUTOBACKCOLOR;
		SetSelectionCharFormat(cf);
	}

	m_coloredText = _T("");
	m_coloredLength = 0;
}