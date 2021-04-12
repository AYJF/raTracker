#ifndef VARY_H
#define VARY_H

#include <windows.h>
#include <setupapi.h>
#include <stdlib.h>
#include <winioctl.h>
#include <WinNT.h>
#include <tchar.h>	// Note UNICODE may not work for Win98??

#define FILE_DEVICE_PCI  0x8000

// Define Interface reference/dereference routines for
// Interfaces exported by IRP_MN_QUERY_INTERFACE

#define PCI__IOCTL(index) \
    CTL_CODE(FILE_DEVICE_PCI, index, METHOD_BUFFERED, FILE_READ_DATA)

#define IOCTL_PCI_READ \
    CTL_CODE(FILE_DEVICE_PCI, 0x800, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_PCI_WRITE \
    CTL_CODE(FILE_DEVICE_PCI, 0x801, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_PCI_READCONFIG \
    CTL_CODE(FILE_DEVICE_PCI, 0x802, METHOD_IN_DIRECT, FILE_READ_ACCESS)
#define IOCTL_PCI_WRITECONFIG \
    CTL_CODE(FILE_DEVICE_PCI, 0x803, METHOD_OUT_DIRECT, FILE_READ_ACCESS)
#define IOCTL_PCI_DMA \
    CTL_CODE(FILE_DEVICE_PCI, 0x804, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_PCI_DMASETTING \
    CTL_CODE(FILE_DEVICE_PCI, 0x805, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_PCI_DMASTOP \
    CTL_CODE(FILE_DEVICE_PCI, 0x806, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_PCI_DATAREAD \
    CTL_CODE(FILE_DEVICE_PCI, 0x807, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define DEVINTF_INLINE inline

typedef struct RW_ARG
{
	DWORD FIRST;
	DWORD SECOND;
    
} RWARG,*PRWARG;
typedef struct DMA_ARG
{
	DWORD FIRST;
	DWORD SECOND;
	DWORD THIRD;
    
} DMAARG,*PDMAARG;

#define DEVINTF_INLINE inline
//////////////////////////////////////////////////////////////////////////////
// class CDeviceInterfaceClass
//
class CDeviceInterfaceClass
{
public:
	CDeviceInterfaceClass(GUID* pClassGuid, PDWORD status);
	~CDeviceInterfaceClass(void);
	GUID* GetGuid(void)      { return &m_Guid; }
	HDEVINFO GetHandle(void) { return m_hInfo; }

protected:
	HDEVINFO		m_hInfo;
	GUID			m_Guid;
};

//////////////////////////////////////////////////////////////////////////////
// class CDeviceInterface
//
class CDeviceInterface
{
public:
	DEVINTF_INLINE CDeviceInterface( CDeviceInterfaceClass* pClassObject, DWORD Index, PDWORD Error );
	DEVINTF_INLINE ~CDeviceInterface(void);
	DEVINTF_INLINE TCHAR* DevicePath(void);

protected:
	CDeviceInterfaceClass*				m_Class;
	SP_DEVICE_INTERFACE_DATA			m_Data;
	PSP_INTERFACE_DEVICE_DETAIL_DATA	m_Detail;
};


//////////////////////////////////////////////////////////////////////////////
// CDeviceInterfaceClass constructor
//
DEVINTF_INLINE CDeviceInterfaceClass::CDeviceInterfaceClass(
	GUID* pClassGuid, 
	PDWORD status
	) 
{
	DWORD flags = DIGCF_DEVICEINTERFACE | DIGCF_PRESENT ;
	m_hInfo = INVALID_HANDLE_VALUE;
	ZeroMemory(&m_Guid,sizeof(GUID));

	try
	{
		*status = ERROR_INVALID_PARAMETER;
		m_Guid = *pClassGuid;
		m_hInfo = SetupDiGetClassDevs(pClassGuid, NULL, NULL, flags);

		if ( m_hInfo == INVALID_HANDLE_VALUE )
			*status = GetLastError();
		else
			*status = ERROR_SUCCESS;

	}
	catch (...)
	{
		m_hInfo = INVALID_HANDLE_VALUE;
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterfaceClass destructor
//
DEVINTF_INLINE CDeviceInterfaceClass::~CDeviceInterfaceClass(void)
{
	if ( m_hInfo != INVALID_HANDLE_VALUE )
		SetupDiDestroyDeviceInfoList(m_hInfo);

	m_hInfo = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterface constructor
//
DEVINTF_INLINE CDeviceInterface::CDeviceInterface(
	CDeviceInterfaceClass* pClassObject, 
	DWORD Index,
	PDWORD Error
	)
{
	m_Class = pClassObject;

	BOOL status;
	DWORD ReqLen;
	
	m_Detail = NULL;
	m_Data.cbSize = sizeof SP_DEVICE_INTERFACE_DATA;
	try
	{
		*Error = ERROR_INVALID_PARAMETER;

		status = SetupDiEnumDeviceInterfaces(
			m_Class->GetHandle(), 
			NULL, 
			m_Class->GetGuid(), 
			Index, 
			&m_Data
			);

		if ( !status )
		{
			*Error = GetLastError();
			return;
		}					  

		SetupDiGetInterfaceDeviceDetail (
			m_Class->GetHandle(),
			&m_Data,
			NULL,
			0,
			&ReqLen,
			NULL 
			);

		*Error = GetLastError();

		if ( *Error != ERROR_INSUFFICIENT_BUFFER )
			return;

		m_Detail = PSP_INTERFACE_DEVICE_DETAIL_DATA(new char[ReqLen]);

		if ( !m_Detail )
		{
			*Error = ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		m_Detail->cbSize = sizeof SP_INTERFACE_DEVICE_DETAIL_DATA;

		status = SetupDiGetInterfaceDeviceDetail (
			m_Class->GetHandle(),
			&m_Data,
			m_Detail,
			ReqLen,
			&ReqLen,
			NULL 
			);

		if ( !status )
		{
			*Error = GetLastError();
			delete m_Detail;
			m_Detail = NULL;
			return;
		}
	
		*Error = ERROR_SUCCESS;
	}
	catch (...)
	{
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterface destructor
//
DEVINTF_INLINE CDeviceInterface::~CDeviceInterface(void)
{
	if (m_Detail)
	{
		delete m_Detail;
		m_Detail = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterface::DevicePath
//
DEVINTF_INLINE TCHAR* CDeviceInterface::DevicePath(void)
{
	try
	{
		if ( m_Detail)
			return m_Detail->DevicePath;
		else
			return NULL;
	}
	catch (...)
	{
		return NULL;
	}

}
//////RAEAD  WRITE CLASS/////////////////////////

class CPCI_RW
{
public:
	
	inline CPCI_RW();
	inline ~CPCI_RW(void);	
    inline BOOL CPCI_RW::OnFine(GUID ClassGuid);

	inline BOOL CPCI_RW::OnRead(DWORD address,DWORD number,DWORD* data);

	inline BOOL CPCI_RW::OnWrite(DWORD address,DWORD number,DWORD* data);

	inline BOOL CPCI_RW::OnCofigRead(DWORD OFFSET,DWORD BYTENUM,PVOID data);

	inline BOOL CPCI_RW::OnCofigWrite(DWORD OFFSET,DWORD BYTENUM,PVOID data);

	inline BOOL CPCI_RW::OnDmaSetting(DWORD unit);

	inline BOOL CPCI_RW::OnDmaStop();

	inline BOOL CPCI_RW::OnDmaReadDATA(DWORD* data,DWORD* ErrNum,DWORD Erase);
    
	BOOL DevIO(int Task);
    HANDLE hDev;
	HANDLE hProcess;
	DWORD m_pData21,m_pData20;
protected:
	DWORD sizen;
	BOOL MAPED;
};
inline CPCI_RW::CPCI_RW()
{
	hDev=0;MAPED=FALSE;
}
inline CPCI_RW::~CPCI_RW(void)
{	
	hDev=0; MAPED=FALSE;
	if(MAPED) 
	{
		OnDmaStop( );
	}    
}
inline BOOL CPCI_RW::OnFine(GUID ClassGuid)
{
	hDev=NULL;
	DWORD pError;
	DWORD instance=0;
	CDeviceInterfaceClass DevClass(&ClassGuid, &pError);
	if (pError == ERROR_SUCCESS)
	{
		CDeviceInterface DevInterface(&DevClass, instance, &pError);
		if (pError == ERROR_SUCCESS)
		{
			hDev = CreateFile(DevInterface.DevicePath(),
				GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);	
			if (hDev != INVALID_HANDLE_VALUE)
			{				
				return TRUE;
			}
		}		
	}
	return FALSE;
}
inline BOOL CPCI_RW::OnCofigRead(DWORD OFFSET,DWORD BYTENUM,PVOID data)
{
	ULONG	nOutput=0;
	RWARG   RWCON;
    RWCON.FIRST=OFFSET,RWCON.SECOND=BYTENUM;
	BOOL res;
	res=DeviceIoControl(hDev,IOCTL_PCI_READCONFIG,&RWCON,sizeof(RWCON), 
		                data,sizeof(RWCON), &nOutput,NULL);
	return (nOutput==BYTENUM&&res);
}
inline BOOL CPCI_RW::OnCofigWrite(DWORD OFFSET,DWORD BYTENUM,PVOID data)
{
	ULONG	nOutput=0;
	RWARG   RWCON;
    RWCON.FIRST=OFFSET,RWCON.SECOND=BYTENUM;
	BOOL res;
	res=DeviceIoControl(hDev,IOCTL_PCI_WRITECONFIG,&RWCON,sizeof(RWCON), 
		                data,sizeof(RWCON), &nOutput,NULL);
	return (nOutput==BYTENUM&&res);
}
inline BOOL CPCI_RW::OnRead(DWORD address,DWORD number,DWORD* data)
{		
	ULONG	nOutput;
	RW_ARG DataRead;DataRead.FIRST=address,DataRead.SECOND=number*4;
	BOOL res;
	//res=ReadFile(hDev,data,number*4,&nOutput,NULL);
	res= DeviceIoControl(hDev,IOCTL_PCI_READ, &DataRead,sizeof(DataRead)
		                               ,data,number*sizeof(DWORD),&nOutput,NULL);
	return (nOutput==number*4&&res);
}
inline BOOL CPCI_RW::OnWrite(DWORD address,DWORD number,DWORD* data)
{	
	ULONG	nOutput;
	BOOL res;
	RW_ARG DataWrite;DataWrite.FIRST=address,DataWrite.SECOND=number*4;
	res=DeviceIoControl(hDev,IOCTL_PCI_WRITE,&DataWrite,sizeof(DataWrite),
						 data,number*sizeof(DWORD),&nOutput,NULL);
	return (nOutput==number*4&&res);
	
}
inline BOOL CPCI_RW::OnDmaSetting(DWORD unit)
{
	DWORD	ff;
	sizen=unit*16;
	RW_ARG MasterStart;MasterStart.FIRST=0x19,MasterStart.SECOND=unit*16;	
	ULONG	nOutput;
	BOOL res;
	res=DeviceIoControl(hDev,IOCTL_PCI_DMASETTING,&MasterStart,sizeof(MasterStart),
						 &ff,sizeof(DWORD),&nOutput,NULL);
    if(res) MAPED=TRUE;
	return res;

}
inline BOOL CPCI_RW::OnDmaReadDATA(DWORD* data,DWORD* ErrNum,DWORD Erase)
{		
	RW_ARG MasterRead;MasterRead.FIRST=Erase,MasterRead.SECOND=0;
	//ULONG	nOutput;
	BOOL res;
	res=DeviceIoControl(hDev,IOCTL_PCI_DATAREAD,&MasterRead,sizeof(MasterRead),
						 data,sizen*0x10000,ErrNum,NULL);
    ULONG dd=GetLastError();
	return res;

}
inline BOOL CPCI_RW::OnDmaStop()
{
	BOOL res;
	RW_ARG MasterStop;MasterStop.FIRST=0x04,MasterStop.SECOND=0x00;	
	ULONG	nOutput;
	res=DeviceIoControl(hDev,IOCTL_PCI_DMASTOP,&MasterStop,sizeof(MasterStop),
						 &MasterStop,sizeof(MasterStop),&nOutput,NULL);
    if(res) MAPED=FALSE;
	return res;
}
inline BOOL CPCI_RW::DevIO(int Task)
{
	if (Task==4)
	{
		OnWrite(0,1,&m_pData21);
	}
	if (Task==5)
	{
		OnRead(0,1,&m_pData20);
		m_pData20=m_pData20 & 0x0fffffff;
	}
	return TRUE;
}

#endif
