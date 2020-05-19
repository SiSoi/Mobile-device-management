
// Lab3Dlg.h : header file
//

#pragma once

#define WM_MOBILE	WM_USER+1
#define WM_INTERNET	WM_USER+2
#define WM_PRINTPDF	WM_USER+3

// CLab3Dlg dialog
class CLab3Dlg : public CDialogEx
{
// Construction
public:
	CLab3Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAB3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRMobile();
	afx_msg void OnBnClickedRInternet();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBPrint();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnMobile(WPARAM, LPARAM);
	afx_msg LRESULT OnInternet(WPARAM, LPARAM);
	afx_msg LRESULT OnPrintPDF(WPARAM, LPARAM);
	void TariffingMobile();
	void TariffingInternet();
	static UINT ThreadProcTarifMobile(LPVOID);
	static UINT ThreadProcTarifInternet(LPVOID);
	static UINT ThreadProcPrintPDF(LPVOID);
private:
	CEdit m_ePhone;
	CIPAddressCtrl m_IPAddress;
	CEdit m_eTarif;
	CEdit m_eStatus;
	UINT m_nMode;
	Mobile::Client m_curMobileClient;
	Mobile::Billing m_curMobileBilling;
	Internet::Client m_curInternetClient;
	Internet::Billing m_curInternetBilling;
};
