#include "stdafx.h"



CUsbWinUSB::CUsbWinUSB()
{
	m_winUsb = new CWinUsbWrapper();
}


CUsbWinUSB::~CUsbWinUSB()
{
	if (NULL != m_winUsb)
	{
		delete m_winUsb;
		m_winUsb = NULL;
	}
}

BOOL CUsbWinUSB::OpenDevice(LPCTSTR path)
{
	return m_winUsb->OpenDevice(path);
}

void CUsbWinUSB::CloseDevice()
{
	m_winUsb->CloseDevice();
}

BOOL CUsbWinUSB::WriteCommand(CSetupBase& setup)
{
	ULONG readBytes = 0;
	if (!m_winUsb->ControlTransfer(setup.GetSetupPacket(), setup.GetBuffer(), setup.GetLength(), &readBytes))
		return FALSE;

	return TRUE;
}

BOOL CUsbWinUSB::ReadCommand(CSetupBase& setup, PBYTE buff)
{
	ULONG readBytes = 0;
	if (!m_winUsb->ControlTransfer(setup.GetSetupPacket(), setup.GetBuffer(), setup.GetLength(), &readBytes))
		return FALSE;

	if (readBytes == setup.GetLength())
	{
		memcpy(buff, setup.GetBuffer(), setup.GetLength());
		return TRUE;
	}

	return FALSE;
}

