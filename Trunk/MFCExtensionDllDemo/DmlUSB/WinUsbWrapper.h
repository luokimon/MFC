#pragma once
class CWinUsbWrapper
{
public:
	CWinUsbWrapper();
	virtual ~CWinUsbWrapper();

private:
	HANDLE	m_hDevice;
	CString m_devPath;
	INT		m_nMaxEndpoints;
	WINUSB_INTERFACE_HANDLE m_winUsbHandle;
	std::vector<WINUSB_PIPE_INFORMATION>* m_pipeInfoList;


private:
	BOOL WinUsbInitialize();

public:
	enum
	{
		ENDPOINT_SPI = 0x81,
		ENDPOINT_I2C = 0x82
	};
public:
	BOOL OpenDevice(LPCTSTR path);
	void CloseDevice();

	BOOL ControlTransfer(WINUSB_SETUP_PACKET setupPacket,
		PBYTE				 Buffer,
		ULONG				 BufferLength,
		PULONG				 lenTransferred);

	BOOL ReadBulkPipe(IN  BYTE	  pipeID,
		IN  PUCHAR  Buffer,
		IN  ULONG   bufSize,
		OUT PULONG  lenTransferred,
		IN  LPOVERLAPPED  Overlapped);

	BOOL FlushPipe(UCHAR PipeID);

	UCHAR GetI2CEndpoint();
	UCHAR GetSPIEndpoint();
};

