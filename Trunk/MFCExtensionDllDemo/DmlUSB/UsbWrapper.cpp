#include "stdafx.h"
#include "UsbWrapper.h"


CUsbWrapper::CUsbWrapper()
{
	m_pUsbBase = NULL;
}


CUsbWrapper::~CUsbWrapper()
{
	if (NULL != m_pUsbBase)
		delete m_pUsbBase;
	m_pUsbBase = NULL;
}

BOOL CUsbWrapper::CreateDevice(USB_TYPE typ, const CString& name)
{
	switch (typ)
	{
	case HOST_BOARD:
		m_pUsbBase = new CUsbWinUSB();
		break;
	default:
		break;
	}

	return (NULL != m_pUsbBase);
}


BOOL CUsbWrapper::OpenDevice(LPCTSTR path)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->OpenDevice(path);
}

void CUsbWrapper::CloseDevice()
{
	if (NULL == m_pUsbBase)
		return;

	m_pUsbBase->CloseDevice();
}

BOOL CUsbWrapper::SetPowerStatus(PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->SetPowerStatus(buff, len);
}

BOOL CUsbWrapper::GetPowerStatus(PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetPowerStatus(buff, len);
}

BOOL CUsbWrapper::GetBridgeInformation(PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetBridgeInformation(buff, len);
}

BOOL CUsbWrapper::GetDeviceInformation(PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetDeviceInformation(buff, len);
}

BOOL CUsbWrapper::GetAdbData(WORD mode, PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetAdbData(mode, buff, len);
}

BOOL CUsbWrapper::SetDeviceGroup(WORD grp)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->SetDeviceGroup(grp);
}

BOOL CUsbWrapper::SetOperationMode(BYTE major, BYTE minor)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->SetOperationMode(major, minor);
}

BOOL CUsbWrapper::SetDeviceReset()
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->SetDeviceReset();
}

BOOL CUsbWrapper::EepromAccess(BOOL bEnter)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->EepromAccess(bEnter);
}

BOOL CUsbWrapper::EepromEraseAll()
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->EepromEraseAll();
}

BOOL CUsbWrapper::EepromRead(WORD addr, PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->EepromRead(addr, buff, len);
}

BOOL CUsbWrapper::EepromWrite(WORD addr, PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->EepromWrite(addr, buff, len);
}

BOOL CUsbWrapper::I2CPortMonitor(WORD addr, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->I2CPortMonitor(addr, len);
}

BOOL CUsbWrapper::SPIPortMonitor(BOOL bEnable)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->SPIPortMonitor(bEnable);
}

BOOL CUsbWrapper::FindI2CAddress()
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->FindI2CAddress();
}

BOOL CUsbWrapper::BridgeReset()
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->BridgeReset();
}

BOOL CUsbWrapper::GetDeviceID(PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetDeviceID(buff, len);
}

BOOL CUsbWrapper::GetLdoAdjust(WORD refVal, PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetLdoAdjust(refVal, buff, len);
}

BOOL CUsbWrapper::CheckResetIO()
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->CheckResetIO();
}

BOOL CUsbWrapper::GetRCAdjust(PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetRCAdjust(buff, len);
}

BOOL CUsbWrapper::GetTxShort(BYTE delay, BYTE time, PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetTxShort(delay, time, buff, len);
}

BOOL CUsbWrapper::GetRxShort(BYTE time, PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetRxShort(time, buff, len);
}

BOOL CUsbWrapper::GetRxAdjust(PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->GetRxAdjust(buff, len);
}

BOOL CUsbWrapper::CheckIntIO(PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->CheckIntIO(buff, len);
}

BOOL CUsbWrapper::SetHostTimeout(WORD val)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->SetHostTimeout(val);
}

BOOL CUsbWrapper::SetInfoMonitor(WORD mode)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->SetInfoMonitor(mode);
}

BOOL CUsbWrapper::ForceErase()
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->ForceErase();
}

BOOL CUsbWrapper::ReadData(PBYTE buff, WORD len)
{
	if (NULL == m_pUsbBase)
		return FALSE;

	return m_pUsbBase->ReadBulkPipe(buff, len);
}