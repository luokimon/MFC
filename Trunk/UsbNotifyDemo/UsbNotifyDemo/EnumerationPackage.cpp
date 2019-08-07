#include "stdafx.h"
#include "EnumerationPackage.h"


CEnumerationPackage::CEnumerationPackage()
{
}

CEnumerationPackage::CEnumerationPackage(GUID guid, CString pre, CString non)
	:m_guid(guid), m_preStr(pre), m_nonexistentStr(non) 
{

}

CEnumerationPackage::~CEnumerationPackage()
{
}

void CEnumerationPackage::operator = (const CEnumerationPackage& o)
{
	if (this != &o)
	{
		m_guid = o.m_guid;
		m_preStr = o.m_preStr;
		m_nonexistentStr = o.m_nonexistentStr;
	}
}