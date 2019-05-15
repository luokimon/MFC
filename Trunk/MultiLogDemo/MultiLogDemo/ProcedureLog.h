#pragma once

enum PROCEDURE_LOG_TYPE
{
	pltHTML,
	pltTEXT
};

class CProcedureLog
{
public:
	CProcedureLog();
	virtual ~CProcedureLog();

public:
	CBaseLog* m_log;

public:
	void CreateLogInstance(PROCEDURE_LOG_TYPE typ);
	PBYTE GetContent() { return m_log->GetContent(); }
	UINT GetLength() { return m_log->GetLength(); }

	void WriteLine(LPCTSTR dat, COLORREF color = 0) { m_log->WriteLine(dat, color); }
	void WriteData(LPCTSTR dat, COLORREF color = 0) { m_log->WriteData(dat, color); }

	template <class T>
	void WriteArray(UINT count, T* array, BYTE* validArray)
	{
		CString str;
		m_log->AddTableStart();
		m_log->AddTableRowStart();

		for (UINT i = 0; i < count; i++)
		{
			str.Format("%d, ", array[i]);
			if(0 == validArray[i])
				m_log->AddTableUnitData(str, RGB(255, 0, 0));
			else
				m_log->AddTableUnitData(str);
		}

		m_log->AddTableRowEnd();
		m_log->AddTableEnd();
	}

	template <class T>
	void WriteTable(UINT tx, UINT rx, T** table, BYTE** validTable)
	{
		CString str;
		m_log->AddTableStart();

		for (UINT i = 0; i < tx; i++)
		{
			m_log->AddTableRowStart();
			for (UINT j = 0; j < rx; j++)
			{
				str.Format("%d, ", table[i][j]);
				if (0 == validTable[i][j])
					m_log->AddTableUnitData(str, RGB(255, 0, 0));
				else
					m_log->AddTableUnitData(str);
			}
			m_log->AddTableRowEnd();
		}

		m_log->AddTableEnd();
	}
};

