/*
	Copyright(C) Nishant Sivakumar, 2005. All rights reserved.
	http://blog.voidnish.com
*/

// MACAddressChangerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MACAddressChanger.h"
#include "MACAddressChangerDlg.h"
#include "VersionInfo.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool SetCaption(CDialog* pDlg, int nID, UINT nResID)
{
	CString strTmp;
	if(strTmp.LoadString(nResID))
	{
		CWnd* pWnd = pDlg->GetDlgItem(nID);
		if(pWnd != NULL)
		{
			pWnd->SetWindowText(strTmp);
			return true;
		}
	}
	return false;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_TxtAbout;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ABOUT, m_TxtAbout);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMACAddressChangerDlg dialog




CMACAddressChangerDlg::CMACAddressChangerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMACAddressChangerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAdapters = NULL;
}

void CMACAddressChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_AdapterListCombo);
	DDX_Control(pDX, IDC_EDIT1, m_MACEdit);
}

BEGIN_MESSAGE_MAP(CMACAddressChangerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMACAddressChangerDlg::OnBnChangeMAC)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMACAddressChangerDlg::OnBnResetMAC)
	ON_BN_CLICKED(IDCANCEL, &CMACAddressChangerDlg::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_EDIT1, &CMACAddressChangerDlg::OnEnSetfocusMACEdit)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMACAddressChangerDlg::OnCbnSelchangeAdapterCombo)
END_MESSAGE_MAP()


// CMACAddressChangerDlg message handlers

BOOL CMACAddressChangerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CString strTmp;	
	strTmp.LoadString(IDS_PROGTITLE);
	SetWindowText(strTmp);

	SetCaption(IDC_BUTTON_RESET, IDS_BTNRESET);
	SetCaption(IDOK, IDS_BTNCHANGE);
	SetCaption(IDCANCEL, IDS_BTNEXIT);
	SetCaption(IDC_STATIC_CHOOSEADAPTER, IDS_STATICCHADAP);
	SetCaption(IDC_STATIC_MACADDR, IDS_STATICMACADDR);
	SetCaption(IDC_STATIC_TOPTEXT, IDS_STATICTOPTXT);

	m_MACEdit.LimitText(17);

	Init();

	return TRUE;
}

void CMACAddressChangerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMACAddressChangerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMACAddressChangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMACAddressChangerDlg::OnBnChangeMAC()
{
	if(m_AdapterListCombo.GetCount() > 0)
	{
		int index = m_AdapterListCombo.GetCurSel();
		if(index != CB_ERR )
		{
			ADAPTERINFO* pAdInfo = (ADAPTERINFO*)m_AdapterListCombo.GetItemDataPtr(index);
			
			CString strMacID;
			m_MACEdit.GetWindowText(strMacID);

			if(!VerifyMacID(strMacID))
			{
				MessageBoxEx(IDS_INVALIDMACERR, MB_OK | MB_ICONSTOP);
				m_MACEdit.SetSel(0, -1);
				m_MACEdit.SetFocus();
				return;
			}

			strMacID.Remove(_T(' '));
			if(UpdateRegistry(pAdInfo->InstanceId, strMacID))
			{
				if(Reset(pAdInfo))
				{
					//Success
					int oldSel = m_AdapterListCombo.GetCurSel();
					MessageBoxEx(IDS_MACCHANGESUC, MB_OK | MB_ICONINFORMATION);
					Init();
					if(oldSel != CB_ERR)
					{
						m_AdapterListCombo.SetCurSel(oldSel);
						OnCbnSelchangeAdapterCombo();
					}
				}
				else
				{
					//Adapter reset error, manual reset required
					MessageBoxEx(IDS_MACCHANGEHALFSUC, MB_OK | MB_ICONEXCLAMATION);
				}
			}
			else
			{
				//Registry update error
				MessageBoxEx(IDS_REGACCESSERR, MB_OK | MB_ICONSTOP);
			}
		}
	}
}

void CMACAddressChangerDlg::OnBnResetMAC()
{
	if(m_AdapterListCombo.GetCount() > 0)
	{
		int index = m_AdapterListCombo.GetCurSel();
		if(index != CB_ERR )
		{
			ADAPTERINFO* pAdInfo = (ADAPTERINFO*)m_AdapterListCombo.GetItemDataPtr(index);
			
			if(UpdateRegistry(pAdInfo->InstanceId)) //Delete NetCfgInstanceId entry
			{
				if(Reset(pAdInfo))
				{
					//Success
					int oldSel = m_AdapterListCombo.GetCurSel();
					MessageBoxEx(IDS_MACRESETSUC, MB_OK | MB_ICONINFORMATION);
					Init();
					if(oldSel != CB_ERR)
					{
						m_AdapterListCombo.SetCurSel(oldSel);
						OnCbnSelchangeAdapterCombo();
					}
				}
				else
				{
					//Adapter reset error, manual reset required
					MessageBoxEx(IDS_MACRESETHALFSUC, MB_OK | MB_ICONEXCLAMATION);
				}
			}
			else
			{
				//Registry update error
				MessageBoxEx(IDS_REGACCESSERR, MB_OK | MB_ICONSTOP);
			}
		}
	}
}

