#include "stdafx.h"
#include "RichEditCtrlEx.h"


CRichEditCtrlEx::CRichEditCtrlEx()
{
}


CRichEditCtrlEx::~CRichEditCtrlEx()
{
}

void CRichEditCtrlEx::AddColorText(const CString& str, COLORREF col)
{
	CHARFORMAT cf;

	GetSelectionCharFormat(cf);
	cf.dwEffects = 0;
	cf.crTextColor = col;
	SetSelectionCharFormat(cf);

	SetSel(-1, -1);
	ReplaceSel(str);
}

void CRichEditCtrlEx::AddNormalLog(const CString& str)
{
	AddColorText(str, RGB(0, 0, 0));
}

void CRichEditCtrlEx::AddErrorLog(const CString& str)
{
	AddColorText(str, RGB(255, 0, 0));
}

void CRichEditCtrlEx::AddPassLog(const CString& str)
{
	AddColorText(str, RGB(0, 255, 0));
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
	StreamOut(SF_RTF, es);
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
	StreamIn(SF_RTF, es);
}