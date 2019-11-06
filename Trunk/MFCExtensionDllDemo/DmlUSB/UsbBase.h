#pragma once
class CUsbBase
{
public:
	CUsbBase();
	virtual ~CUsbBase();

public:

	virtual BOOL OpenDevice(LPCTSTR path) = 0;
	virtual void CloseDevice() = 0;

	virtual BOOL SetPowerStatus(PBYTE buff, WORD len) { return TRUE; }
	virtual BOOL GetPowerStatus(PBYTE buff, WORD len) { return TRUE; }

	virtual BOOL GetBridgeInformation(PBYTE buff, WORD len) = 0;
	virtual BOOL GetDeviceInformation(PBYTE buff, WORD len) = 0;

	virtual BOOL GetAdbData(WORD mode, PBYTE buff, WORD len) = 0;
	virtual BOOL SetDeviceGroup(WORD grp) { return TRUE; }

	virtual BOOL SetOperationMode(BYTE major, BYTE minor) = 0;
	virtual BOOL SetDeviceReset() = 0;

	virtual BOOL EepromAccess(BOOL bEnter) = 0;
	virtual BOOL EepromEraseAll() = 0;
	virtual BOOL EepromRead(WORD addr, PBYTE buff, WORD len) = 0;
	virtual BOOL EepromWrite(WORD addr, PBYTE buff, WORD len) = 0;

	virtual BOOL I2CPortMonitor(WORD addr, WORD len) = 0;
	virtual BOOL SPIPortMonitor(BOOL bEnable) = 0;

	virtual BOOL FindI2CAddress() { return TRUE; }
	virtual BOOL BridgeReset() = 0;

	virtual BOOL GetDeviceID(PBYTE buff, WORD len) = 0;
	virtual BOOL GetLdoAdjust(WORD refVal, PBYTE buff, WORD len) = 0;

	virtual BOOL CheckResetIO() = 0;
	virtual BOOL GetRCAdjust(PBYTE buff, WORD len) = 0;
	virtual BOOL GetTxShort(BYTE delay, BYTE time, PBYTE buff, WORD len) = 0;
	virtual BOOL GetRxShort(BYTE time, PBYTE buff, WORD len) = 0;
	virtual BOOL GetRxAdjust(PBYTE buff, WORD len) = 0;
	virtual BOOL CheckIntIO(PBYTE buff, WORD len) = 0;
	virtual BOOL SetHostTimeout(WORD val) = 0;
	virtual BOOL SetInfoMonitor(WORD mode) = 0;
	virtual BOOL ForceErase() = 0;

	virtual BOOL ReadBulkPipe( IN  PUCHAR  Buffer, IN  ULONG   bufSize) = 0;
};

