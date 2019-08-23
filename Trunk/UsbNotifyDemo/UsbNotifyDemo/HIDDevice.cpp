#include "stdafx.h"
#include "HIDDevice.h"
#include "CommandBase.h"


CHIDDevice::CHIDDevice()
{
}

CHIDDevice::CHIDDevice(const CString& name)
	: CHIDDevice()
{
	m_strName = new CString(name);
}

CHIDDevice::~CHIDDevice()
{
}

BOOL CHIDDevice::OpenDeviceHandle()
{
	CloseDeviceHandle();

	if (m_strName->IsEmpty())
		return FALSE;

	for (int i = 0; i < CREATE_FILE_RETRY; i++)
	{
		m_hDisk = CreateFile(m_strName->GetBuffer(),
			//GENERIC_READ | GENERIC_WRITE,
			0,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			0, OPEN_EXISTING,
			0, 0);

		if (m_hDisk != INVALID_HANDLE_VALUE)
			break;

		Sleep(200);
	}
	return (m_hDisk != INVALID_HANDLE_VALUE);
}

BOOL CHIDDevice::WriteRam(PBYTE buff, USHORT len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == buff)
		return FALSE;

	int cnt = (len + MAX_PACKAGE_DATA_SIZE-1) / MAX_PACKAGE_DATA_SIZE;
	for (USHORT i = 0; i < cnt; i++)
	{
		BYTE buf[64] = { 0 };
		BYTE packageLen = (i == cnt - 1) ? len % MAX_PACKAGE_DATA_SIZE : MAX_PACKAGE_DATA_SIZE;
		//CreateUsbUpgradeHeader(buf, i, packageLen, i == cnt - 1);
		//CreateUsbHeader(buf, i, packageLen, i == cnt - 1, USB_SUB_COMMAND::uscWriteData, len);
		CreateCommandHeader(buf, i, packageLen, i == cnt - 1, 0, len);
		memcpy(&buf[6], &buff[i * MAX_PACKAGE_DATA_SIZE], packageLen);

		//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
		if (!SendCommand(m_hDisk, buf, 64))
		{
			//CString str;
			//str.Format(_T("Write Ram failed:%s"), LastErrorCodeToString(GetLastError()));
			//AfxMessageBox(str);
			CWarningMessage::Show(_T("Write Ram failed:"));
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CHIDDevice::ReadRam(PBYTE buff, USHORT len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == buff)
		return FALSE;

	int cnt = (len + MAX_HID_RAM_SIZE - 1) / MAX_HID_RAM_SIZE;
	PBYTE pBuf = NULL;
	for (USHORT i = 0; i < cnt; i++)
	{
		BYTE buf[64] = { 0 };

		WORD size = ((len - i * MAX_HID_RAM_SIZE) > MAX_HID_RAM_SIZE) ? MAX_HID_RAM_SIZE : (len - i * MAX_HID_RAM_SIZE);

		CreateUsbHeader(buf, 0, 3, TRUE, USB_SUB_COMMAND::uscReadRam, size);

		//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
		if (!SendCommand(m_hDisk, buf, 64))
		{
			//CString str;
			//str.Format(_T("Read Ram failed:%s"), LastErrorCodeToString(GetLastError()));
			//AfxMessageBox(str);
			CWarningMessage::Show(_T("Read Ram failed:"));
			return FALSE;
		}

		pBuf = &buff[i*MAX_HID_RAM_SIZE];
		//pBuf[0] = 0x04;
		//if (!HidD_GetInputReport(m_hDisk, pBuf, size))
		if (!ReadData(m_hDisk, pBuf, size))
		{
			//CString str;
			//str.Format(_T("(Read Ram)Get Input Report failed:%s"), LastErrorCodeToString(GetLastError()));
			//AfxMessageBox(str);
			CWarningMessage::Show(_T("(Read Ram)Get Input Report failed:"));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CHIDDevice::EraseAll()
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = { 0 };
	CreateUsbHeader(buf, 0, 1, TRUE, USB_SUB_COMMAND::uscEraseAll);

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("Erase All Failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("Erase All Failed:"));
		return FALSE;
	}

	//Sleep(100);

	return TRUE;
}

BOOL CHIDDevice::HIDReset()
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = { 0 };
	CreateUsbHeader(buf, 0, 1, TRUE, USB_SUB_COMMAND::uscDeviceReset);
	HidD_SetOutputReport(m_hDisk, buf, 64); // Bypass reset error

	return TRUE;
}

BOOL CHIDDevice::RCAdjust(const PBYTE buff, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = { 0 };
	CreateTouchHeader(buf, 0, 3, TRUE, (TOUCH_SUB_COMMAND)0x50);

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("RCAdjust failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("RCAdjust failed:"));
		return FALSE;
	}

	//Sleep(100);

	if (!ReadData(m_hDisk, buff, len))
	{
		CWarningMessage::Show(_T("(RC Adjust)Get Input Report failed"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::EnableProgram(USHORT len, USHORT offset)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = {0};
	CreateUsbHeader(buf, 0, 5, TRUE, USB_SUB_COMMAND::uscStartSpiProgram, len, offset);
	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("Enable Program Failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("Enable Program Failed:"));
		return FALSE;
	}

	//Sleep(100);

	return TRUE;
}

BOOL CHIDDevice::ReadSpi(PBYTE buff, USHORT len, USHORT offset)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == buff)
		return FALSE;

	BYTE buf[64] = { 0 };
	CreateUsbHeader(buf, 0, 5, TRUE, USB_SUB_COMMAND::uscReadSpi, len, offset);
	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		CWarningMessage::Show(_T("Read Spi Failed"));
		return FALSE;
	}
	Sleep(100);

	if (!ReadData(m_hDisk, buff, len))
	{
		CWarningMessage::Show(_T("(Read Spi)Get Input Report failed"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::SetDebugLog(BOOL mode, BYTE type)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	TRACE("SetDebugLog start\r\n");

	BYTE buf[64] = { 0 };
	CSetAdbDebugLog logMode(type);
	CreateCommandHeader(buf, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, 1024, logMode.GetBuffer(),300);

	if (!SendCommand(m_hDisk, buf, 64))
	{
		CWarningMessage::Show(_T("Set DebugLogType failed:"));
		return FALSE;
	}

	ZeroMemory(buf, 64);
	CreateUsbHeader(buf, 0, 3, TRUE, USB_SUB_COMMAND::uscGetDebugLog, 2048);
	buf[7] = mode;

	if (!SendCommand(m_hDisk, buf, 64))
	{
		CWarningMessage::Show(_T("GetDebugLog failed"));
		return FALSE;
	}

	TRACE("SetDebugLog End\r\n");
	return TRUE;
}

BOOL CHIDDevice::GetDebugLog(PBYTE buff, UINT len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (!ReadData(m_hDisk, buff, len))
	{
		CWarningMessage::Show(_T("Eeprom_Read failed:"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::SetUpdateMode(BOOL mode)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;
	TRACE("SetUpdateMode start\r\n");
	BYTE buf[64] = { 0 };
	CreateUsbHeader(buf, 0, 3, TRUE, USB_SUB_COMMAND::uscSetUpdateMode);
	if (mode)
		buf[7] = 0x00;
	else
		buf[7] = 0x01;

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("SetUpdateMode failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("SetUpdateMode failed"));
		return FALSE;
	}

	//Sleep(100);

	TRACE("SetUpdateMode End\r\n");
	return TRUE;
}

void CHIDDevice::CreateTouchUpgradeHeader(PBYTE buff,
	USHORT sn,
	BYTE packageLen,
	BOOL lastData)
{
	CreateHidHeader(buff, HID_WRITE_TYPE::hwtTouch, sn, packageLen, lastData);
}

void CHIDDevice::CreateUsbUpgradeHeader(PBYTE buff,
	USHORT sn,
	BYTE packageLen,
	BOOL lastData)
{
	CreateHidHeader(buff, HID_WRITE_TYPE::hwtUsb, sn, packageLen, lastData);
}

void CHIDDevice::CreateCommandHeader(PBYTE buff,
	USHORT sn,
	BYTE packageLen,
	BOOL lastData,
	BYTE hct,
	USHORT totalLen,
	WORD offset)
{
	CreateHidHeader(buff, HID_WRITE_TYPE::hwtCommand, sn, packageLen, lastData, (HID_COMMAND_TYPE)hct, totalLen, offset);
}

void CHIDDevice::CreateUsbHeader(PBYTE buff,
	USHORT sn,
	BYTE packageLen,
	BOOL lastData,
	USB_SUB_COMMAND usc,
	USHORT totalLen,
	WORD offset)
{
	CreateHidHeader(buff, HID_WRITE_TYPE::hwtUsb, sn, packageLen, lastData, (HID_COMMAND_TYPE)usc, totalLen, offset);
}

void CHIDDevice::CreateTouchHeader(PBYTE buff,
	USHORT sn,
	BYTE packageLen,
	BOOL lastData,
	TOUCH_SUB_COMMAND tsc,
	USHORT totalLen,
	WORD offset)
{
	CreateHidHeader(buff, HID_WRITE_TYPE::hwtTouch, sn, packageLen, lastData, (HID_COMMAND_TYPE)tsc, totalLen, offset);
}

void CHIDDevice::CreateHidHeader(PBYTE buff,
	HID_WRITE_TYPE hwt,
	USHORT sn,
	BYTE packageLen,
	BOOL lastData,
	HID_COMMAND_TYPE hct,
	USHORT totalLen,
	USHORT offset)
{
	buff[0] = 0x04;//TW包头
	buff[1] = 0x57;//TW包头
	buff[2] = hwt;//0 : cmd       1 : 318升级      2 : 8380升级（2模式下，只下发代码，当校验内存代码通过之后再单独发编程命令）
	buff[3] = (sn & 0xFF00) >> 8;//包的序号 3H4L
	buff[4] = sn & 0xFF;         //包的序号 3H4L
	buff[5] = lastData ? 0x80 | packageLen : packageLen;//64byte 有效length（最大58）  最高位结束标志为1
	buff[6] = hct;//cmd（用于8380的命令）   01读内存，02读spi，10编程
	buff[7] = (totalLen & 0xFF00) >> 8; //spi bin文件的总长度(加了512之后的size) 7H8L
	buff[8] = totalLen & 0xFF;          //
	buff[9] = (offset & 0xFF00) >> 8;
	buff[10] = offset & 0xFF;
}
void CHIDDevice::Create3118Header(PBYTE buff,
	HID_WRITE_TYPE hwt,
	USHORT sn,
	BYTE packageLen,
	BOOL lastData,
	TOUCH_SUB_COMMAND hct,
	USHORT totalLen,
	PBYTE CMD )
{
	buff[0] = 0x04;//TW包头
	buff[1] = 0x57;//TW包头
	buff[2] = hwt;//0 : cmd       1 : 318升级      2 : 8380升级（2模式下，只下发代码，当校验内存代码通过之后再单独发编程命令）
	buff[3] = (sn & 0xFF00) >> 8;//包的序号 3H4L
	buff[4] = sn & 0xFF;         //包的序号 3H4L
	buff[5] = lastData ? 0x80 | packageLen : packageLen;//64byte 有效length（最大58）  最高位结束标志为1
	buff[6] = hct;//cmd（用于8380的命令）   01读内存，02读spi，10编程
	buff[7] = (totalLen & 0xFF00) >> 8; //spi bin文件的总长度(加了512之后的size) 7H8L
	buff[8] = totalLen & 0xFF;          //
	if (CMD == NULL)return;
	buff[9] = CMD[0];
	buff[10] = CMD[1];
	buff[11] = CMD[2];
}

void CHIDDevice::CreateCommandHeader(PBYTE buff,
	USHORT sn,
	BYTE packageLen,
	BOOL lastData,
	TOUCH_SUB_COMMAND tsc,
	USHORT totalLen,
	PBYTE cmd,
	USHORT timeout
	)
{
	buff[0] = 0x04;//TW包头
	buff[1] = 0x57;//TW包头
	buff[2] = HID_WRITE_TYPE::hwtTouch;
	buff[3] = (sn & 0xFF00) >> 8;//包的序号 3H4L
	buff[4] = sn & 0xFF;         //包的序号 3H4L
	buff[5] = lastData ? 0x80 | packageLen : packageLen;//64byte 有效length（最大58）  最高位结束标志为1
	buff[6] = tsc;
	buff[7] = (totalLen & 0xFF00) >> 8;				// 7H8L
	buff[8] = totalLen & 0xFF;						// 7H8L
	memcpy(&buff[9], cmd, COMMAND_REGISTER_SIZE);	// 命令寄存器数据
	*((PUSHORT)&buff[9 + COMMAND_REGISTER_SIZE]) = timeout;
}

BOOL CHIDDevice::FirmwareUpgrade(PBYTE buff, DWORD len)
{
	std::unique_ptr<BYTE[]> readBuff(new BYTE[MAX_HID_RAM_SIZE]);

	SetTaskPosition(0);

	BOOL ret = FALSE;
	for (int retry = 0; retry < 3; retry++)
	{
		UINT cnt = (len + MAX_HID_RAM_SIZE-1) / MAX_HID_RAM_SIZE;
		PBYTE buf;
		BOOL fail = FALSE;

		//if (!SetUpdateMode(FALSE))
		//	continue;

		if (!EraseAll())
			continue;

		for (UINT i = 0; i < cnt; i++)
		{
			buf = &buff[i * MAX_HID_RAM_SIZE];
			UINT sLen = ((len - i * MAX_HID_RAM_SIZE) > MAX_HID_RAM_SIZE) ? MAX_HID_RAM_SIZE : (len - i * MAX_HID_RAM_SIZE);
			UINT offset = i * MAX_HID_RAM_SIZE;
			if (!WriteRam(buf, sLen))
			{
				fail = TRUE;
				CString str;
				str.Format("Write Ram:%d", i + 1);
				//AfxMessageBox(str);
				CWarningMessage::Show(str);
				break;
			}

			ZeroMemory(readBuff.get(), MAX_HID_RAM_SIZE);
			if (!ReadRam(readBuff.get(), sLen))
			{
				fail = TRUE;
				CString str;
				str.Format("Read Ram:%d", i + 1);
				//AfxMessageBox(str);
				CWarningMessage::Show(str);
				break;
			}

			if (0 != memcmp(buf, readBuff.get(), sLen))
			{
				fail = TRUE;
				CString str;
				str.Format("Read Ram memcmp:%d", i + 1);
				//AfxMessageBox(str);
				CWarningMessage::Show(str);
				break;
			}

			if (!EnableProgram(sLen, i*MAX_HID_RAM_SIZE))
			{
				fail = TRUE;
				break;
			}

			ZeroMemory(readBuff.get(), MAX_HID_RAM_SIZE);
			if (!ReadSpi(readBuff.get(), sLen, i*MAX_HID_RAM_SIZE))
			{
				fail = TRUE;
				CString str;
				str.Format("Read Spi memcmp:%d", i + 1);
				//AfxMessageBox(str);
				CWarningMessage::Show(str);
				break;
			}

			if (0 != memcmp(buf, readBuff.get(), sLen))
			{
				fail = TRUE;
				CString str;
				str.Format("Read Spi memcmp:%d", i + 1);
				//AfxMessageBox(str);
				CWarningMessage::Show(str);
				break;
			}
		}

		if (fail)
			continue;

		ret = TRUE;
		break;
	}

	//SetUpdateMode(TRUE);
	SetTaskPosition(100);
	return ret;
}
BOOL CHIDDevice::DeviceReset(PBYTE buff, DWORD len)
{ 
	return HIDReset();
}

BOOL CHIDDevice::SPI_EraseAll()
{
	if (!EraseAll())
		return FALSE;
	return TRUE;
}

BOOL CHIDDevice::SPI_Read(PBYTE buff, DWORD len)
{
	UINT cnt = (len + MAX_HID_RAM_SIZE - 1) / MAX_HID_RAM_SIZE;
	BOOL fail = FALSE;

	SetTaskPosition(0);

	for (UINT i = 0; i < cnt; i++)
	{
		SetTaskPosition((i * 100) / cnt);

		UINT sLen = ((len - i * MAX_HID_RAM_SIZE) > MAX_HID_RAM_SIZE) ? MAX_HID_RAM_SIZE : (len - i * MAX_HID_RAM_SIZE);
		UINT offset = i * MAX_HID_RAM_SIZE;
		if (!ReadSpi(&buff[i * MAX_HID_RAM_SIZE], sLen, i*MAX_HID_RAM_SIZE))
		{
			fail = FALSE;
			CString str;
			str.Format("Read Spi memcmp:%d", i + 1);
			AfxMessageBox(str);
			break;
		}
		fail = TRUE;
	}

	SetTaskPosition(100);

	return fail;
}

BOOL CHIDDevice::Eeprom_EraseAll()
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[MAX_HID_PACKAGE_SIZE] = { 0 };
	CreateTouchHeader(buf, 0, 1, TRUE, TOUCH_SUB_COMMAND::tscEraseAll);

	//if (!HidD_SetOutputReport(m_hDisk, buf, MAX_HID_PACKAGE_SIZE))
	if (!SendCommand(m_hDisk, buf, MAX_HID_PACKAGE_SIZE))
	{
		//CString str;
		//str.Format(_T("Eeprom_EraseAll failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("Eeprom_EraseAll failed:"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::Eeprom_ForceErase()
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[MAX_HID_PACKAGE_SIZE] = { 0 };
	CreateTouchHeader(buf, 0, 1, TRUE, TOUCH_SUB_COMMAND::tscEepromForceErase);

	//if (!HidD_SetOutputReport(m_hDisk, buf, MAX_HID_PACKAGE_SIZE))
	if (!SendCommand(m_hDisk, buf, MAX_HID_PACKAGE_SIZE))
	{
		//CString str;
		//str.Format(_T("Eeprom_ForceErase failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("Eeprom_ForceErase failed:"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::SendCommand(HANDLE handle, PBYTE buff, ULONG len)
{
	BOOL bRet = FALSE;
	for (int i = 0; i < COMMAND_RETRY_COUNT; i++)
	{
		if (HidD_SetOutputReport(handle, buff, len))
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

BOOL CHIDDevice::ReadData(HANDLE handle, PBYTE buff, ULONG len)
{
	buff[0] = 0x04;
	BOOL bRet = FALSE;
	for (int i = 0; i < READ_DATA_RETRY_COUNT; i++)
	{
		if (HidD_GetInputReport(m_hDisk, buff, len))
		{
			bRet = TRUE;
			break;
		}
	}
	
	return bRet;
}

BOOL CHIDDevice::Touch_Erase(WORD addr, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[MAX_HID_PACKAGE_SIZE] = { 0 };
	CreateTouchHeader(buf, 0, 5, TRUE, TOUCH_SUB_COMMAND::tscErase, len, addr);

	//if (!HidD_SetOutputReport(m_hDisk, buf, MAX_HID_PACKAGE_SIZE))
	if (!SendCommand(m_hDisk, buf, MAX_HID_PACKAGE_SIZE))
	{
		//CString str;
		//str.Format(_T("Touch_Erase failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("Touch_Erase failed:"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::Touch_EnableProgram(WORD addr, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = { 0 };
	CreateTouchHeader(buf, 0, 5, TRUE, TOUCH_SUB_COMMAND::tscStartProgram, len, addr);

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("Touch_EnableProgram failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("Touch_EnableProgram failed:"));
		return FALSE;
	}

	//Sleep(100);
	return TRUE;
}

BOOL CHIDDevice::Touch_EnablePageProgram(WORD addr, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = { 0 };
	CreateTouchHeader(buf, 0, 5, TRUE, TOUCH_SUB_COMMAND::tscStartPageProgram, len, addr);

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("Touch_EnablePageProgram failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("Touch_EnablePageProgram failed:"));
		return FALSE;
	}

	//Sleep(100);

	return TRUE;
}

BOOL CHIDDevice::Eeprom_Read(PBYTE buff, WORD addr, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == buff)
		return FALSE;

	UINT cnt = (len + MAX_HID_RAM_SIZE - 1) / MAX_HID_RAM_SIZE;
	PBYTE pBuf = NULL;
	for (UINT i = 0; i < cnt; i++)
	{
		pBuf = &buff[i*MAX_HID_RAM_SIZE];
		WORD size = ((len - i * MAX_HID_RAM_SIZE) > MAX_HID_RAM_SIZE) ? MAX_HID_RAM_SIZE : (len - i * MAX_HID_RAM_SIZE);

		BYTE buf[64] = { 0 };
		CreateTouchHeader(buf, 0, 5, TRUE, TOUCH_SUB_COMMAND::tscReadEeprom, size, addr+i* MAX_HID_RAM_SIZE);

		//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
		if (!SendCommand(m_hDisk, buf, 64))
		{
			//CString str;
			//str.Format(_T("Eeprom_Read failed:%s"), LastErrorCodeToString(GetLastError()));
			//AfxMessageBox(str);
			CWarningMessage::Show(_T("Eeprom_Read failed:"));
			return FALSE;
		}		
		//Sleep(100);

		//pBuf[0] = 0x04;
		//if (!HidD_GetInputReport(m_hDisk, pBuf, size))
		if (!ReadData(m_hDisk, pBuf, size))
		{
			//CString str;
			//str.Format(_T("Eeprom_Read failed:%s"), LastErrorCodeToString(GetLastError()));
			//AfxMessageBox(str);
			CWarningMessage::Show(_T("Eeprom_Read failed:"));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CHIDDevice::Eeprom_Write(PBYTE buff, WORD addr, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == buff)
		return FALSE;

	UINT pageCnt = (len + EEPROM_PAGE_SIZE - 1) / EEPROM_PAGE_SIZE;
	UINT cnt = (pageCnt * EEPROM_PAGE_SIZE + MAX_HID_RAM_SIZE - 1) / MAX_HID_RAM_SIZE;
	PBYTE pBuf = NULL;
	for (UINT i = 0; i < cnt; i++)
	{
		pBuf = &buff[i*MAX_HID_RAM_SIZE];
		WORD size = ((pageCnt * EEPROM_PAGE_SIZE - i * MAX_HID_RAM_SIZE) > MAX_HID_RAM_SIZE) ? MAX_HID_RAM_SIZE : (pageCnt * EEPROM_PAGE_SIZE - i * MAX_HID_RAM_SIZE);
		if (!WriteRam(pBuf, size))
			return FALSE;

		if (!Touch_EnableProgram(addr + i * MAX_HID_RAM_SIZE, size))
			return FALSE;
	}

	return TRUE;
}
BOOL CHIDDevice::Eeprom_PageWrite(PBYTE buff, WORD addr, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == buff)
		return FALSE;

	UINT pageCnt = (len + EEPROM_PAGE_SIZE - 1) / EEPROM_PAGE_SIZE;
	UINT cnt = (pageCnt * EEPROM_PAGE_SIZE + MAX_HID_RAM_SIZE - 1) / MAX_HID_RAM_SIZE;
	PBYTE pBuf = NULL;
	for (UINT i = 0; i < cnt; i++)
	{
		pBuf = &buff[i*MAX_HID_RAM_SIZE];
		WORD size = ((pageCnt * EEPROM_PAGE_SIZE - i * MAX_HID_RAM_SIZE) > MAX_HID_RAM_SIZE) ? MAX_HID_RAM_SIZE : (pageCnt * EEPROM_PAGE_SIZE - i * MAX_HID_RAM_SIZE);
		if (!WriteRam(pBuf, size))
			return FALSE;

		if (!Touch_EnablePageProgram(addr + i * MAX_HID_RAM_SIZE, size))
			return FALSE;
	}

	return TRUE;
}


BOOL CHIDDevice::Touch_EepromAccess(BOOL access)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = { 0 };
	CreateTouchHeader(buf, 0, 3, TRUE, TOUCH_SUB_COMMAND::tscEepromAccess, access ? 0x100 : 0);

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("Touch_EepromAccess failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("Touch_EepromAccess failed:"));
		return FALSE;
	}

	//Sleep(100);
	return TRUE;
}

BOOL CHIDDevice::Touch_WriteCommand(const PBYTE buff, const PBYTE cmd, WORD len)
{
	return TRUE;
}

BOOL CHIDDevice::Touch_ReadCommand(const PBYTE buff, const PBYTE cmd, WORD len)
{
	return TRUE;
}


BOOL CHIDDevice::Touch_EepromEraseAll(BOOL accessOn, BOOL accessOff)
{
	SetTaskPosition(0);
	if (accessOn)
		if (!Touch_EepromAccess(TRUE))
			return FALSE;

	SetTaskPosition(10);

	if (!Eeprom_EraseAll())
		return FALSE;

	SetTaskPosition(90);

	if(accessOff)
		if (!Touch_EepromAccess(FALSE))
			return FALSE;

	SetTaskPosition(100);

	return TRUE;
}

BOOL CHIDDevice::Touch_EepromForceErase()
{
	SetTaskPosition(0);

	if (!Eeprom_ForceErase())
		return FALSE;

	SetTaskPosition(100);

	return TRUE;
}

BOOL CHIDDevice::Touch_EepromErase(WORD addr, WORD len, BOOL accessOn, BOOL accessOff)
{
	if (accessOn)
		if (!Touch_EepromAccess(TRUE))
			return FALSE;

	if (!Touch_Erase(addr, len))
		return FALSE;

	if (accessOff)
		if (!Touch_EepromAccess(FALSE))
			return FALSE;

	return TRUE;
}

BOOL CHIDDevice::Touch_EepromPageProgram(PBYTE buff, WORD addr, WORD len, BOOL accessOn, BOOL accessOff)
{
	if (accessOn)
		if (!Touch_EepromAccess(TRUE))
			return FALSE;

	UINT cnt = (len + MAX_HID_RAM_SIZE - 1) / MAX_HID_RAM_SIZE;
	PBYTE pBuf = NULL;
	for (UINT i = 0; i < cnt; i++)
	{
		pBuf = &buff[i*MAX_HID_RAM_SIZE];
		WORD size = ((len - i * MAX_HID_RAM_SIZE) > MAX_HID_RAM_SIZE) ? MAX_HID_RAM_SIZE : (len - i * MAX_HID_RAM_SIZE);
		if (!WriteRam(pBuf, size))
			return FALSE;

		if (!Touch_EnablePageProgram(addr+i* MAX_HID_RAM_SIZE, size))
			return FALSE;

		//Sleep(100);
	}

	if (accessOff)
		if (!Touch_EepromAccess(FALSE))
			return FALSE;

	return TRUE;
}

BOOL CHIDDevice::Touch_EepromProgram(PBYTE buff, WORD addr, WORD len, BOOL accessOn, BOOL accessOff)
{
	if (accessOn)
		if (!Touch_EepromAccess(TRUE))
			return FALSE;

	if (!Touch_Erase(addr, len))
		return FALSE;

	UINT cnt = (len + MAX_HID_RAM_SIZE - 1) / MAX_HID_RAM_SIZE;
	PBYTE pBuf = NULL;
	for (UINT i = 0; i < cnt; i++)
	{
		pBuf = &buff[i*MAX_HID_RAM_SIZE];
		WORD size = ((len - i * MAX_HID_RAM_SIZE) > MAX_HID_RAM_SIZE) ? MAX_HID_RAM_SIZE : (len - i * MAX_HID_RAM_SIZE);
		if (!WriteRam(pBuf, size))
			return FALSE;

		if (!Touch_EnableProgram(addr + i * MAX_HID_RAM_SIZE, size))
			return FALSE;
	}

	if (accessOff)
		if (!Touch_EepromAccess(FALSE))
			return FALSE;

	return TRUE;
}

BOOL CHIDDevice::Touch_EepromRead(PBYTE buff, WORD addr, WORD len, BOOL accessOn, BOOL accessOff)
{
	if (accessOn)
		if (!Touch_EepromAccess(TRUE))
			return FALSE;

	SetTaskPosition(0);

	UINT cnt = (len + MAX_HID_RAM_SIZE - 1) / MAX_HID_RAM_SIZE;
	for (UINT i = 0; i < cnt; i++)
	{
		SetTaskPosition((i * 100) / cnt);
		PBYTE pBuf = &buff[i*MAX_HID_RAM_SIZE];
		WORD size = ((len - i * MAX_HID_RAM_SIZE) > MAX_HID_RAM_SIZE) ? MAX_HID_RAM_SIZE : (len - i * MAX_HID_RAM_SIZE);
		if (!Eeprom_Read(pBuf, addr + i * MAX_HID_RAM_SIZE, size))
			return FALSE;
	}

	SetTaskPosition(100);

	if (accessOff)
		if (!Touch_EepromAccess(FALSE))
			return FALSE;

	return TRUE;
}

BOOL CHIDDevice::Touch_AdbEepromRead(PBYTE buff, WORD addr, WORD len, UINT timeout)
{
	UINT cnt = (len + MAX_ADB_EEPROM_SIZE - 1) / MAX_ADB_EEPROM_SIZE;
	BYTE startIdx = addr / MAX_ADB_EEPROM_SIZE;
	for (UINT i = 0; i < cnt; i++)
	{
		PBYTE pBuf = &buff[i*MAX_ADB_EEPROM_SIZE];
		WORD size = ((len - i * MAX_ADB_EEPROM_SIZE) > MAX_ADB_EEPROM_SIZE) ? MAX_ADB_EEPROM_SIZE : (len - i * MAX_ADB_EEPROM_SIZE);
		if(!GetAdbData(pBuf, size, ADB_MODE::amGetEeprom, startIdx+i,timeout))
			return FALSE;
	}

	return TRUE;
}

UINT CHIDDevice::ReverseFindZero(PBYTE buff, UINT idx)
{
	while (0 == buff[--idx]);
	return ++idx;
}

BOOL CHIDDevice::Eeprom_MfpUpgrade(PBYTE buff, WORD addr, WORD len)
{
	if (!Touch_EepromAccess(TRUE))
		return FALSE;

	std::unique_ptr<BYTE[]> eepromBuff(new BYTE[EEPROM_DOWNLOAD_SIZE]);
	ZeroMemory(eepromBuff.get(), EEPROM_DOWNLOAD_SIZE);

	memcpy(eepromBuff.get(), buff, len);

	SetTaskPosition(0);

	if (!Eeprom_EraseAll())
		return FALSE;

	// Write Firmware
	UINT fwLen = EEPROM_PAGE_ALIGN(ReverseFindZero(eepromBuff.get(), EEPROM_PARAMETER_ADDR));

	//callback(obj, m_nItem, 60);
	SetTaskPosition(50);

	if (!Eeprom_Write(eepromBuff.get(), 0, fwLen))
		return FALSE;


	// Write Parameter	
	//callback(obj, m_nItem, 60);
	SetTaskPosition(60);

	UINT paramLen = EEPROM_PAGE_ALIGN(ReverseFindZero(eepromBuff.get(), EEPROM_DOWNLOAD_SIZE) - EEPROM_PARAMETER_ADDR);
	if (!Eeprom_Write(eepromBuff.get() + EEPROM_PARAMETER_ADDR, EEPROM_PARAMETER_ADDR, paramLen))
		return FALSE;


	std::unique_ptr<BYTE[]> readBuff(new BYTE[EEPROM_DOWNLOAD_SIZE]);
	ZeroMemory(readBuff.get(), EEPROM_DOWNLOAD_SIZE);

	// Read Firmware
	//callback(obj, m_nItem, 90);
	SetTaskPosition(90);

	if (!Eeprom_Read(readBuff.get(), 0, fwLen))
		return FALSE;

	// Read Parameter
	if (!Eeprom_Read(readBuff.get() + EEPROM_PARAMETER_ADDR, EEPROM_PARAMETER_ADDR, paramLen))
		return FALSE;
	
	TRACE("memcmp start\r\n");
	if (0 != memcmp(eepromBuff.get(), readBuff.get(), EEPROM_DOWNLOAD_SIZE))
		return FALSE;
	TRACE("memcmp end\r\n");

	// User area backup & SN Create
	std::unique_ptr<BYTE[]> userBuff(new BYTE[EEPROM_PAGE_SIZE]);
	ZeroMemory(userBuff.get(), EEPROM_PAGE_SIZE);
	if (!Eeprom_Read(userBuff.get(), EEPROM_TOTAL_SIZE, EEPROM_PAGE_SIZE))
		return FALSE;

	CUserArea ua(userBuff.get());

	if (!ua.IsBackup())
	{
		ua.Backup();
		if (!Eeprom_PageWrite(ua.GetBuffer(), EEPROM_TOTAL_SIZE, ua.GetLength()))
			return FALSE;
	}

	//callback(obj, m_nItem, 100);
	SetTaskPosition(100);

	if (!Touch_EepromAccess(FALSE))
		return FALSE;

	return TRUE;
}

BOOL CHIDDevice::Eeprom_ParamUpgrade(PBYTE buff, WORD addr, WORD len)
{
	if (len != 1024)
		return FALSE;

	if (NULL == buff)
		return FALSE;

	if (0xA800 != addr)
		return FALSE;

	if (!Touch_EepromAccess(TRUE))
		return FALSE;

	if (!WriteRam(buff, len))
		return FALSE;

	if (!Touch_EnablePageProgram(addr, len))
		return FALSE;

	std::unique_ptr<BYTE[]> readBuff(new BYTE[len]);
	if (!Eeprom_Read(readBuff.get(), addr, len))
		return FALSE;

	if (0 != memcmp(readBuff.get(), buff, len))
		return FALSE;

	if (!Touch_EepromAccess(FALSE))
		return FALSE;

	return TRUE;
}


BOOL CHIDDevice::GetBridgeInfo(PBYTE buff, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == buff)
		return FALSE;

	
	BYTE buf[64] = { 0 };
	CreateUsbHeader(buf, 0, 1, TRUE, USB_SUB_COMMAND::uscDeviceInfo, len);

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("GetBridgeInfo failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("GetBridgeInfo failed:"));
		return FALSE;
	}
	//Sleep(100);
	
	if (!ReadData(m_hDisk, buff, len))
	{
		CWarningMessage::Show(_T("GetBridgeInfo Failed"));
		return FALSE;
	}

	return TRUE;

}

BOOL CHIDDevice::ReadTxShortScan(PBYTE buff, UINT len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == buff)
		return FALSE;



	BYTE buf[64] = { 0 };
	CTxShortScan cmd;
	CreateCommandHeader(buf, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, len, cmd.GetBuffer());

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("Read Tx failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("Read Tx failed:"));
		return FALSE;
	}
	//Sleep(100);

	if (!ReadData(m_hDisk, buff, len))
	{
		CWarningMessage::Show(_T("(Read Tx)Get Input Report Failed"));
		return FALSE;
	}

	return TRUE;
}
BOOL CHIDDevice::ReadRxShortScan(PBYTE buff, UINT len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == buff)
		return FALSE;


	BYTE buf[64] = { 0 };
	CRxShortScan cmd;
	CreateCommandHeader(buf, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, len, cmd.GetBuffer());

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("Read Rx failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("(Read Rx Short)Set Output Report Failed"));
		return FALSE;
	}
	//Sleep(100);

	if (!ReadData(m_hDisk, buff, len))
	{
		CWarningMessage::Show(_T("(Read Rx Short)Get Input Report Failed"));
		return FALSE;
	}

	return TRUE;
}
BOOL CHIDDevice::GetResetCheck(BOOL autoCal)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = { 0 };
	CreateTouchHeader(buf, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscResetCheck, autoCal ? 0x00 : 0x100);

	if (!SendCommand(m_hDisk, buf, 64))
	{
		CWarningMessage::Show(_T("GetResetCheck SendCommand Failed"));
		return FALSE;
	}

	BYTE readData[2] = { 0 };
	if (!ReadData(m_hDisk, readData, 2))
	{
		CWarningMessage::Show(_T("GetResetCheck ReadData Failed"));
		return FALSE;
	}

	if (0x3110 == *((PWORD)readData))
		return FALSE;

	return TRUE;
}

BOOL CHIDDevice::SetDeviceReset(BOOL autoCal)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = { 0 };
	CreateTouchHeader(buf, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscResetDevice, autoCal ? 0x00 : 0x100);

	if (!SendCommand(m_hDisk, buf, 64))
	{
		CWarningMessage::Show(_T("SetDeviceReset SendCommand Failed"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::ReadDeviceInfo(PBYTE buff,UINT len, USHORT timeout)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buf[64] = { 0 };
	CReadDeviceInfo cmd;
	CreateCommandHeader(buf, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, len, cmd.GetBuffer(), timeout);

	//if (!HidD_SetOutputReport(m_hDisk, buf, 64))
	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("ReadDeviceInfo failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("ReadDeviceInfo failed"));
		return FALSE;
	}
	//Sleep(100);

	if (!ReadData(m_hDisk, buff, len))
	{
		CWarningMessage::Show(_T("(Read Device Info)Get Input Report Failed"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::GetCapData(PBYTE buff, USHORT rLen, BYTE cmd, BYTE type, UINT timeout )
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (rLen <= 0)return FALSE;

	BYTE buf[64] = { 0 };

	CSetAdbMode modecmd;
	modecmd.SetCmd(cmd);
	modecmd.SetMode(type);

	CreateCommandHeader(buf, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, rLen, modecmd.GetBuffer(), timeout);

	if (!SendCommand(m_hDisk, buf, 64))
	{
		CWarningMessage::Show(_T("ADB_Modle failed:"));
		return FALSE;
	}

	if (!ReadData(m_hDisk, buff, rLen))
	{
		CWarningMessage::Show(_T("(Get Raw Cap)Get Input Report Failed"));
		return FALSE;
	}

	return TRUE;
}
BOOL CHIDDevice::GetCapAverage(PBYTE buff, USHORT rLen, BYTE capNum, UINT timeout)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (rLen <= 0)return FALSE;

	BYTE buf[64] = { 0 };

	CGetCapAverage CapAverage(capNum);
	CreateCommandHeader(buf, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, rLen, CapAverage.GetBuffer(), timeout);

	if (!SendCommand(m_hDisk, buf, 64))
	{
		CWarningMessage::Show(_T("Get CapAverage failed:\r\n"));
		return FALSE;
	}

	if (!ReadData(m_hDisk, buff, rLen))
	{
		CWarningMessage::Show(_T("Get CapAverage Failed!\r\n"));
		return FALSE;
	}

	return TRUE;
}
BOOL CHIDDevice::GetAdbData(PBYTE buff, USHORT rLen, ADB_MODE mode, BYTE param, UINT timeout)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;
	if (rLen <= 0)return FALSE;

	BYTE cmd[64] = { 0 };

	CSetAdbMode adbMode(mode, param);
	CreateCommandHeader(cmd, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, rLen, adbMode.GetBuffer(), timeout);

	if (!SendReadCommand(cmd, buff, rLen))
	{
		CWarningMessage::Show(_T("GetAdbData Failed"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::SetSerialOutput()
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE cmd[64] = { 0 };
	CSetSerialOutput serial;
	CreateCommandHeader(cmd, 0, 17, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, 0, serial.GetBuffer());

	if (!SendReadCommand(cmd))
	{
		CWarningMessage::Show(_T("SetSerialOutput Failed"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::SendCmd_Test(PBYTE buff, UINT len,BOOL read)
{
	if(INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (buff == NULL)return FALSE;
	//if (!HidD_SetOutputReport(m_hDisk, buff, len))
	if (!SendCommand(m_hDisk, buff, 64))
	{
		CWarningMessage::Show(_T("Send Cmd_Test failed!"));
		return FALSE;
	}
	if (read && len > 0)
	{
		if (!ReadData(m_hDisk, buff, len))
		{
			CWarningMessage::Show(_T("(Get Raw Cap)Get Input Report Failed"));
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CHIDDevice::LDOAdjust(const PBYTE buff, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (len <= 0)return FALSE;

	BYTE buf[64] = { 0 };

	CLDOAdjust LDOAdjust;
	CreateCommandHeader(buf, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, len, LDOAdjust.GetBuffer());

	if (!SendCommand(m_hDisk, buf, 64))
	{
		//CString str;
		//str.Format(_T("LDOAdjust failed:%s"), LastErrorCodeToString(GetLastError()));
		//AfxMessageBox(str);
		CWarningMessage::Show(_T("LDOAdjust failed:"));
		return FALSE;
	}

	if (!ReadData(m_hDisk, buff, len))
	{
		CWarningMessage::Show(_T("LDOAdjust ReadData Failed!"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::AutoCalibration(BYTE mode, const PBYTE buff, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE cmd[64] = { 0 };
	CAutoCalibration autoCalibration(mode);
	CreateCommandHeader(cmd, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, len, autoCalibration.GetBuffer());

	if (!SendReadCommand(cmd, buff, len))
	{
		CWarningMessage::Show(_T("AutoCalibration Failed"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::SendReadCommand(PBYTE cmd, PBYTE buff, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (!SendCommand(m_hDisk, cmd, 64))
	{
		TRACE(_T("Send Command Failed!\r\n"));
        return FALSE;
	}

	if ((NULL != buff) && (0 != len))
	{
		if (!ReadData(m_hDisk, buff, len))
		{
			TRACE(_T("ReadData Failed!\r\n"));
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CHIDDevice::SetUSBMode()
{
	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	BYTE buff[64] = { 0 };

	CUSBMode usbMode;
	CreateCommandHeader(buff, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscReadCommand, 64, usbMode.GetBuffer(), 300);

	if (!SendCommand(m_hDisk, buff, 64))
	{
		TRACE(_T("Send Command Failed!\r\n"));
		return FALSE;
	}

	ZeroMemory(buff, 64);

	if (!ReadData(m_hDisk, buff, 32))
	{
		TRACE(_T("ReadData Failed!\r\n"));
		return FALSE;
	}

	buff[0] = 0x01;

	if (!WriteRam(buff, 32))
	{
		TRACE(_T("WriteData Failed!\r\n"));
		return FALSE;
	}

	ZeroMemory(buff, 64);
	usbMode.SetCmd(0xA6);
	CreateCommandHeader(buff, 0, 11, TRUE, TOUCH_SUB_COMMAND::tscWriteCommand, 64, usbMode.GetBuffer(), 300);

	if (!SendCommand(m_hDisk, buff, 64))
	{
		TRACE(_T("Send Command Failed!\r\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL CHIDDevice::Touch_HexUpgrade(PBYTE pBuff, WORD len)
{
	if (INVALID_HANDLE_VALUE == m_hDisk || pBuff==NULL || len<=0)
		return FALSE;

	SetTaskPosition(10);

	if (!Touch_EepromAccess(TRUE))
		return FALSE;

	SetTaskPosition(20);

	if (!Touch_EepromErase(0, len, FALSE, FALSE))
		return FALSE;

	//callback(obj, m_nItem, 60);
	SetTaskPosition(30);

	UINT fwLen = EEPROM_PAGE_ALIGN(ReverseFindZero(pBuff, EEPROM_PARAMETER_ADDR));

	if (!Eeprom_Write(pBuff, 0, fwLen))
		return FALSE;

	SetTaskPosition(50);

	std::unique_ptr<BYTE[]> readBuff(new BYTE[EEPROM_DOWNLOAD_SIZE]);
	ZeroMemory(readBuff.get(), EEPROM_DOWNLOAD_SIZE);

	if (!Eeprom_Read(readBuff.get(), 0, len))
		return FALSE;

	SetTaskPosition(60);

	if (0 != memcmp(pBuff, readBuff.get(), len))
		return FALSE;

	SetTaskPosition(80);

	// User area backup & SN Create
	std::unique_ptr<BYTE[]> userBuff(new BYTE[EEPROM_PAGE_SIZE]);
	ZeroMemory(userBuff.get(), EEPROM_PAGE_SIZE);
	if (!Eeprom_Read(userBuff.get(), EEPROM_TOTAL_SIZE, EEPROM_PAGE_SIZE))
		return FALSE;

	SetTaskPosition(90);

	CUserArea ua(userBuff.get());

	if (!ua.IsBackup())
	{
		ua.Backup();
		if (!Eeprom_PageWrite(ua.GetBuffer(), EEPROM_TOTAL_SIZE, ua.GetLength()))
			return FALSE;
	}

	SetTaskPosition(100);

	if (!Touch_EepromAccess(FALSE))
		return FALSE;

	return TRUE;
}