BOOL CUsbWinUSB::SetPowerStatus(PBYTE buff, WORD len)
{
	CSetupSetPowerStatus power(buff, len);
	if(!WriteCommand(power))
	{
		TRACE(_T("SetPowerStatus Fail!\r\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL CUsbWinUSB::GetPowerStatus(PBYTE buff, WORD len)
{
	CSetupGetPowerStatus power(len);
	if(!ReadCommand(power, buff))
	{
		TRACE(_T("GetPowerStatus Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::GetBridgeInformation(PBYTE buff, WORD len)
{
	CSetupGetBridgeInformation info(len);
	if (!ReadCommand(info, buff))
	{
		TRACE(_T("GetBridgeInformation Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::GetDeviceInformation(PBYTE buff, WORD len)
{
	CSetupGetDeviceInformation info(len);
	if (!ReadCommand(info, buff))
	{
		TRACE(_T("GetDeviceInformation Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::GetAdbData(WORD mode, PBYTE buff, WORD len)
{
	CSetupGetAdbData data(mode, len);
	if (!ReadCommand(data, buff))
	{
		TRACE(_T("GetAdbData Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::SetDeviceGroup(WORD grp)
{
	CSetupSetDeviceGroup dev(grp);
	if (!WriteCommand(dev))
	{
		TRACE(_T("SetDeviceGroup Fail!\r\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL CUsbWinUSB::SetOperationMode(BYTE major, BYTE minor)
{
	CSetupSetOperationMode mode(major, minor);
	if (!WriteCommand(mode))
	{
		TRACE(_T("SetOperationMode Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::SetDeviceReset()
{
	CSetupSetDeviceReset reset;
	if (!WriteCommand(reset))
	{
		TRACE(_T("SetDeviceReset Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::EepromAccess(BOOL bEnter)
{
	CSetupEepromAccess access(bEnter);
	if (!WriteCommand(access))
	{
		TRACE(_T("EepromAccess Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::EepromEraseAll()
{
	CSetupEepromEraseAll eraseAll;
	if (!WriteCommand(eraseAll))
	{
		TRACE(_T("EepromEraseAll Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::EepromRead(WORD addr, PBYTE buff, WORD len)
{
	CSetupEepromReadWrite read(TRUE, addr, len);
	if (!ReadCommand(read, buff))
	{
		TRACE(_T("EepromRead Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::EepromWrite(WORD addr, PBYTE buff, WORD len)
{
	CSetupEepromReadWrite read(FALSE, addr, len);
	read.SetBuffer(buff);
	if (!WriteCommand(read))
	{
		TRACE(_T("EepromWrite Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::I2CPortMonitor(WORD addr, WORD len)
{
	CSetupI2CPortMonitor monitor(addr, len);
	if (!WriteCommand(monitor))
	{
		TRACE(_T("I2CPortMonitor Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::SPIPortMonitor(BOOL bEnable)
{
	CSetupSPIPortMonitor monitor(bEnable);
	if (!WriteCommand(monitor))
	{
		TRACE(_T("SPIPortMonitor Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::FindI2CAddress()
{
	CSetupFindI2CAddress find;
	if (!WriteCommand(find))
	{
		TRACE(_T("FindI2CAddress Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::BridgeReset()
{
	CSetupBridgeReset reset;
	if (!WriteCommand(reset))
	{
		TRACE(_T("BridgeReset Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::GetDeviceID(PBYTE buff, WORD len)
{
	CSetupGetDeviceID id(len);
	if (!ReadCommand(id, buff))
	{
		TRACE(_T("GetDeviceID Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::GetLdoAdjust(WORD refVal, PBYTE buff, WORD len)
{
	CSetupGetLdoAdjust ldo(refVal, len);
	if (!ReadCommand(ldo, buff))
	{
		TRACE(_T("GetLdoAdjust Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::CheckResetIO()
{
	CSetupCheckResetIO reset;
	if (!WriteCommand(reset))
	{
		TRACE(_T("CheckResetIO Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::GetRCAdjust(PBYTE buff, WORD len)
{
	CSetupGetRCAdjust rc(len);
	if (!ReadCommand(rc, buff))
	{
		TRACE(_T("GetRCAdjust Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::GetTxShort(BYTE delay, BYTE time, PBYTE buff, WORD len)
{
	CSetupGetTxShort tx(delay, time, len);
	if (!ReadCommand(tx, buff))
	{
		TRACE(_T("GetTxShort Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::GetRxShort(BYTE time, PBYTE buff, WORD len)
{
	CSetupGetRxShort rx(time, len);
	if (!ReadCommand(rx, buff))
	{
		TRACE(_T("GetRxShort Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::GetRxAdjust(PBYTE buff, WORD len)
{
	CSetupGetRxAdjust rx(len);
	if (!ReadCommand(rx, buff))
	{
		TRACE(_T("GetRxAdjust Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::CheckIntIO(PBYTE buff, WORD len)
{
	CSetupCheckIntIO io(len);
	if (!ReadCommand(io, buff))
	{
		TRACE(_T("CheckIntIO Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::SetHostTimeout(WORD val)
{
	CSetupSetHostTimeout timeout(val);
	if (!WriteCommand(timeout))
	{
		TRACE(_T("SetHostTimeout Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::SetInfoMonitor(WORD mode)
{
	CSetupSetInfoMonitor info(mode);
	if (!WriteCommand(info))
	{
		TRACE(_T("SetInfoMonitor Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}

BOOL CUsbWinUSB::ForceErase()
{
	CSetupForceErase erase;
	if (!WriteCommand(erase))
	{
		TRACE(_T("SetInfoMonitor Fail!\r\n"));
		return FALSE;
	}
	return TRUE;
}


BOOL CUsbWinUSB::ReadBulkPipe(
	IN  PUCHAR  Buffer,
	IN  ULONG   bufSize)
{
	ULONG lenTransferred = 0;
	*((PINT)Buffer) = 0xFFFFFFFF;
	BOOL bRet =  m_winUsb->ReadBulkPipe(m_winUsb->GetI2CEndpoint(), Buffer, bufSize, &lenTransferred, NULL);

	m_winUsb->FlushPipe(m_winUsb->GetI2CEndpoint());

	return bRet;
}


/*
BOOL CUsbWinUSB::ReadBulkPipe(
	IN  PUCHAR  Buffer,
	IN  ULONG   bufSize)
{
	ULONG lenTransferred = 0;
	OVERLAPPED overlap;

	HANDLE evt = CreateEvent(NULL, TRUE, FALSE, _T("ReadBulkPipe"));
	ZeroMemory(&overlap, sizeof(OVERLAPPED));
	overlap.hEvent = evt;

	*((PINT)Buffer) = 0xFFFFFFFF; // Fix System Bug

	BOOL bRet = m_winUsb->ReadBulkPipe(m_winUsb->GetI2CEndpoint(), Buffer, bufSize, &lenTransferred, &overlap);
	DWORD dwWait = WaitForSingleObject(evt, INFINITE);

	// Fix System Bug
	if (bRet || (*((PINT)Buffer) != 0xFFFFFFFF))
	{
		TRACE("Get Data Success!\r\n");
	}
	else
	{
		m_winUsb->FlushPipe(m_winUsb->GetI2CEndpoint());
		return FALSE;
	}

	m_winUsb->FlushPipe(m_winUsb->GetI2CEndpoint());

	return TRUE;
}
*/