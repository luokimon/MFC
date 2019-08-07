#pragma once
class CEnumerationPackage
{
public:
	CEnumerationPackage(GUID guid, CString pre, CString non);
	virtual ~CEnumerationPackage();

private:
	GUID m_guid;
	CString m_preStr;
	CString m_nonexistentStr;
private:
	CEnumerationPackage();

public:
	virtual void operator = (const CEnumerationPackage& o);
	GUID GetGuid() { return m_guid; }
	CString GetPrefixString() { return m_preStr; }
	CString GetNonexistentString() { return m_nonexistentStr; }
};

