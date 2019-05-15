#include "stdafx.h"
#include "ProcedureLog.h"


CProcedureLog::CProcedureLog()
{
	m_log = NULL;
}


CProcedureLog::~CProcedureLog()
{
	if (m_log != NULL)
	{
		delete m_log;
		m_log = NULL;
	}
}


void CProcedureLog::CreateLogInstance(PROCEDURE_LOG_TYPE typ)
{
	switch (typ)
	{
	case PROCEDURE_LOG_TYPE::pltHTML:
		m_log = new CHtmlLog();
		break;
	case PROCEDURE_LOG_TYPE::pltTEXT:
		m_log = new CTextLog();
		break;
	default:
		break;
	}
}