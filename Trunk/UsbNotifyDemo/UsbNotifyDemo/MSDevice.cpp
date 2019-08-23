#include "stdafx.h"
#include "MSDevice.h"


CMSDevice::CMSDevice()
{
}

CMSDevice::CMSDevice(const CString& name)
	:CMSDevice()
{
	m_strName = new CString(name);
}

CMSDevice::~CMSDevice()
{
}

long  GetSenseKey(PBYTE SenseBuf)
{
	unsigned long iRetCode = 0x00;
	unsigned char SenseKey = SenseBuf[2];
	SenseKey = SenseKey & 15;
	unsigned char ASC = SenseBuf[12];
	unsigned char ASCQ = SenseBuf[13];

	iRetCode = SenseKey << 16 | ASC << 8 | ASCQ;
	return  iRetCode;
}

BOOL CMSDevice::DeviceIO(void* pCMD, DWORD DataTransferLength, PVOID pBuff, BYTE DataIn, PDWORD RetLength)
{
#define    MAX_IO_BUFF            ((UINT)128*512)

	DWORD		 RetLen;
	PDWORD       pRetLen = NULL;

	if (INVALID_HANDLE_VALUE == m_hDisk)
		return FALSE;

	if (NULL == RetLength)
		pRetLen = &RetLen;
	else
		pRetLen = RetLength;

	DataTransferLength = DataTransferLength < MAX_IO_BUFF ? DataTransferLength : MAX_IO_BUFF;

	SCSI_PASS_THROUGH_WITH_BUFFER  sptwb;
	ZeroMemory(&sptwb, sizeof(SCSI_PASS_THROUGH_WITH_BUFFER));
	sptwb.spt.Length = sizeof(SCSI_PASS_THROUGH);
	sptwb.spt.PathId = 0;
	sptwb.spt.TargetId = 0;
	sptwb.spt.Lun = 0;
	sptwb.spt.SenseInfoLength = 24;
	sptwb.spt.TimeOutValue = 1;
	sptwb.spt.CdbLength = 16;
	sptwb.spt.DataIn = DataIn;
	sptwb.spt.DataTransferLength = DataTransferLength;

	memcpy(sptwb.spt.Cdb, pCMD, 16);


	if ((SCSI_IOCTL_DATA_OUT == DataIn) && (NULL != pBuff))
	{
		memcpy_s(sptwb.ucDataBuf, MAX_IO_BUFF, pBuff, DataTransferLength);
	}

	sptwb.spt.DataBufferOffset = offsetof(SCSI_PASS_THROUGH_WITH_BUFFER, ucDataBuf);
	sptwb.spt.SenseInfoOffset = offsetof(SCSI_PASS_THROUGH_WITH_BUFFER, ucSenseBuf);
	ULONG length = offsetof(SCSI_PASS_THROUGH_WITH_BUFFER, ucDataBuf) + sptwb.spt.DataTransferLength;


	BOOL status = DeviceIoControl(
		m_hDisk,
		IOCTL_SCSI_PASS_THROUGH,
		&sptwb,
		sizeof(SCSI_PASS_THROUGH_WITH_BUFFER),
		&sptwb,
		length,
		pRetLen,
		NULL);

	if (!status)
	{
		//CloseDeviceHandle();
		return FALSE;
	}

	if ((SCSI_IOCTL_DATA_IN == DataIn) && (NULL != pBuff))
	{
		memcpy_s(pBuff, MAX_IO_BUFF, sptwb.ucDataBuf, DataTransferLength);
	}

	BYTE         SenseBuf[SENSEBUFSIZE];          //SenseCode
	memcpy(SenseBuf, sptwb.ucSenseBuf, SENSEBUFSIZE);

	m_SenseCode = GetSenseKey(SenseBuf);//命令失败状态码

	return (status && (0 == m_SenseCode));
}

BOOL CMSDevice::DeviceInquiry(PVOID     pScsiInquiryResult, UINT       BuffLebgth)
{
	BYTE           CBD[12] = { 0, 0, 0 };
	CBD[0] = 0x12;
	CBD[4] = 0x24;
	return DeviceIO(CBD, BuffLebgth, pScsiInquiryResult);

}

BOOL CMSDevice::SPI_Read_ID(PBYTE vtBuff, UINT uAddress, const UINT uSecCnt, BOOL Refresh)
{
	//if (Refresh)
	{
		//BYTE buff[16*1024*2];
		//ZeroMemory(buff, 16 * 1024 * 2);
		//DownLoadData(buff, 16 * 1024);
	}
	return BaseIO(0xF0, 0x13, uAddress, uSecCnt, vtBuff, SCSI_IOCTL_DATA_IN);
}

