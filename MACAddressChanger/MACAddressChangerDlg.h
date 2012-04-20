/*
	Copyright(C) Nishant Sivakumar, 2005. All rights reserved.
	http://blog.voidnish.com
*/

// MACAddressChangerDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "NetworkAdapter.h"
using namespace CodeProjectUtils;

#define BALLTIMEREVID 0x1000

// CMACAddressChangerDlg dialog
class CMACAddressChangerDlg : public CDialog
{
// Construction
public:
	CMACAddressChangerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MACADDRESSCHANGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_AdapterListCombo;
	ADAPTERINFO* m_pAdapters;
public:
	CEdit m_MACEdit;
	afx_msg void OnBnChangeMAC();
	afx_msg void OnBnResetMAC();
	int MessageBoxEx(UINT nIDPrompt, UINT nType = MB_OK, UINT nIDTitle = IDS_MSGBOXTITLE);
	bool Init(void);
	afx_msg void OnBnClickedCancel();
	// Check whether the entered MAC ID is valid
	bool VerifyMacID(CString strMac);
	afx_msg void OnEnSetfocusMACEdit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool SetCaption(int nID, UINT nResID);
	afx_msg void OnCbnSelchangeAdapterCombo();
};
