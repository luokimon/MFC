#pragma once

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
	PBYTE GetBuffer() { return m_buff; }
	PBYTE GetBufferSetLength(UINT len);
	UINT GetSize() { return m_len; }
	UINT GetLength() { return m_len; }
	VOID SetLength(UINT len);
};