BOOL CMSDevice::DisableExtCode()
{
	std::unique_ptr<BYTE[]> buff(new BYTE[1024]);
	BUFF_COPY_T	bc;

	ZeroMemory(&bc, sizeof(bc));
	ZeroMemory(buff.get(), 1024);

	bc.m_Source0ffset_0 = RAM_DMA_BUFF_ADDRESS + sizeof(bc) - RAM_BASE_ADDRESS;
	bc.m_DestOffset_0 = RAM_BASE_ADDRESS - RAM_BASE_ADDRESS;          //m_ef 所在的地址
	bc.m_CopyLenght_0 = 4;

	memcpy(buff.get(), &bc, sizeof(bc));

	return DownLoadData(buff.get(), 1024, TRUE);
}

BOOL CMSDevice::SPI_ResetDevice()
{
	BYTE buf[512] = { 0 };

	return BaseIO(0xC3, 0, 0, 1, buf, SCSI_IOCTL_DATA_OUT);
}

BOOL CMSDevice::SPI_EraseHalfBlk_32K(const UINT uBlkAddr)
{
	return BaseIO(0xF0, 0x16, uBlkAddr);
}

BOOL CMSDevice::SPI_EraseBlk_64K(const UINT uBlkAddr)
{
	return BaseIO(0xC0, 0x00, uBlkAddr);
}

BOOL CMSDevice::SPI_WriteData(PBYTE vtBuff, UINT uAddress, const UINT uSecCnt)
{
	return BaseIO(0xC1, 0x00, uAddress, uSecCnt, vtBuff, SCSI_IOCTL_DATA_OUT);
}

BOOL CMSDevice::SPI_ReadData(PBYTE vtBuff, UINT uAddress, const UINT uSecCnt)
{
	return BaseIO(0xC2, 0x00, uAddress, uSecCnt, vtBuff, SCSI_IOCTL_DATA_IN);
}


BOOL CMSDevice::DownLoadData(LPVOID lpBuff, DWORD BuffLen, BOOL CopyFlag)
{
	BOOL     bRet = FALSE;
	//UINT32   SecNum = ALIGN_512(BuffLen) / 512;
	UINT32 SecNum = (BuffLen+511) / 512;

	BYTE   strbuf[64 * 1024];

	ZeroMemory(strbuf, sizeof(strbuf));
	memcpy(strbuf, lpBuff, BuffLen);

	return BaseIO(0xF0, 0, CopyFlag, SecNum, strbuf, SCSI_IOCTL_DATA_OUT);

}

DWORD ReverseDWORD(DWORD InData)
{
	BYTE   da1 = (BYTE)(InData);
	BYTE   da2 = (BYTE)(InData >> 8);
	BYTE   da3 = (BYTE)(InData >> 16);
	BYTE   da4 = (BYTE)(InData >> 24);

	return ((((DWORD)da1) << 24) | (((DWORD)da2) << 16) | (((DWORD)da3) << 8) | (((DWORD)da4)));
}

WORD WORDReverse(WORD InData)
{
	return  ((InData >> 8) | (InData << 8));
}


BOOL CMSDevice::BaseIO(BYTE opCode, BYTE subCode, unsigned long pageAdd, int secCnt, LPVOID  pBuff, int In_or_Out, DWORD Chip, DWORD Residue, DWORD TrxLen, DWORD KeyAdd, UINT  uMaxSecLen)
{
	MSGCMD_USB m_Scsi_Cmd;
	ZeroMemory(&m_Scsi_Cmd, sizeof(m_Scsi_Cmd));

	m_Scsi_Cmd.OpCode = opCode;
	m_Scsi_Cmd.SubOpCode = subCode;
	if ((0xF0 == opCode) || (0xA2 == opCode) || (0xA3 == opCode) || (0xB2 == opCode) || (0xB3 == opCode))
	{
		m_Scsi_Cmd.Address = ReverseDWORD(pageAdd);
		m_Scsi_Cmd.Length = WORDReverse(secCnt);
		m_Scsi_Cmd.Residue = ReverseDWORD(Residue);
		m_Scsi_Cmd.TrxLen = WORDReverse(TrxLen);    //此处USB 命令只有三个Byte，软件自己反转
	}
	else
	{
		m_Scsi_Cmd.Address = (pageAdd);
		m_Scsi_Cmd.Length = (secCnt);
		m_Scsi_Cmd.Residue = (Residue);
		m_Scsi_Cmd.TrxLen = (TrxLen);
	}

	m_Scsi_Cmd.Chip = Chip;

	DWORD DataTransferLength = secCnt * SECTOR;

	return DeviceIO(&m_Scsi_Cmd, DataTransferLength, pBuff, In_or_Out);
}

