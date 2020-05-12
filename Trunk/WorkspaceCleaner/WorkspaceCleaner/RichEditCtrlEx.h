#pragma once
#include <afxcmn.h>

typedef struct _TEXT_COLOR_MAPPING
{
	UINT start;
	UINT stop;
	COLORREF col;
}TEXT_COLOR_MAPPING, *PTEXT_COLOR_MAPPING;

class CRichEditCtrlEx :
	public CRichEditCtrl
{
public:
	CRichEditCtrlEx();
	virtual ~CRichEditCtrlEx();

private:
	CList<TEXT_COLOR_MAPPING, TEXT_COLOR_MAPPING>* m_pColorList;
	CString m_coloredText;
	UINT m_coloredLength;

private:
	void AddColorText(const CString& str, COLORREF col);
	void AddColorListItem(const CString& str, COLORREF col);

public:
	void AddBlackText(const CString& str, BOOL delay = FALSE);
	void AddRedText(const CString& str, BOOL delay = FALSE);
	void AddGreenText(const CString& str, BOOL delay = FALSE);
	void AddBlueText(const CString& str, BOOL delay = FALSE);
	void SaveTo(const CString path);
	void LoadFrom(const CString path);

	void DisplayColoredText();
};

