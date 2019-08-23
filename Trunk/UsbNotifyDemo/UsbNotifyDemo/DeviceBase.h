#pragma once

#ifdef  _DLL_LIB_EXPORT_
#define  DLL_EXPORT   __declspec(dllexport)
#else
#define  DLL_EXPORT   __declspec(dllimport)
#endif

#include <winioctl.h>

#define SCSI_IOCTL_DATA_OUT		0
#define	SCSI_IOCTL_DATA_IN		1

#define CREATE_FILE_RETRY		(5)

#define	 SENSEBUFSIZE			(32)    //DeviceIo ���� sense����

#define		    SECTOR									(512)                       //һ�������������ֽڸ���

#define		DMA_BUFF_SIZE				(16 * 1024)                               //DMA ram ������
#define     DMA_BUFF_SIZE_8380A         (32*1024)                               //DMA ram ������
#define     DMA_BUFF_LEN                DMA_BUFF_SIZE
#define     DMA_BUFF_LEN_8380A          DMA_BUFF_SIZE_8380A

#define IOCTL_SCSI_BASE							FILE_DEVICE_CONTROLLER
#define IOCTL_SCSI_PASS_THROUGH					CTL_CODE(IOCTL_SCSI_BASE, 0x0401, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#pragma pack(push)
#pragma pack(1)

typedef struct 	MSGCMD_USB_     //USBͨѶ��صĽṹ�壬 SCSI
{
	unsigned char		OpCode;		//0
	unsigned char		SubOpCode;	//1
	unsigned long		Address;	//2,3,4,5
	unsigned char		Chip;		//6
	unsigned short		Length;		//7,8
	unsigned long       Residue;	//9,10,11,12  ʣ��,��
	//unsigned long		TrxLen;		//13,14,15,16
	unsigned short		TrxLen;		//13,14   //�޸ĳ�2��2Byte
	unsigned short		TrxLen1;    //15,16  
	//unsigned char		Page;		//17
}MSGCMD_USB, *PMSG_USB;
#pragma pack(pop)

typedef struct _SCSI_PASS_THROUGH {	//SCSI�ײ�������Ҫ�õ�
	USHORT Length;
	UCHAR ScsiStatus;
	UCHAR PathId;
	UCHAR TargetId;
	UCHAR Lun;
	UCHAR CdbLength;
	UCHAR SenseInfoLength;
	UCHAR DataIn;
	ULONG DataTransferLength;
	ULONG TimeOutValue;
	ULONG DataBufferOffset;
	ULONG SenseInfoOffset;
	UCHAR Cdb[16];
}SCSI_PASS_THROUGH, *PSCSI_PASS_THROUGH;

typedef struct _SCSI_PASS_THROUGH_WITH_BUFFER {
	SCSI_PASS_THROUGH spt;
	ULONG             Filler;      // realign buffers to double word boundary
	UCHAR             ucSenseBuf[32];
	UCHAR             ucDataBuf[256 * 528];
} SCSI_PASS_THROUGH_WITH_BUFFER, *PSCSI_PASS_THROUGH_WITH_BUFFER;

typedef struct  SPI_BOOT_TABLE_8380_A_T
{
	UINT32     TableFlg[2];             // 8���ֽڵı�ͷ����ʶ������Ϊ����������˶ԣ�[0]        
	UINT32     FlyCode_RamAdd;         //����flycode��ʱ�򣬶�Ӧ��RAM��ʼ��ַ
	UINT32     FlyCodeSectorCount;     //��Ҫ���ص�Flycode�ĳ���
	UINT32     GlobalTable_RamAdd;     //ȫ�ֱ������ran�еĵ�ַ;
	UINT32     FlyCodeCheckSum32;      // FlyCode�����ֵ[0+16]  //++[��1���ֽڱ��4���ֽ�]
	UINT32     TabCheckSum32;          // �����������ݵ����ֵ��  ��BYTE(8Bit)�������    
}SPI_BOOT_TABLE_8380_A;

typedef struct BUFF_COPY_T_
{
	WORD   m_Source0ffset_0;                   //Դָ�룬��Ե�ǰbuff��ƫ��ֵ
	WORD   m_Source0ffset_1;                   //Դָ�룬��Ե�ǰbuff��ƫ��ֵ
	WORD   m_Source0ffset_2;                   //Դָ�룬��Ե�ǰbuff��ƫ��ֵ
	WORD   m_DestOffset_0;                      //Ŀ���ַָ�룬MCU RAM �ľ��Ե�ַ
	WORD   m_DestOffset_1;                      //Ŀ���ַָ�룬MCU RAM �ľ��Ե�ַ
	WORD   m_DestOffset_2;                      //Ŀ���ַָ�룬MCU RAM �ľ��Ե�ַ
	WORD   m_CopyLenght_0;                       //���������ݳ���
	WORD   m_CopyLenght_1;                       //���������ݳ���   
	WORD   m_CopyLenght_2;                       //���������ݳ���   
}BUFF_COPY_T;

#define		RAM_BASE_ADDRESS			(0x20000000)
#define     RAM_BASE_ADDRESS_8380       (0x00000000)
#define		RAM_DMA_BUFF_ADDRESS		(0x20000940)

