#include "stdafx.h"



CUsbWinUSB::CUsbWinUSB()
{
	m_hDevice = INVALID_HANDLE_VALUE;
	m_nMaxEndpoints = 0;
	m_winUsbHandle = NULL;
	m_pipeInfoList = new std::vector<WINUSB_PIPE_INFORMATION>();
}


CUsbWinUSB::~CUsbWinUSB()
{
	CloseDevice();
}

BOOL CUsbWinUSB::OpenDevice(LPCTSTR path)
{
	if (NULL == path)
		return FALSE;

	for (int i = 0; i < CREATE_FILE_RETRY; i++)
	{
		m_hDevice = CreateFile(path,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			NULL);

		if (INVALID_HANDLE_VALUE == m_hDevice)
			continue;
		else
			break;
	}

	if (INVALID_HANDLE_VALUE == m_hDevice)
		return FALSE;

	m_devPath = path;

	return WinUsbInitialize();
}

void CUsbWinUSB::CloseDevice()
{
	if (m_winUsbHandle != NULL)
	{
		WinUsb_Free(m_winUsbHandle);
		m_winUsbHandle = NULL;
	}

	if (m_hDevice != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDevice);
		m_hDevice = INVALID_HANDLE_VALUE;
	}

	if (m_pipeInfoList->size() > 0)
		m_pipeInfoList->clear();
	delete m_pipeInfoList;
}


BOOL CUsbWinUSB::WinUsbInitialize()
{
	if (NULL != m_winUsbHandle)
		WinUsb_Free(m_winUsbHandle);

	if (!WinUsb_Initialize(m_hDevice, &m_winUsbHandle))
		return FALSE;
	
	do
	{
		USB_INTERFACE_DESCRIPTOR ifaceDescriptor;
		if (!WinUsb_QueryInterfaceSettings(m_winUsbHandle, 0, &ifaceDescriptor))
		{
			break;
		}

		m_nMaxEndpoints = ifaceDescriptor.bNumEndpoints;
		ULONG timeout = 512;
		for (int i = 0; i < ifaceDescriptor.bNumEndpoints; i++)
		{
			WINUSB_PIPE_INFORMATION  pipeInfo;
			ZeroMemory(&pipeInfo, sizeof(WINUSB_PIPE_INFORMATION));

			WinUsb_QueryPipe(m_winUsbHandle,
				0,				// AlternateInterfaceNumber
				i,				// PipeIndex
				&pipeInfo);

			//if (!SetPipePolicy(pipeInfo.PipeId, pipeInfo.PipeType, PIPE_TRANSFER_TIMEOUT))
			WinUsb_SetPipePolicy(m_winUsbHandle,
				pipeInfo.PipeId,
				PIPE_TRANSFER_TIMEOUT,
				sizeof(ULONG),
				&timeout);

			m_pipeInfoList->push_back(pipeInfo);
		}

		return TRUE;
	} while (FALSE);

	if (m_winUsbHandle != NULL)
	{
		WinUsb_Free(m_winUsbHandle);
		m_winUsbHandle = NULL;
	}

	return FALSE;
}

BOOL CUsbWinUSB::ControlTransfer(WINUSB_SETUP_PACKET setupPacket,
	PBYTE				 Buffer,
	ULONG				 BufferLength,
	PULONG				 ReadBytes)
{
	BOOL  bResult = TRUE;

	if (m_winUsbHandle == NULL)
		return FALSE;

	if ((BufferLength > 0) && (NULL == Buffer))
		return FALSE;

	bResult = WinUsb_ControlTransfer(m_winUsbHandle,
		setupPacket,
		Buffer,
		BufferLength,
		ReadBytes,
		NULL);

	return bResult;
}

BOOL CUsbWinUSB::ReadBulkPipe(IN  BYTE	  pipeID,
	IN  PUCHAR  Buffer,
	IN  ULONG   bufSize,
	OUT PULONG  bytesRead,
	IN  LPOVERLAPPED  Overlapped)
{
	return WinUsb_ReadPipe(m_winUsbHandle,
		pipeID,
		Buffer,
		bufSize,
		bytesRead,
		Overlapped);
}


BOOL CUsbWinUSB::WriteCommand(CSetupBase& setup)
{
	ULONG readBytes = 0;
	if (!ControlTransfer(setup.GetSetupPacket(), setup.GetBuffer(), setup.GetLength(), &readBytes))
		return FALSE;

	return TRUE;
}

BOOL CUsbWinUSB::ReadCommand(CSetupBase& setup, PBYTE buff)
{
	ULONG readBytes = 0;
	if (!ControlTransfer(setup.GetSetupPacket(), setup.GetBuffer(), setup.GetLength(), &readBytes))
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