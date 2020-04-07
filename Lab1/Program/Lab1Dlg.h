
// Lab1Dlg.h : header file
//

#pragma once

#define WM_COMPLETE WM_USER+1

// CLab1Dlg dialog
class CLab1Dlg : public CDialogEx
{
// Construction
public:
	CLab1Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAB1_DIALOG };
#endif

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
	CString m_szPhone;
	CEdit m_ePhone;
	CEdit m_eVariant;
	CEdit m_eBilling;
public:
	afx_msg void OnEnChangeEphone();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnComplete(WPARAM, LPARAM);
	static UINT FileProcessingThreadProc(LPVOID);
};
