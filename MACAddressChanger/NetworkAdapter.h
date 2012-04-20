/*
	Copyright(C) Nishant Sivakumar, 2005. All rights reserved.
	http://blog.voidnish.com
*/

#pragma once

#pragma comment(lib,"Iphlpapi.lib")
#include <Iphlpapi.h>

#pragma comment(lib,"Rpcrt4.lib")

#include <netcon.h>

namespace CodeProjectUtils
{
	struct ADAPTERINFO
	{
		ADAPTERINFO()
		{
			InstanceId = Description = MAC = "";
		}
		CString InstanceId;
		CString Description;
		CString MAC;
		operator LPCTSTR()
		{
			return MAC;
		}
	};

	class CNetworkAdapterList
	{
	public:
		CNetworkAdapterList(void);
		~CNetworkAdapterList(void);
	private:
		PIP_ADAPTER_INFO pAdapterInfo;
		int m_count;
	public:
		bool IsValid(void);
		int GetCount(void);
		int GetAdapters(ADAPTERINFO* pADAPTERINFO);
	};

	bool EnableConnection(GUID guidId, bool bEnable);
	bool UpdateRegistry(CString strNetCfgInstanceId, LPCTSTR lpszMacID = NULL);
	bool Reset(ADAPTERINFO* pAdInfo);
	DWORD ErrorCOMToWin32(HRESULT hr);
}