#pragma once

#ifdef  _DLL_LIB_EXPORT_
#define  DLL_EXPORT   __declspec(dllexport)
#else
#define  DLL_EXPORT   __declspec(dllimport)
#endif

#define MAX_HID_RAM_SIZE		4096
#define MAX_SPI_ROM_SIZE		1024
#define MAX_ADB_EEPROM_SIZE		1024
#define MAX_PACKAGE_DATA_SIZE	58
#define MAX_HID_PACKAGE_SIZE	64

#define EEPROM_PAGE_SIZE		64
#define EEPROM_TOTAL_SIZE		(48*1024)
#define EEPROM_DOWNLOAD_SIZE	0xAC00
#define EEPROM_PARAMETER_ADDR	0xA800

#define EEPROM_PAGE_ALIGN(X)	(((X+EEPROM_PAGE_SIZE-1)/EEPROM_PAGE_SIZE)*EEPROM_PAGE_SIZE)

#define COMMAND_RETRY_COUNT		5
#define READ_DATA_RETRY_COUNT	5

/*
enum HID_WRITE_TYPE
{
	hwtCommand = 0x00,
	hwtTouch = 0x01,
	hwtUsb = 0x02
};

*/
enum HID_COMMAND_TYPE
{
	hctDefault = 0x00,
	hctMemory = 0x01,
	hctSpi = 0x02,
	hctProgram = 0x10,
	hctEraseAll = 0x11,
	hctWriteCmd = 0x30,
	hctReadCmd = 0x31
};

enum HID_MAIN_COMMAND
{
	hmcNormal = 0x00,
	hmcTouch = 0x01,
	hmcUsb = 0x02
};

enum USB_SUB_COMMAND
{
	uscWriteData		= 0x00,
	uscEraseAll			= 0x10,
	uscStartSpiProgram	= 0x12,
	uscReadRam			= 0x20,
	uscReadSpi			= 0x21,
	uscDeviceReset		= 0x30,
	uscDeviceInfo		= 0x40,
	uscSetUpdateMode	= 0x50,
	uscGetDebugLog		= 0x60
};
/*
enum TOUCH_SUB_COMMAND
{
	tscWriteData		= 0x00,
	tscEraseAll			= 0x10,
	tscErase			= 0x11,
	tscStartProgram		= 0x12,
	tscStartPageProgram = 0x13,
	tscReadEeprom		= 0x14,
	tscEepromAccess		= 0x15,
	tscWriteCommand		= 0x30,
	tscReadCommand		= 0x31,
	tscReadRowCap       = 0x40
};*/


#define MAX_HW_TX_CH_NUM 28
#define MAX_HW_RX_CH_NUM 16
#define U08 BYTE  
#define U16 USHORT
#define U32 UINT

#pragma pack(push)
#pragma pack(1)
struct _REPLY_DEVINFO_OBJECT
{
	U08 DataType;			//Reserved
	U08 TotalByte;			//???? ?? Size??...
	U08 FwStat;
	U08 ModelName[16];
	U08 FwMajorVersion;
	U08 FwMinorVersion;
	U16 DevId;
	U08 ProtocolVersion;
	U08 TpmType;
	U08 TxChCount;
	U08 RxChCount;//27
	U08 TxChOrder[MAX_HW_TX_CH_NUM];//55
	U08 RxChOrder[MAX_HW_RX_CH_NUM];//71
	U08 CapNodeDataSize;
	U08 CapNodeCount;
	U08 HstNodeDataSize;
	U08 HstNodeCount;
	U08 SolutionType;
	U16 XResol;
	U16 YResol;
	U08 ReportType;//81
	U32 FwChecksum;
	U16 ParameterChecksum;
	U16 ModelInfoChecksum;

	U08 I2CAddress;//90
	U16 MP_INFO_IC_ID;
	U08 MP_INFO_IC_Rev;
	U16 MP_INFO_FPCB_Ver;
	U16 MP_INFO_SENSOR_Ver;
	U16 MP_INFO_TPM_Ver;

	U16 IC_ID;
	U16 IC_Rev;
	U16 TPM_Version;
	U16 Platform_Version;
	U08 UseFrameNumAndDivVal;
};
#pragma pack(pop)

enum ADB_MODE;



