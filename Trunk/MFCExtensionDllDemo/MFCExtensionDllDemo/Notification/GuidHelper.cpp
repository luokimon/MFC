#include "stdafx.h"
#include "../stdafx.h"
#include "GuidHelper.h"

GUID GUID_USB_HOSTBOARD = { 0x7852339BL, 0x4992, 0x474C, {0xBB, 0x87, 0xB9, 0x3B, 0x2C, 0x94, 0x9C, 0xFE} };
#define USB_HOST_PREFIX		_T("\\\\?\\usb#vid_284b&pid_3000")

GUID GUID_USB_DISK = { 0x53F56307L, 0xB6BF, 0x11D0, {0x94, 0xF2, 0x00, 0xA0, 0xC9, 0x1E, 0xFB, 0x8B} };
GUID GUID_USB_HID = { 0x4D1E55B2L, 0xF16F, 0x11CF, {0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30} };

CGuidHelper::CGuidHelper()
{
}


CGuidHelper::~CGuidHelper()
{
}

BOOL CGuidHelper::IsHostBoard(GUID guid)
{
	return guid == GUID_USB_HOSTBOARD;
}

GUID CGuidHelper::GetHostBoard()
{
	return GUID_USB_HOSTBOARD;
}

CString CGuidHelper::GetHostBoardPrefix()
{
	return USB_HOST_PREFIX;
}