#define		BOOT_TABLE_FLG_0			(0x9BF4AD4C)		                    //Boot Table Key Word
#define		BOOT_TABLE_FLG_1			(0xA1B3C5F7)		                    //Boot Table Key Word


typedef int(*ProgressCallBack)(PVOID wnd, int nItem, int pos);
enum HID_WRITE_TYPE
{
	hwtCommand = 0x00,
	hwtTouch = 0x01,
	hwtUsb = 0x02
};

enum TOUCH_SUB_COMMAND
{
	tscUnknown = 0x00,
	tscEraseAll = 0x10,
	tscErase = 0x11,
	tscStartProgram = 0x12,
	tscStartPageProgram = 0x13,
	tscReadEeprom = 0x14,
	tscEepromAccess = 0x15,
	tscEepromForceErase = 0x16,
	tscWriteCommand = 0x30,
	tscReadCommand = 0x31,
	//tscReadRowCap = 0x40,
	//tscReportMode = 0x50
	tscResetDevice	= 0x40,
	tscResetCheck	= 0x41,
	tscRcAdjust		= 0x50
};

typedef int(*TaskCallBack)(PVOID obj, UINT nItem, UINT nPos);

class DLL_EXPORT CDeviceBase
{
public:
	CDeviceBase();
	virtual ~CDeviceBase();

protected:
	CString* m_strName;
	HANDLE m_hDisk;
	UINT m_nStartPos;
	UINT m_nStopPos;
	UINT m_nItem;
	PVOID m_pObject;
	TaskCallBack m_pCallback;

protected:
	virtual VOID SetTaskPosition(UINT pos);

public:
	virtual BOOL OpenDeviceHandle();
	virtual void CloseDeviceHandle();

	virtual VOID Free(CDeviceBase* base);

	virtual VOID SetListCtrlItem(UINT nItem) { m_nItem = nItem; }
	virtual VOID SetProgressPosition(UINT start, UINT stop) { m_nStartPos = start; m_nStopPos = stop; }
	virtual VOID SetCallback(PVOID obj, TaskCallBack callback, UINT start = 0, UINT stop = 100);


	virtual BOOL FirmwareUpgrade(PBYTE buff, DWORD len) { return FALSE; }
	virtual BOOL SPI_EraseAll() { return FALSE; }
	virtual BOOL SPI_Read(PBYTE buff, DWORD len) { return FALSE; }
	virtual BOOL DeviceReset(PBYTE buff = NULL, DWORD len = 0) = 0;

	//virtual BOOL SetUpdateMode(BOOL mode) { return TRUE; }

	//virtual BOOL Eeprom_EraseAll() { return TRUE; }
	//virtual BOOL Touch_Erase(WORD addr, WORD len) { return TRUE; }
	//virtual BOOL Touch_EnableProgram(WORD addr, WORD len) { return TRUE; }
	//virtual BOOL Touch_EnablePageProgram(WORD addr, WORD len) { return TRUE; }
	//virtual BOOL Eeprom_Read(WORD addr, WORD len) { return TRUE; }
	//virtual BOOL Touch_EepromAccess(BOOL access) { return TRUE; }
	//virtual BOOL Touch_WriteCommand(const PBYTE buff, const PBYTE cmd, WORD len) { return TRUE; }
	//virtual BOOL Touch_ReadCommand(const PBYTE buff, const PBYTE cmd, WORD len) { return TRUE; }

	//virtual BOOL GetBridgeInfo(PBYTE buff, WORD len) {return FALSE;}

	//virtual BOOL Touch_EepromEraseAll(BOOL accessOn = TRUE, BOOL accessOff = TRUE) { return FALSE; }
	//virtual BOOL Touch_EepromForceErase() { return FALSE; }
	//virtual BOOL Touch_EepromRead(PBYTE buff, WORD addr, WORD len, BOOL accessOn = TRUE, BOOL accessOff = TRUE) { return TRUE; }
	//virtual BOOL Touch_AdbEepromRead(PBYTE buff, WORD addr, WORD len) { return TRUE; }
	//virtual BOOL Touch_EepromErase(WORD addr, WORD len, BOOL accessOn = TRUE, BOOL accessOff = TRUE) { return TRUE; }


	//virtual BOOL Touch_EepromPageProgram(PBYTE buff, WORD addr, WORD len, BOOL accessOn = TRUE, BOOL accessOff = TRUE) { return TRUE; }
	//virtual BOOL Eeprom_MfpUpgrade(PBYTE buff, WORD addr, WORD len, PVOID wnd, ProgressCallBack callback) { return FALSE; }
	//virtual BOOL Eeprom_ParamUpgrade(PBYTE buff, WORD addr, WORD len) { return FALSE; }

	//virtual BOOL ReadDeviceInfo(PBYTE buff, UINT len, USHORT timeout = 3000) { return FALSE; }

	virtual BOOL IsTouchScreen() { return FALSE; }
	virtual BOOL IsMassStorage() { return FALSE; }

	static BOOL IsTouchScreenName(const CString& name) { 
		return (0 == name.Find(_T("\\\\?\\hid#vid_26ae&pid_8507")))||
			(0 == name.Find(_T("\\\\?\\hid#vid_222a&pid_0001")));
	}
	static BOOL IsMassStorageName(const CString& name) { return (0 == name.Find(_T("\\\\?\\usbstor#disk&ven_usb&prod_flash_drive&rev_1.68"))); }

};