int CMACAddressChangerDlg::MessageBoxEx(UINT nIDPrompt, UINT nType /*= MB_OK*/, UINT nIDTitle /*= IDS_MSGBOXTITLE*/)
{
	CString strTitle;
	CString strPrompt;
	strTitle.LoadString(nIDTitle);
	strPrompt.LoadString(nIDPrompt);
	return MessageBox(strPrompt, strTitle, nType);
}

bool CMACAddressChangerDlg::Init(void)
{
	bool bRet = false;
	delete[] m_pAdapters;
	m_pAdapters = NULL;
	m_AdapterListCombo.ResetContent();
	CNetworkAdapterList nal;
	if(nal.IsValid())
	{
		int count = nal.GetCount();				
		if(count>0)
		{
			m_pAdapters = new ADAPTERINFO[count];
			nal.GetAdapters(m_pAdapters);
			for(int i=0; i<count; i++)
			{
				m_AdapterListCombo.SetItemDataPtr(
					m_AdapterListCombo.AddString(m_pAdapters[i].Description),(void*)&m_pAdapters[i]);				
			}	
			m_AdapterListCombo.SetCurSel(0);
			m_MACEdit.SetWindowText(m_pAdapters[0]);
			bRet = true;
		}
	}
	if(bRet == false)
	{
		MessageBoxEx(IDS_ADAPINFOERROR, MB_OK | MB_ICONSTOP);
		m_AdapterListCombo.EnableWindow(FALSE);
		m_MACEdit.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	return bRet;
}

void CMACAddressChangerDlg::OnBnClickedCancel()
{	
	/* if(MessageBoxEx(IDS_SUREEXIT, MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		delete[] m_pAdapters;
		OnCancel();
	} */	
	
	// Override this stupid box and exit :-P
	delete[] m_pAdapters;
	OnCancel();
}

// Check whether the entered MAC ID is valid
bool CMACAddressChangerDlg::VerifyMacID(CString strMac)
{
	bool bRet = false;
	strMac.MakeUpper().Remove(_T(' '));		
	if(strMac.GetLength() == 12)
	{			
		for(int i=0; i<12; i++)
		{
			if( !( ( (strMac[i] >= _T('0')) && (strMac[i] <= _T('9')) )
				|| ( (strMac[i] >= _T('A')) && (strMac[i] <= _T('F')) ) ) )
			{				
				break;
			}
			if(i==11)
				bRet = true;
		}		
	}
	return bRet;
}

void CMACAddressChangerDlg::OnEnSetfocusMACEdit()
{
	CString tmpStrTitle, tmpStrText;
	EDITBALLOONTIP ebt = {0};
	ebt.cbStruct = sizeof(ebt);
	ebt.ttiIcon = TTI_INFO;	
	if(tmpStrText.LoadString(IDS_BALLOONTIP))
	{
		ebt.pszText = CT2CW(tmpStrText);		
		if(tmpStrTitle.LoadString(IDS_BALLOONTITLE))
		{
			ebt.pszTitle =  CT2CW(tmpStrTitle);			
			if(Edit_ShowBalloonTip(m_MACEdit.GetSafeHwnd(), &ebt))
			{
				SetTimer(BALLTIMEREVID, 3000, NULL);
			}
		}
	}
}

void CMACAddressChangerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == BALLTIMEREVID )
	{		
		if(Edit_HideBalloonTip(m_MACEdit.GetSafeHwnd()))
		{
			KillTimer(BALLTIMEREVID);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

bool CMACAddressChangerDlg::SetCaption(int nID, UINT nResID)
{
	return ::SetCaption(this, nID, nResID);
}

void CMACAddressChangerDlg::OnCbnSelchangeAdapterCombo()
{
	int index = m_AdapterListCombo.GetCurSel();
	if(index != CB_ERR )
	{
		ADAPTERINFO* pAdInfo = (ADAPTERINFO*)m_AdapterListCombo.GetItemDataPtr(index);
		m_MACEdit.SetWindowText(pAdInfo->MAC);
	}
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strTmp;	
	strTmp.LoadString(IDS_ABTBOXTITLE);
	SetWindowText(strTmp);

	::SetCaption(this, IDOK, IDS_STROK);

	strTmp.LoadString(IDS_TXTVERSION);

	CVersionInfo vi;
	CString crlf = _T("\r\n");
	m_TxtAbout.SetWindowText(vi.GetValue(_T("ProductName")) + crlf
		+ strTmp + _T(" ") + vi.GetValue(_T("FileVersion")) + crlf
		+ vi.GetValue(_T("LegalCopyright")));

	return TRUE;
}
