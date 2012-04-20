/*
	Copyright(C) Nishant Sivakumar, 2005. All rights reserved.
	http://blog.voidnish.com
*/

#include "StdAfx.h"
#include "NetworkAdapter.h"

CodeProjectUtils::CNetworkAdapterList::CNetworkAdapterList(void)
{
	pAdapterInfo = NULL;
	ULONG OutBufLen = 0;
	m_count = 0;

	//Get the required size of the buffer
	if( GetAdaptersInfo(NULL, &OutBufLen) == ERROR_BUFFER_OVERFLOW )
	{
		int divisor = sizeof IP_ADAPTER_INFO;
		if(sizeof time_t == 8)
			divisor -= 8;

		m_count = OutBufLen/divisor;
		pAdapterInfo = new IP_ADAPTER_INFO[m_count];

		//Get info for the adapters
		if( GetAdaptersInfo(pAdapterInfo, &OutBufLen) != ERROR_SUCCESS )
		{
			//Call failed
			delete[] pAdapterInfo;
			pAdapterInfo = NULL;
			m_count = 0;
		}		
	}	
}

CodeProjectUtils::CNetworkAdapterList::~CNetworkAdapterList(void)
{
	delete[] pAdapterInfo;
	pAdapterInfo = NULL;
}


//IsValid returns true if the adapter info has been successfully obtained.
//This function should be called before using the class.
bool CodeProjectUtils::CNetworkAdapterList::IsValid(void)
{
	return !(pAdapterInfo == NULL);
}

int CodeProjectUtils::CNetworkAdapterList::GetCount(void)
{
	if(!IsValid())
		return -1;
	return m_count;
}

int CodeProjectUtils::CNetworkAdapterList::GetAdapters(ADAPTERINFO* pADAPTERINFO)
{
	if(!IsValid())
		return -1;

	int index = 0;
	PIP_ADAPTER_INFO pAdapInfo = pAdapterInfo;
	while(pAdapInfo)
	{
		pADAPTERINFO[index].InstanceId = pAdapInfo->AdapterName;
		pADAPTERINFO[index].Description = pAdapInfo->Description;
		for(int i=0; i < (int)pAdapInfo->AddressLength; i++)
			pADAPTERINFO[index].MAC.Format(_T("%s%02X "),
			CString(pADAPTERINFO[index].MAC),(int)pAdapInfo->Address[i]);
		pADAPTERINFO[index].MAC.Trim();

		index++;
		pAdapInfo = pAdapInfo->Next;
	}

	return 0;
}

bool CodeProjectUtils::EnableConnection(GUID guidId, bool bEnable) 
{ 
	bool ret = false; 
	typedef void (__stdcall *LPNCFREENETCONPROPERTIES) (NETCON_PROPERTIES* pProps); 
	HMODULE hNSModule = LoadLibrary(_T("netshell.dll")); 
	if (hNSModule) 
	{		
		LPNCFREENETCONPROPERTIES NcFreeNetconProperties = 
			(LPNCFREENETCONPROPERTIES)GetProcAddress(hNSModule, "NcFreeNetconProperties"); 
		if (NcFreeNetconProperties) 
		{
			CoInitialize(0); 

			INetConnectionManager* pConnMan = 0; 
			HRESULT hr = CoCreateInstance(CLSID_ConnectionManager, 0,CLSCTX_ALL, 
				__uuidof(INetConnectionManager), (void**)&pConnMan); 
			if(SUCCEEDED(hr)) 
			{ 
				IEnumNetConnection* pEnum = 0; 
				HRESULT hr = pConnMan->EnumConnections(NCME_DEFAULT, &pEnum); 
				if(SUCCEEDED(hr)) 
				{ 
					INetConnection* pCon = 0; 
					ULONG count; 
					bool bDone = false; 
					while (pEnum->Next(1, &pCon, &count) == S_OK && !bDone) 
					{ 
						NETCON_PROPERTIES* pProps = 0; 
						hr = pCon->GetProperties(&pProps); 
						if(SUCCEEDED(hr)) 
						{ 		
							if(IsEqualGUID(guidId, pProps->guidId) == TRUE )
							{ 
								int nRetry = 3;

								if (bEnable) 
								{									
									do
									{
										ret = ((hr = pCon->Connect()) == S_OK);											
									}
									while(!ret && --nRetry && (ErrorCOMToWin32(hr) == ERROR_RETRY));
								}
								else 
								{									
									do
									{
										ret = ((hr = pCon->Disconnect()) == S_OK);										
									}
									while(!ret && --nRetry && (ErrorCOMToWin32(hr) == ERROR_RETRY));
								}							

								bDone = true; 
							} 
							NcFreeNetconProperties(pProps); 
						} 
						pCon->Release(); 
					} 
					pEnum->Release(); 
				} 
				pConnMan->Release(); 				
			} 			
			CoUninitialize(); 
		}		
		FreeLibrary(hNSModule); 
	}
	return ret; 
}

