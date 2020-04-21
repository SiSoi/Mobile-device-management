
// Lab2Dlg.h : header file
//

#pragma once

#define WM_COMPLETE WM_USER+1
#define WM_CHARTDRAW WM_USER+2

// CLab2Dlg dialog
class CLab2Dlg : public CDialogEx
{
// Construction
public:
	CLab2Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAB2_DIALOG };
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
private:
	CIPAddressCtrl m_IPAddress;
	CEdit m_eVariant;
	CEdit m_eBilling;
	CChartCtrl m_ChartCtrl;
	std::vector<UINT> m_vtInBps;
	std::vector<COleDateTime> m_vtInTime;
	std::vector<UINT> m_vtOutBps;
	std::vector<COleDateTime> m_vtOutTime;
protected:
	void CChartDraw();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnComplete(WPARAM, LPARAM);
	afx_msg LRESULT OnChartDraw(WPARAM, LPARAM);
	static UINT ThreadProcCSVBilling(LPVOID);
	static UINT ThreadProcCSVGraphing(LPVOID);
};
