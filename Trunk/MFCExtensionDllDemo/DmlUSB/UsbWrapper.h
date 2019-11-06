#pragma once

#include "UsbBase.h"

class AFX_EXT_CLASS CUsbWrapper
{
public:
	CUsbWrapper();
	virtual ~CUsbWrapper();

public:
	enum USB_TYPE
	{
		UNKNOWN = 0,
		HOST_BOARD = 1,
		USB_DISK = 2,
		USB_HID = 3
	};

private:
	CUsbBase* m_pUsbBase;

public:
	BOOL CreateDevice(USB_TYPE typ, const CString& name);

	BOOL OpenDevice(LPCTSTR path);
	void CloseDevice();

	BOOL SetPowerStatus(PBYTE buff, WORD len);
	BOOL GetPowerStatus(PBYTE buff, WORD len);

	BOOL GetBridgeInformation(PBYTE buff, WORD len);
	BOOL GetDeviceInformation(PBYTE buff, WORD len);

	BOOL GetAdbData(WORD mode, PBYTE buff, WORD len);
	BOOL SetDeviceGroup(WORD grp);

	BOOL SetOperationMode(BYTE major, BYTE minor);
	BOOL SetDeviceReset();

	BOOL EepromAccess(BOOL bEnter);
	BOOL EepromEraseAll();
	BOOL EepromRead(WORD addr, PBYTE buff, WORD len);
	BOOL EepromWrite(WORD addr, PBYTE buff, WORD len);

	BOOL I2CPortMonitor(WORD addr, WORD len);
	BOOL SPIPortMonitor(BOOL bEnable);

	BOOL FindI2CAddress();
	BOOL BridgeReset();

	BOOL GetDeviceID(PBYTE buff, WORD len);
	BOOL GetLdoAdjust(WORD refVal, PBYTE buff, WORD len);

	BOOL CheckResetIO();
	BOOL GetRCAdjust(PBYTE buff, WORD len);
	BOOL GetTxShort(BYTE delay, BYTE time, PBYTE buff, WORD len);
	BOOL GetRxShort(BYTE time, PBYTE buff, WORD len);
	BOOL GetRxAdjust(PBYTE buff, WORD len);
	BOOL CheckIntIO(PBYTE buff, WORD len);
	BOOL SetHostTimeout(WORD val);
	BOOL SetInfoMonitor(WORD mode);
	BOOL ForceErase();
	BOOL ReadData(PBYTE buff, WORD len);
};

