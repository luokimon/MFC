#pragma once

//#ifdef  _DLL_LIB_EXPORT_
//#define  DLL_EXPORT   __declspec(dllexport)
//#else
//#define  DLL_EXPORT   __declspec(dllimport)
//#endif

//class DLL_EXPORT CSafeBuff
class AFX_EXT_CLASS CSafeBuff
{
public:
	CSafeBuff();
	CSafeBuff(UINT len);
	virtual ~CSafeBuff();

private:
	PBYTE m_buff;
	UINT m_len;
	CString m_str;

private:
	VOID ReleaseBuffer();
public:
	void SetString(const CString& str) { m_str = str; }
	PBYTE GetBuffer() { return m_buff; }
	PBYTE GetBufferSetLength(UINT len);
	UINT GetSize() { return m_len; }
	UINT GetLength() { return m_len; }
	VOID SetLength(UINT len);
};