//BOOL CMSDevice::FirmwareUpgrade_Single(PBYTE buff, DWORD len)
BOOL CMSDevice::FirmwareUpgrade(PBYTE buff, DWORD len)
{
	if ((NULL == buff) || (0 == len))
		return FALSE;

	SetTaskPosition(0);

	BOOL ret = FALSE;
	for (int retry = 0; retry < SPI_UPGRADE_RETRY; retry++)
	{
		if (FirmwareUpgrade_Single(buff, len))
		{
			ret = TRUE;
			break;
		}
		Sleep(200);
	}

	SetTaskPosition(100);

	return ret;
}

BOOL CMSDevice::FirmwareUpgrade_Single(PBYTE buff, DWORD len)
//BOOL CMSDevice::FirmwareUpgrade(PBYTE buff, DWORD len)
{

	if (!SPI_EraseBlk_64K(0))
	{
		TRACE(_T("FirmwareUpgrade-SPI_EraseBlk_64K Fail!\n"));
		return FALSE;
	}

	Sleep(100);

	UINT secCnt = (511 + len) / 512;
	for (UINT i = 0; i < (secCnt + 7) / 8; i++)
	{
		UINT cnt = (secCnt - 8 * i) > 8 ? 8 : secCnt - 8 * i;
		if (!SPI_WriteData(&buff[i * 8 * 512], 512 + i * 8 * 512, cnt))
		{
			//TRACE(_T("FirmwareUpgrade-SPI_WriteData Fail!\n"));
			CWarningMessage::Show(_T("FirmwareUpgrade-SPI_WriteData Fail!"));
			return FALSE;
		}
		Sleep(50);
	}

	std::unique_ptr<BYTE[]> readBuf(new BYTE[len]);
	ZeroMemory(readBuf.get(), len);

	BOOL ret = FALSE;

	for (UINT i = 0; i < (secCnt + 7) / 8; i++)
	{
		UINT cnt = (secCnt - 8 * i) > 8 ? 8 : secCnt - 8 * i;
		if (!SPI_ReadData(&readBuf[i * 8 * 512], 512 + i * 8 * 512, cnt))
		{
			TRACE(_T("FirmwareUpgrade-SPI_ReadData Fail!\n"));
			return FALSE;
		}
		Sleep(50);
	}

	if (0 == memcmp(buff, readBuf.get(), len))
		ret = TRUE;
	else
		TRACE(_T("FirmwareUpgrade-DataCompare Fail!\n"));

	return ret;
}

BOOL CMSDevice::SPI_EraseAll()
{
	if (!SPI_EraseBlk_64K(0))
		return FALSE;
	return TRUE;
}

BOOL CMSDevice::SPI_Read(PBYTE buff, DWORD len)
{
	UINT secCnt = (511 + len) / 512;
	BOOL ret = TRUE;
	
	SetTaskPosition(0);

	do
	{
		UINT cnt = (secCnt + 7) / 8;
		for (UINT i = 0; i < cnt; i++)
		{
			SetTaskPosition((i * 100) / cnt);

			UINT cnt = (secCnt - 8 * i) > 8 ? 8 : secCnt - 8 * i;
			if (!SPI_ReadData(&buff[i * 8 * 512], 512 + i * 8 * 512, cnt))
			{	
				ret = FALSE;
				break;
			}
			Sleep(50);
		}
	} while (0);

	SetTaskPosition(100);

	return ret;
}

BOOL CMSDevice::DeviceReset(PBYTE buff, DWORD len)
{
	if (0 == len)
		return FALSE;

	//if (!DisableExtCode())
	//	return FALSE;

	if (!DownLoadData(buff, len, TRUE))
		return FALSE;

	Sleep(100);

	SPI_ResetDevice();

	//DWORD errCode = GetLastError();
	//TRACE(_T("Last Error(%d) %s \n"), errCode, LastErrorCodeToString(errCode));


	return TRUE;
}