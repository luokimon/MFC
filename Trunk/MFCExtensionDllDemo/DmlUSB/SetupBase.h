#pragma once
class CSetupBase
{
public:
	CSetupBase();
	virtual ~CSetupBase();

	WINUSB_SETUP_PACKET pack;
	PBYTE m_pBuff;

protected:
	enum
	{
		TYPE_WRITE = 0x40, 
		TYPE_READ = 0xC0,
	};

	enum
	{
		REQUEST_GetAdbData					= 0x05,
		REQUEST_GetBridgeInformation		= 0xA0,
		//REQUEST_GetErrorStatus			= 0xA1,
		REQUEST_SetDeviceGroup				= 0xA2,
		REQUEST_SetOperationMode			= 0xA3,
		REQUEST_DeviceReset					= 0xA4,
		REQUEST_EepromReadWrite				= 0xA5,
		REQUEST_EepromAccess				= 0xA6,
		REQUEST_EepromEraseAll				= 0xA7,
		REQUEST_I2CPortMonitor				= 0xA8,
		REQUEST_SPIPortMonitor				= 0xA9,
		REQUEST_GetDeviceInformation		= 0xAA,
		REQUEST_FindI2CAddress				= 0xAB,
		REQUEST_BridgeReset					= 0xAC,
		//REQUEST_INTPortMonitor			= 0xAD,
		REQUEST_GetDeviceID					= 0xAE,
		REQUEST_GetLdoAdjust				= 0xAF,
		REQUEST_SetPowerStatus				= 0xB0,
		REQUEST_GetPowerStatus				= 0xB1,
		REQUEST_CheckResetIO				= 0xB2,
		//REQUEST_SetStart					= 0xB6,
		REQUEST_GetRCAdjust					= 0xB8,
		REQUEST_GetTxShort					= 0xB9,
		REQUEST_GetRxShort					= 0xBA,
		REQUEST_GetRxAdjust					= 0xBB,
		//REQUEST_GetSelfRxAdjust			= 0xBC,
		//REQUEST_GetSelfTxAdjust			= 0xBD,
		REQUEST_CheckIntIO					= 0xBE,
		//REQUEST_GetInterCapAdjust			= 0xBF,
		REQUEST_SetHostTimeout				= 0xC0,
		REQUEST_SetInfoMonitor				= 0xC1,
		REQUEST_ForceErase					= 0xC2,
		//REQUEST_SetI2CAddress				= 0xC3,
		//REQUEST_SetRegisterValue			= 0xC4,
	};
public:
	void SetRequestType(BYTE typ) { pack.RequestType = typ; }
	void SetRequest(BYTE req) { pack.Request = req; }
	void SetValue(WORD val) { pack.Value = val; }
	void SetIndex(WORD idx) { pack.Index = idx; }
	void SetLength(UINT len);

	WINUSB_SETUP_PACKET GetSetupPacket() { return pack; }
	void SetBuffer(PBYTE buff) { memcpy(m_pBuff, buff, pack.Length); }
	PBYTE GetBuffer() { return m_pBuff; }
	WORD GetLength() { return pack.Length; }
};


//REQUEST_GetAdbData = 0x05,
class CSetupGetAdbData : public CSetupBase
{
public:
	CSetupGetAdbData(WORD mode, WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetAdbData);
		SetValue(mode);
		SetLength(len);
	}
	virtual ~CSetupGetAdbData() {}
};


//REQUEST_GetBridgeInformation = 0xA0,
class CSetupGetBridgeInformation : public CSetupBase
{
public:
	CSetupGetBridgeInformation(WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetBridgeInformation);

		SetLength(len);
	}
	virtual ~CSetupGetBridgeInformation(){}
};


//REQUEST_SetDeviceGroup = 0xA2,
class CSetupSetDeviceGroup : public CSetupBase
{
public:
	CSetupSetDeviceGroup(WORD grp)
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_SetDeviceGroup);

		SetValue(grp);
	}
	virtual ~CSetupSetDeviceGroup() {}
};


//REQUEST_SetOperationMode = 0xA3,
class CSetupSetOperationMode : public CSetupBase
{
public:
	CSetupSetOperationMode(BYTE major, BYTE minor)
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_SetOperationMode);

		SetValue((USHORT)((major << 8) | (minor)));
	}
	virtual ~CSetupSetOperationMode() {}
};


//REQUEST_DeviceReset = 0xA4,
class CSetupSetDeviceReset : public CSetupBase
{
public:
	CSetupSetDeviceReset()
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_DeviceReset);

	}
	virtual ~CSetupSetDeviceReset() {}
};


//REQUEST_EepromReadWrite = 0xA5,
class CSetupEepromReadWrite : public CSetupBase
{
public:
	CSetupEepromReadWrite(BOOL bRead, WORD addr, WORD len)
	{
		SetRequestType(bRead ? CSetupBase::TYPE_READ : CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_EepromReadWrite);
		SetValue(addr);
		SetLength(len);
	}
	virtual ~CSetupEepromReadWrite() {}
};


//REQUEST_EepromAccess = 0xA6,
class CSetupEepromAccess : public CSetupBase
{
public:
	CSetupEepromAccess(BOOL bEnter)
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_EepromAccess);
		SetValue(bEnter ? 0x01 : 0x00);
	}
	virtual ~CSetupEepromAccess() {}
};


//REQUEST_EepromEraseAll = 0xA7,
class CSetupEepromEraseAll : public CSetupBase
{
public:
	CSetupEepromEraseAll()
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_EepromEraseAll);
	}
	virtual ~CSetupEepromEraseAll() {}
};


