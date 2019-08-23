#pragma once

#ifdef  _DLL_LIB_EXPORT_
#define  DLL_EXPORT   __declspec(dllexport)
#else
#define  DLL_EXPORT   __declspec(dllimport)
#endif

#define SPI_UPGRADE_RETRY		(5)

class DLL_EXPORT CMSDevice :
	public CDeviceBase
{
public:
	CMSDevice();
	CMSDevice(const CString& name);
	virtual ~CMSDevice();

private:
	UINT   m_SenseCode;
private:
	BOOL DeviceIO(void* pCMD, DWORD DataTransferLength = NULL, PVOID pBuff = NULL, BYTE DataIn = SCSI_IOCTL_DATA_IN, PDWORD RetLength = NULL);
	BOOL BaseIO(BYTE opCode, BYTE subCode = NULL, unsigned long pageAdd = NULL, int secCnt = NULL,
		LPVOID pBuff = NULL, int In_or_Out = SCSI_IOCTL_DATA_OUT, DWORD Chip = NULL, DWORD Residue = NULL, DWORD TrxLen = NULL, DWORD KeyAdd = 0xFFFFF000,
		UINT  uMaxSecLen = 0xFFFFFFFF);

	BOOL DeviceInquiry(PVOID pScsiInquiryResult, UINT BuffLebgth);
	BOOL SPI_EraseHalfBlk_32K(const UINT uBlkAddr);
	BOOL SPI_EraseBlk_64K(const UINT uBlkAddr);

	BOOL SPI_Read_ID(PBYTE vtBuff, UINT uAddress, const UINT uSecCnt, BOOL Refresh = TRUE);
	BOOL SPI_ReadData(PBYTE vtBuff, UINT uAddress, const UINT uSecCnt);
	BOOL SPI_WriteData(PBYTE vtBuff, UINT uAddress, const UINT uSecCnt);
	BOOL DownLoadData(LPVOID lpBuff, DWORD BuffLen, BOOL CopyFlag = FALSE);
	BOOL DisableExtCode();
	BOOL SPI_ResetDevice();

	BOOL FirmwareUpgrade_Single(PBYTE buff, DWORD len);

public:
	BOOL FirmwareUpgrade(PBYTE buff, DWORD len);
	BOOL SPI_EraseAll();
	BOOL SPI_Read(PBYTE buff, DWORD len);
	BOOL DeviceReset(PBYTE buff = NULL, DWORD len = 0);	
	BOOL IsMassStorage() { return TRUE; }
};

