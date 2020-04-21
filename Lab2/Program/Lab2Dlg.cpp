
// Lab2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Lab2.h"
#include "Lab2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab2Dlg dialog



CLab2Dlg::CLab2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LAB2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLab2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_EVARIANT, m_eVariant);
	DDX_Control(pDX, IDC_EBILLING, m_eBilling);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
}

BEGIN_MESSAGE_MAP(CLab2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLab2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLab2Dlg::OnBnClickedCancel)
	ON_MESSAGE(WM_COMPLETE, &CLab2Dlg::OnComplete)
	ON_MESSAGE(WM_CHARTDRAW, &CLab2Dlg::OnChartDraw)
END_MESSAGE_MAP()


// CLab2Dlg message handlers

BOOL CLab2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_IPAddress.SetAddress(192, 168, 250, 27);
	m_eVariant.SetWindowTextW(_T("VARIANT 3 - Internet: 1 RUB/Mbit"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLab2Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLab2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLab2Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	m_IPAddress.EnableWindow(FALSE);
	AfxBeginThread(ThreadProcCSVBilling, this);
}


void CLab2Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_IPAddress.SetAddress(0, 0, 0, 0);
	m_eVariant.SetWindowTextW(_T(""));
	m_eBilling.SetWindowTextW(_T(""));
	CDialogEx::OnCancel();
}


LRESULT CLab2Dlg::OnComplete(WPARAM wParam, LPARAM lParam)
{
	CString* szResult = (CString*)lParam;
	CString& szTemp = *szResult;
	m_eBilling.SetWindowTextW(szTemp);
	if (wParam == 1)
		AfxBeginThread(ThreadProcCSVGraphing, this);
	else
		m_ChartCtrl.RemoveAllSeries();
	m_IPAddress.EnableWindow(TRUE);
	return 0;
}


LRESULT CLab2Dlg::OnChartDraw(WPARAM wParam, LPARAM lParam)
{
	// Disable the refresh
	m_ChartCtrl.EnableRefresh(false);

	// Create the bottom axis and configure it properly
	CChartDateTimeAxis* pBottomAxis = m_ChartCtrl.CreateDateTimeAxis(CChartCtrl::BottomAxis);
	COleDateTime timeMin(2020, 2, 25, 8, 20, 0);
	COleDateTime timeMax(2020, 2, 25, 9, 30, 0);
	pBottomAxis->SetMinMax(timeMin, timeMax);
	pBottomAxis->SetDiscrete(false);
	pBottomAxis->SetTickIncrement(false, CChartDateTimeAxis::tiMinute, 10);

	// Create the left axis and configure it properly
	CChartStandardAxis* pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	UINT bpsInMax = *std::max_element(m_vtInBps.begin(), m_vtInBps.end());
	UINT bpsOutMax = *std::max_element(m_vtOutBps.begin(), m_vtOutBps.end());
	if (bpsInMax >= bpsOutMax)
		pLeftAxis->SetMinMax(0, bpsInMax);
	else
		pLeftAxis->SetMinMax(0, bpsOutMax);

	// Configure the legend
	m_ChartCtrl.GetLegend()->SetVisible(true);
	m_ChartCtrl.GetLegend()->SetHorizontalMode(true);
	m_ChartCtrl.GetLegend()->UndockLegend(90, 50);

	// Add text to the title and set the font & color
	CChartFont titleFont;
	titleFont.SetFont(_T("MS Shell Dlg"), 120, false, false, false);
	m_ChartCtrl.GetTitle()->AddString(_T("Network traffic"));
	m_ChartCtrl.GetTitle()->SetFont(titleFont);
	m_ChartCtrl.GetTitle()->SetColor(RGB(0, 0, 0));
	pLeftAxis->GetLabel()->SetText(_T("bps"));

	// Sets a gradient background
	m_ChartCtrl.SetBackColor(RGB(255, 255, 255));
	
	// Create two line series and populate them with data
	CChartLineSerie* pLineInKbps = m_ChartCtrl.CreateLineSerie();
	for (int i = 0; i < (int)m_vtInBps.size(); i++)
		pLineInKbps->AddPoint(m_vtInTime[i], m_vtInBps[i]);
	CChartLineSerie* pLineOutKbps = m_ChartCtrl.CreateLineSerie();
	for (int i = 0; i < (int)m_vtOutBps.size(); i++)
		pLineOutKbps->AddPoint(m_vtOutTime[i], m_vtOutBps[i]);

	// Configure the series properly
	pLineInKbps->SetColor(RGB(255, 0, 0));
	pLineInKbps->SetName(_T("Receiving"));
	pLineInKbps->SetWidth(2);
	pLineOutKbps->SetColor(RGB(0, 0, 255));
	pLineOutKbps->SetName(_T("Sending"));
	pLineOutKbps->SetWidth(2);

	// Re enable the refresh
	m_ChartCtrl.EnableRefresh(true);
	return 0;
}


UINT CLab2Dlg::ThreadProcCSVBilling(LPVOID pParam)
{
	CLab2Dlg* currWnd = reinterpret_cast<CLab2Dlg*>(pParam);
	
	BYTE ipA, ipB, ipC, ipD;
	CString szIPAddress;
	currWnd->m_IPAddress.GetAddress(ipA, ipB, ipC, ipD);
	szIPAddress.Format(_T("%u.%u.%u.%u"), ipA, ipB, ipC, ipD);

	CSVProcessor inProcessor(_T("res\\nfcapd_202002251200_byt.csv"));
	inProcessor.CSVRow2Vector();
	
	Variant3 outProcessor;
	outProcessor.SetIPAddress(szIPAddress);
	for (int iRow = 1; iRow < (int)inProcessor.m_CSVRow.size(); iRow++)
		outProcessor.SearchClient(inProcessor.m_CSVRow[iRow]);
	outProcessor.SetBilling();
	CString* szBilling = new CString(outProcessor.GetBillingE());
	FLOAT fTotal = outProcessor.GetTotal();
	if (fTotal == 0)
		currWnd->PostMessageW(WM_COMPLETE, 0, (LPARAM)(LPCTSTR)szBilling);
	else
		currWnd->PostMessageW(WM_COMPLETE, 1, (LPARAM)(LPCTSTR)szBilling);
	return 0;
}


UINT CLab2Dlg::ThreadProcCSVGraphing(LPVOID pParam)
{
	CLab2Dlg* currWnd = reinterpret_cast<CLab2Dlg*>(pParam);
	
	
	BYTE ipA, ipB, ipC, ipD;
	CString szIPAddress;
	currWnd->m_IPAddress.GetAddress(ipA, ipB, ipC, ipD);
	szIPAddress.Format(_T("%u.%u.%u.%u"), ipA, ipB, ipC, ipD);

	CSVProcessor inProcessor(_T("res\\nfcapd_202002251200_all-bps.csv"));
	inProcessor.CSVRow2Vector();

	Variant3 outProcessor;
	outProcessor.SetIPAddress(szIPAddress);
	for (int iRow = 1; iRow < (int)inProcessor.m_CSVRow.size(); iRow++)
		outProcessor.SearchGraph(inProcessor.m_CSVRow[iRow], currWnd->m_vtInBps, currWnd->m_vtInTime,
									currWnd->m_vtOutBps, currWnd->m_vtOutTime);
	

	currWnd->PostMessageW(WM_CHARTDRAW, 0, 0);
	return 0;
}