//REQUEST_I2CPortMonitor = 0xA8,
class CSetupI2CPortMonitor : public CSetupBase
{
public:
	CSetupI2CPortMonitor(WORD addr, WORD len)
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_I2CPortMonitor);
		SetValue(addr);
		SetIndex(len);
	}
	virtual ~CSetupI2CPortMonitor() {}
};


//REQUEST_SPIPortMonitor = 0xA9,
class CSetupSPIPortMonitor : public CSetupBase
{
public:
	CSetupSPIPortMonitor(BOOL bEnable)
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_SPIPortMonitor);
		SetValue(bEnable ? 0x01 : 0x00);
	}
	virtual ~CSetupSPIPortMonitor() {}
};


//REQUEST_GetDeviceInformation = 0xAA,
class CSetupGetDeviceInformation : public CSetupBase
{
public:
	CSetupGetDeviceInformation(WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetDeviceInformation);
		SetLength(len);
	}
	virtual ~CSetupGetDeviceInformation() {}
};


//REQUEST_FindI2CAddress = 0xAB,
class CSetupFindI2CAddress : public CSetupBase
{
public:
	CSetupFindI2CAddress()
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_FindI2CAddress);
	}
	virtual ~CSetupFindI2CAddress() {}
};


//REQUEST_BridgeReset = 0xAC,
class CSetupBridgeReset : public CSetupBase
{
public:
	CSetupBridgeReset()
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_BridgeReset);
	}
	virtual ~CSetupBridgeReset() {}
};


//REQUEST_GetDeviceID = 0xAE,
class CSetupGetDeviceID : public CSetupBase
{
public:
	CSetupGetDeviceID(WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetDeviceID);
		SetLength(len);
	}
	virtual ~CSetupGetDeviceID() {}
};


//REQUEST_GetLdoAdjust = 0xAF,
class CSetupGetLdoAdjust : public CSetupBase
{
public:
	CSetupGetLdoAdjust(WORD val, WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetLdoAdjust);
		SetValue(val);
		SetLength(len);
	}
	virtual ~CSetupGetLdoAdjust() {}
};


//REQUEST_SetPowerStatus = 0xB0,
class CSetupSetPowerStatus : public CSetupBase
{
public:
	CSetupSetPowerStatus(PBYTE buff, WORD len)
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_SetPowerStatus);
		SetLength(len);
		SetBuffer(buff);
	}
	virtual ~CSetupSetPowerStatus() {}
};


//REQUEST_GetPowerStatus = 0xB1,
class CSetupGetPowerStatus : public CSetupBase
{
public:
	CSetupGetPowerStatus(WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetPowerStatus);
		SetLength(len);
	}
	virtual ~CSetupGetPowerStatus() {}
};


//REQUEST_CheckResetIO = 0xB2,
class CSetupCheckResetIO : public CSetupBase
{
public:
	CSetupCheckResetIO()
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_CheckResetIO);
	}
	virtual ~CSetupCheckResetIO() {}
};


//REQUEST_GetRCAdjust = 0xB8,
class CSetupGetRCAdjust : public CSetupBase
{
public:
	CSetupGetRCAdjust(WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetRCAdjust);
		SetLength(len);
	}
	virtual ~CSetupGetRCAdjust() {}
};


//REQUEST_GetTxShort = 0xB9,
class CSetupGetTxShort : public CSetupBase
{
public:
	CSetupGetTxShort(BYTE delay, BYTE time, WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetTxShort);
		SetValue((USHORT)((delay << 8) | (time)));
		SetLength(len);
	}
	virtual ~CSetupGetTxShort() {}
};


//REQUEST_GetRxShort = 0xBA,
class CSetupGetRxShort : public CSetupBase
{
public:
	CSetupGetRxShort(BYTE time, WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetRxShort);
		SetValue(time);
		SetLength(len);
	}
	virtual ~CSetupGetRxShort() {}
};


//REQUEST_GetRxAdjust = 0xBB,
class CSetupGetRxAdjust : public CSetupBase
{
public:
	CSetupGetRxAdjust(WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_GetRxAdjust);
		SetLength(len);
	}
	virtual ~CSetupGetRxAdjust() {}
};


//REQUEST_CheckIntIO = 0xBE,
class CSetupCheckIntIO : public CSetupBase
{
public:
	CSetupCheckIntIO(WORD len)
	{
		SetRequestType(CSetupBase::TYPE_READ);
		SetRequest(CSetupBase::REQUEST_CheckIntIO);
		SetLength(len);
	}
	virtual ~CSetupCheckIntIO() {}
};


//REQUEST_SetHostTimeout = 0xC0,
class CSetupSetHostTimeout : public CSetupBase
{
public:
	CSetupSetHostTimeout(WORD val)
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_SetHostTimeout);
		SetValue(val);
	}
	virtual ~CSetupSetHostTimeout() {}
};


//REQUEST_SetInfoMonitor = 0xC1,
class CSetupSetInfoMonitor : public CSetupBase
{
public:
	CSetupSetInfoMonitor(WORD mode)
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_SetInfoMonitor);
		SetValue(mode);
	}
	virtual ~CSetupSetInfoMonitor() {}
};


//REQUEST_ForceErase = 0xC2,
class CSetupForceErase : public CSetupBase
{
public:
	CSetupForceErase()
	{
		SetRequestType(CSetupBase::TYPE_WRITE);
		SetRequest(CSetupBase::REQUEST_ForceErase);
	}
	virtual ~CSetupForceErase() {}
};