class DLL_EXPORT CHIDDevice :
	public CDeviceBase
{
public:
	CHIDDevice();
	CHIDDevice(const CString& name);
	virtual ~CHIDDevice();
private:
	BOOL SendCommand(HANDLE handle, PBYTE buff, ULONG len);
	BOOL ReadData(HANDLE handle, PBYTE buff, ULONG len);

	BOOL WriteRam(PBYTE buff, USHORT len);
	BOOL ReadRam(PBYTE buff, USHORT len);
	BOOL EraseAll();
	BOOL HIDReset();
	BOOL EnableProgram(USHORT len, USHORT offset);
	BOOL ReadSpi(PBYTE buff, USHORT len, USHORT offset);

	//BOOL ReadData(PBYTE buff, USHORT len);

	BOOL Eeprom_EraseAll();
	BOOL Eeprom_ForceErase();
	BOOL Touch_Erase(WORD addr, WORD len);
	BOOL Touch_EnableProgram(WORD addr, WORD len);
	BOOL Touch_EnablePageProgram(WORD addr, WORD len);
	BOOL Eeprom_Read(PBYTE buff, WORD addr, WORD len);
	BOOL Eeprom_Write(PBYTE buff, WORD addr, WORD len);
	BOOL Eeprom_PageWrite(PBYTE buff, WORD addr, WORD len);

	UINT ReverseFindZero(PBYTE buff, UINT idx);
	
	BOOL Touch_WriteCommand(const PBYTE buff, const PBYTE cmd, WORD len);
	BOOL Touch_ReadCommand(const PBYTE buff, const PBYTE cmd, WORD len);

	void CreateHidHeader(PBYTE buff,
		HID_WRITE_TYPE hwt,
		USHORT sn,
		BYTE packageLen,
		BOOL lastData = FALSE,
		HID_COMMAND_TYPE hct = HID_COMMAND_TYPE::hctDefault,
		USHORT totalLen = 0,
		USHORT offset = 0);
	void Create3118Header(PBYTE buff,
		HID_WRITE_TYPE hwt,
		USHORT sn,
		BYTE packageLen,
		BOOL lastData = FALSE,
		TOUCH_SUB_COMMAND hct = TOUCH_SUB_COMMAND::tscReadCommand,
		USHORT totalLen = 0,
		PBYTE CMD = NULL);

	void CreateTouchUpgradeHeader(PBYTE buff,
		USHORT sn,
		BYTE packageLen,
		BOOL lastData);

	void CreateUsbUpgradeHeader(PBYTE buff,
		USHORT sn,
		BYTE packageLen,
		BOOL lastData);

	void CreateCommandHeader(PBYTE buff,
		USHORT sn,
		BYTE packageLen,
		BOOL lastData,
		BYTE hct = 0,
		USHORT totalLen = 0,
		WORD offset = 0);

	void CreateUsbHeader(PBYTE buff,
		USHORT sn,
		BYTE packageLen,
		BOOL lastData,
		USB_SUB_COMMAND usc = USB_SUB_COMMAND::uscWriteData,
		USHORT totalLen = 0,
		WORD offset = 0);

	void CreateTouchHeader(PBYTE buff,
		USHORT sn,
		BYTE packageLen,
		BOOL lastData,
		TOUCH_SUB_COMMAND tsc = TOUCH_SUB_COMMAND::tscUnknown,
		USHORT totalLen = 0,
		WORD offset = 0);

	void CHIDDevice::CreateCommandHeader(PBYTE buff,
		USHORT sn,
		BYTE packageLen,
		BOOL lastData,
		TOUCH_SUB_COMMAND tsc,
		USHORT totalLen,
		PBYTE cmd,
		USHORT timeout = 3000
	);

	_REPLY_DEVINFO_OBJECT m_devInfo_Object;

public:
	BOOL OpenDeviceHandle();
	BOOL DeviceReset(PBYTE buff = NULL, DWORD len = 0);
	BOOL FirmwareUpgrade(PBYTE buff, DWORD len);
	BOOL SPI_EraseAll();
	BOOL SPI_Read(PBYTE buff, DWORD len);
	BOOL IsTouchScreen() { return TRUE; }

	BOOL GetBridgeInfo(PBYTE buff, WORD len);
	BOOL Touch_EepromAccess(BOOL access);
	BOOL Touch_EepromEraseAll(BOOL accessOn = TRUE, BOOL accessOff = TRUE);
	BOOL Touch_EepromForceErase();
	BOOL Touch_EepromErase(WORD addr, WORD len, BOOL accessOn = TRUE, BOOL accessOff = TRUE);
	BOOL Touch_EepromRead(PBYTE buff, WORD addr, WORD len, BOOL accessOn = TRUE, BOOL accessOff = TRUE);
	BOOL Touch_AdbEepromRead(PBYTE buff, WORD addr, WORD len, UINT timeout=100);
	BOOL Touch_EepromPageProgram(PBYTE buff, WORD addr, WORD len, BOOL accessOn = TRUE, BOOL accessOff = TRUE);
	BOOL Touch_EepromProgram(PBYTE buff, WORD addr, WORD len, BOOL accessOn = TRUE, BOOL accessOff = TRUE);
	BOOL Eeprom_MfpUpgrade(PBYTE buff, WORD addr, WORD len);
	BOOL Eeprom_ParamUpgrade(PBYTE buff, WORD addr, WORD len);

	BOOL RCAdjust(const PBYTE buff, WORD len);

	BOOL ReadTxShortScan(PBYTE buff,UINT len);
	BOOL ReadRxShortScan(PBYTE buff, UINT len);
	BOOL GetResetCheck(BOOL autoCal);
	BOOL SetDeviceReset(BOOL autoCal);
	BOOL ReadDeviceInfo(PBYTE buff, UINT len, USHORT timeout = 500);
	BOOL GetCapData(PBYTE buff, USHORT rLen, BYTE cmd, BYTE type, UINT timeout = 100);
	BOOL GetAdbData(PBYTE buff, USHORT rLen, ADB_MODE mode, BYTE param, UINT timeout = 100);
	BOOL SetSerialOutput();
	BOOL SetUpdateMode(BOOL mode);
	BOOL GetDebugLog(PBYTE buff, UINT len);
	BOOL SetDebugLog(BOOL mode,BYTE type=0);

	WORD GetTotalEepromSize() { return EEPROM_TOTAL_SIZE; }
	BOOL SendCmd_Test(PBYTE buff, UINT len, BOOL read);
	BOOL LDOAdjust(const PBYTE buff, WORD len);
	BOOL AutoCalibration(BYTE mode, const PBYTE buff, WORD len);

	BOOL SendReadCommand(PBYTE cmd, PBYTE buff = NULL, WORD len = 0);

	BOOL SetUSBMode();
	BOOL Touch_HexUpgrade(PBYTE pBuff, WORD len);
	BOOL GetCapAverage(PBYTE buff, USHORT rLen, BYTE capNum, UINT timeout=200);
};