bool CodeProjectUtils::UpdateRegistry(CString strNetCfgInstanceId, LPCTSTR lpszMacID /*= NULL*/)
{
	bool bRet = false;
	HKEY hKey = NULL;			
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002bE10318}"),
		0,KEY_ALL_ACCESS,&hKey) == ERROR_SUCCESS)
	{
		DWORD dwIndex = 0;
		TCHAR Name[1024];
		DWORD cName = 1024;
		while( RegEnumKeyEx(hKey, dwIndex, Name, &cName, 
			NULL, NULL, NULL, NULL) == ERROR_SUCCESS )
		{
			HKEY hSubKey = NULL;
			if(RegOpenKeyEx(hKey,Name,0,KEY_ALL_ACCESS,&hSubKey) == ERROR_SUCCESS)
			{
				BYTE Data[1204];
				DWORD cbData = 1024;
				if(RegQueryValueEx(hSubKey,_T("NetCfgInstanceId"),NULL,NULL,Data,&cbData) == ERROR_SUCCESS)
				{
					if(_tcscmp((TCHAR*)Data,strNetCfgInstanceId) == 0)
					{
						if(lpszMacID == NULL)
						{
							//Delete the NetCfgInstanceId entry
							LONG nErr = RegDeleteValue(hSubKey, _T("NetworkAddress"));
							if( (nErr == ERROR_SUCCESS) || (nErr == ERROR_FILE_NOT_FOUND) )
							{
								bRet = true;
							}							
						}
						else
						{
							//Add the NetCfgInstanceId entry
							if(RegSetValueEx(hSubKey,_T("NetworkAddress"),0,REG_SZ,(const BYTE*)lpszMacID,sizeof(TCHAR) * ((DWORD)_tcslen(lpszMacID) + 1)) == ERROR_SUCCESS)
							{
								bRet = true;
							}							
						}												
					}
				}
				RegCloseKey(hSubKey);
			}					
			cName = 1024;
			dwIndex++;
		}
		RegCloseKey(hKey);
	}
	return bRet;
}

bool CodeProjectUtils::Reset(ADAPTERINFO* pAdInfo)
{
	GUID guidId;
	CString guidstr = pAdInfo->InstanceId;
	BSTR bstr = guidstr.Trim(_T("{}")).AllocSysString();			
	UuidFromStringW((unsigned short*)bstr,&guidId);
	SysFreeString(bstr);

	return(EnableConnection(guidId,false) && EnableConnection(guidId,true));
}

DWORD CodeProjectUtils::ErrorCOMToWin32(HRESULT hr)
{
	HRESULT hrfac = HRESULT_FACILITY(hr);
	if( (hrfac == FACILITY_WINDOWS) || (hrfac == FACILITY_WIN32) ) 
		return HRESULT_CODE(hr);
	else
		return hr;
}