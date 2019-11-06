#include "stdafx.h"
#include "WinUsbWrapper.h"


CWinUsbWrapper::CWinUsbWrapper()
{
	m_hDevice = INVALID_HANDLE_VALUE;
	m_nMaxEndpoints = 0;
	m_winUsbHandle = NULL;
	m_pipeInfoList = new std::vector<WINUSB_PIPE_INFORMATION>();

}


CWinUsbWrapper::~CWinUsbWrapper()
{
	CloseDevice();
}


void CWinUsbWrapper::CloseDevice()
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

BOOL CWinUsbWrapper::OpenDevice(LPCTSTR path)
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

BOOL CWinUsbWrapper::WinUsbInitialize()
{
	if (NULL != m_winUsbHandle)
		WinUsb_Free(m_winUsbHandle);

	if (!WinUsb_Initialize(m_hDevice, &m_winUsbHandle))
		return FALSE;

	do
	{
		USB_INTERFACE_DESCRIPTOR ifaceDescriptor;
		ZeroMemory(&ifaceDescriptor, sizeof(USB_INTERFACE_DESCRIPTOR));

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

BOOL CWinUsbWrapper::ControlTransfer(WINUSB_SETUP_PACKET setupPacket,
	PBYTE				 Buffer,
	ULONG				 BufferLength,
	PULONG				 lenTransferred)
{
	return WinUsb_ControlTransfer(m_winUsbHandle,
		setupPacket,
		Buffer,
		BufferLength,
		lenTransferred,
		NULL);
}

BOOL CWinUsbWrapper::ReadBulkPipe(IN  BYTE	  pipeID,
	IN  PUCHAR  Buffer,
	IN  ULONG   bufSize,
	OUT PULONG  lenTransferred,
	IN  LPOVERLAPPED  Overlapped)
{
	return WinUsb_ReadPipe(m_winUsbHandle,
		pipeID,
		Buffer,
		bufSize,
		lenTransferred,
		Overlapped);
}

BOOL CWinUsbWrapper::FlushPipe( UCHAR PipeID)
{
	return WinUsb_FlushPipe(m_winUsbHandle, PipeID);
}


UCHAR CWinUsbWrapper::GetI2CEndpoint()
{
	return ENDPOINT_I2C;
}

UCHAR CWinUsbWrapper::GetSPIEndpoint()
{
	return ENDPOINT_SPI;
}