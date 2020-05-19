
// Lab3Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Lab3.h"
#include "Lab3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab3Dlg dialog



CLab3Dlg::CLab3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LAB3_DIALOG, pParent)
	, m_nMode(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLab3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EPHONE, m_ePhone);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_ETARIF, m_eTarif);
	DDX_Control(pDX, IDC_ESTATUS, m_eStatus);
}

BEGIN_MESSAGE_MAP(CLab3Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RMOBILE, &CLab3Dlg::OnBnClickedRMobile)
	ON_BN_CLICKED(IDC_RINTERNET, &CLab3Dlg::OnBnClickedRInternet)
	ON_BN_CLICKED(IDOK, &CLab3Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BPRINT, &CLab3Dlg::OnBnClickedBPrint)
	ON_BN_CLICKED(IDCANCEL, &CLab3Dlg::OnBnClickedCancel)
	ON_MESSAGE(WM_MOBILE, &CLab3Dlg::OnMobile)
	ON_MESSAGE(WM_INTERNET, &CLab3Dlg::OnInternet)
	ON_MESSAGE(WM_PRINTPDF, &CLab3Dlg::OnPrintPDF)
END_MESSAGE_MAP()


// CLab3Dlg message handlers

BOOL CLab3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_ePhone.EnableWindow(0);
	m_IPAddress.EnableWindow(0);
	GetDlgItem(IDOK)->EnableWindow(0);
	GetDlgItem(IDC_BPRINT)->EnableWindow(0);
	m_ePhone.SetWindowTextW(_T("915783624"));
	m_IPAddress.SetAddress(192, 168, 250, 27);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLab3Dlg::OnPaint()
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
HCURSOR CLab3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLab3Dlg::OnBnClickedRMobile()
{
	// TODO: Add your control notification handler code here
	m_ePhone.EnableWindow(1);
	m_IPAddress.EnableWindow(0);
	GetDlgItem(IDOK)->EnableWindow(1);
}


void CLab3Dlg::OnBnClickedRInternet()
{
	// TODO: Add your control notification handler code here
	m_ePhone.EnableWindow(0);
	m_IPAddress.EnableWindow(1);
	GetDlgItem(IDOK)->EnableWindow(1);
}


void CLab3Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	GetDlgItem(IDC_RMOBILE)->EnableWindow(0);
	GetDlgItem(IDC_RINTERNET)->EnableWindow(0);
	m_ePhone.EnableWindow(0);
	m_IPAddress.EnableWindow(0);
	GetDlgItem(IDOK)->EnableWindow(0);
	m_nMode = GetCheckedRadioButton(IDC_RMOBILE, IDC_RINTERNET);
	switch (m_nMode)
	{
	case IDC_RMOBILE:
		TariffingMobile();
		break;
	case IDC_RINTERNET:
		TariffingInternet();
		break;
	default:
		AfxMessageBox(_T("ERROR!\r\n"), MB_ICONSTOP);
		m_eStatus.SetWindowTextW(_T("Invalid option!\r\n"));
	}
}


void CLab3Dlg::OnBnClickedBPrint()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_RMOBILE)->EnableWindow(0);
	GetDlgItem(IDC_RINTERNET)->EnableWindow(0);
	m_ePhone.EnableWindow(0);
	m_IPAddress.EnableWindow(0);
	GetDlgItem(IDOK)->EnableWindow(0);
	GetDlgItem(IDC_BPRINT)->EnableWindow(0);
	AfxBeginThread(ThreadProcPrintPDF, this);
}


void CLab3Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


LRESULT CLab3Dlg::OnMobile(WPARAM wParam, LPARAM lParam)
{
	CString* szResult = (CString*)lParam;
	CString szTemp = _T("Mobile tariffing completed.\r\n");
	szTemp = szTemp + *szResult;
	m_eStatus.SetWindowTextW(szTemp);
	GetDlgItem(IDC_RMOBILE)->EnableWindow(1);
	GetDlgItem(IDC_RINTERNET)->EnableWindow(1);
	GetDlgItem(IDC_BPRINT)->EnableWindow(1);
	return 0;
}


LRESULT CLab3Dlg::OnInternet(WPARAM wParam, LPARAM lParam)
{
	CString* szResult = (CString*)lParam;
	CString szTemp = _T("Internet tariffing completed.\r\n");
	szTemp = szTemp + *szResult;
	m_eStatus.SetWindowTextW(szTemp);
	GetDlgItem(IDC_RMOBILE)->EnableWindow(1);
	GetDlgItem(IDC_RINTERNET)->EnableWindow(1);
	GetDlgItem(IDC_BPRINT)->EnableWindow(1);
	return 0;
}


LRESULT CLab3Dlg::OnPrintPDF(WPARAM wParam, LPARAM lParam)
{
	CString* szResult = (CString*)lParam;
	CString szTemp = *szResult;
	m_eStatus.SetWindowTextW(szTemp);
	GetDlgItem(IDC_RMOBILE)->EnableWindow(1);
	GetDlgItem(IDC_RINTERNET)->EnableWindow(1);
	GetDlgItem(IDC_BPRINT)->EnableWindow(0);
	return 0;
}


void CLab3Dlg::TariffingMobile()
{
	CString szPhone;
	m_ePhone.GetWindowTextW(szPhone);
	if (szPhone.GetLength() != 9)
	{
		AfxMessageBox(_T("ERROR!"), MB_ICONSTOP);
		m_eStatus.SetWindowTextW(_T("Invalid phone number (length)!\r\n"));
		GetDlgItem(IDC_RMOBILE)->EnableWindow(1);
		GetDlgItem(IDC_RINTERNET)->EnableWindow(1);
	}
	else
	{
		m_eTarif.SetWindowTextW(_T("VARIANT 3:\r\nCalls: Outgoing - 2 RUB/min (free 20 mins) | Incoming - 0 RUB/min\r\nSMS: 2 RUB/msg"));
		AfxBeginThread(ThreadProcTarifMobile, this);
	}
}

void CLab3Dlg::TariffingInternet()
{
	m_eTarif.SetWindowTextW(_T("VARIANT 3 - Internet: 1 RUB/Mbit"));
	AfxBeginThread(ThreadProcTarifInternet, this);
}

UINT CLab3Dlg::ThreadProcTarifMobile(LPVOID pParam)
{
	CLab3Dlg* currWnd = reinterpret_cast<CLab3Dlg*>(pParam);
	CSVProcessor inProcessor(_T("res\\data.csv"));
	inProcessor.CSVRow2Vector();
	MobileTariffing outProcessor;
	CString szPhone;
	currWnd->m_ePhone.GetWindowTextW(szPhone);
	outProcessor.SetPhone(szPhone);
	for (int iRow = 1; iRow < (int)inProcessor.m_CSVRow.size(); iRow++)
		outProcessor.SearchClient(inProcessor.m_CSVRow[iRow]);
	outProcessor.SetBilling();
	outProcessor.GetBilling(currWnd->m_curMobileClient, currWnd->m_curMobileBilling);
	CString* szBilling = new CString(outProcessor.GetBillingE());
	currWnd->PostMessageW(WM_MOBILE, 0, (LPARAM)(LPCTSTR)szBilling);
	return 0;
}

UINT CLab3Dlg::ThreadProcTarifInternet(LPVOID pParam)
{
	CLab3Dlg* currWnd = reinterpret_cast<CLab3Dlg*>(pParam);

	BYTE ipA, ipB, ipC, ipD;
	CString szIPAddress;
	currWnd->m_IPAddress.GetAddress(ipA, ipB, ipC, ipD);
	szIPAddress.Format(_T("%u.%u.%u.%u"), ipA, ipB, ipC, ipD);

	CSVProcessor inProcessor(_T("res\\nfcapd_202002251200_byt.csv"));
	inProcessor.CSVRow2Vector();

	InternetTariffing outProcessor;
	outProcessor.SetIPAddress(szIPAddress);
	for (int iRow = 1; iRow < (int)inProcessor.m_CSVRow.size(); iRow++)
		outProcessor.SearchClient(inProcessor.m_CSVRow[iRow]);
	outProcessor.SetBilling();
	outProcessor.GetBilling(currWnd->m_curInternetClient, currWnd->m_curInternetBilling);
	CString* szBilling = new CString(outProcessor.GetBillingE());
	currWnd->PostMessageW(WM_INTERNET, 0, (LPARAM)(LPCTSTR)szBilling);
	return 0;
}


UINT CLab3Dlg::ThreadProcPrintPDF(LPVOID pParam)
{
	CLab3Dlg* currWnd = reinterpret_cast<CLab3Dlg*>(pParam);

	using namespace std;
	using namespace PDFHummus;

	PDFWriter pdfWriter;
	EStatusCode eStatCode;

	do
	{
		// Initialise font, text, graphic and image styles		
		PDFUsedFont* TimesNewRomanTTF = pdfWriter.GetFontForFile("C:/windows/fonts/times.ttf");
		if (!TimesNewRomanTTF)
		{
			eStatCode = eFailure;
			break;
		}
		PDFUsedFont* TimesNewRomanBoldTTF = pdfWriter.GetFontForFile("C:/windows/fonts/timesbd.ttf");
		if (!TimesNewRomanTTF)
		{
			eStatCode = eFailure;
			break;
		}
		AbstractContentContext::TextOptions BodyTextOpt(TimesNewRomanTTF, 10, AbstractContentContext::eGray, 0);
		AbstractContentContext::TextOptions NoteTextOpt(TimesNewRomanTTF, 8, AbstractContentContext::eGray, 0);
		AbstractContentContext::TextOptions TitleBoldTextOpt(TimesNewRomanBoldTTF, 16, AbstractContentContext::eGray, 0);
		AbstractContentContext::TextOptions BodyBoldTextOpt(TimesNewRomanBoldTTF, 10, AbstractContentContext::eGray, 0);

		AbstractContentContext::GraphicOptions OutlineOpt(AbstractContentContext::eStroke, AbstractContentContext::eGray, 0, 0.5, false);
		AbstractContentContext::GraphicOptions OutlineBoldOpt(AbstractContentContext::eStroke, AbstractContentContext::eGray, 0, 1.5, false);

		AbstractContentContext::ImageOptions ImgSignatureOpt;
		ImgSignatureOpt.transformationMethod = AbstractContentContext::eFit;
		ImgSignatureOpt.boundingBoxHeight = 36.5;
		ImgSignatureOpt.boundingBoxWidth = 100;

		// Start a PDF file
		string szNamePDF;
		if (currWnd->m_nMode == IDC_RMOBILE)
			szNamePDF = "res/MobileBilling.pdf";
		else
			szNamePDF = "res/InternetBilling.pdf";
		eStatCode = pdfWriter.StartPDF(szNamePDF, ePDFVersion15);
		if (eStatCode != eSuccess)
			break;

		// Create a new page
		PDFPage* pdfPage = new PDFPage();
		pdfPage->SetMediaBox(PDFRectangle(0, 0, 595, 842));

		// Create a content context for the page
		PageContentContext* pageContentContext = pdfWriter.StartPageContentContext(pdfPage);

		// Draw tables
		//Banking table
		pageContentContext->DrawRectangle(35, 720, 480, 80, OutlineOpt);
		pageContentContext->DrawRectangle(35, 752, 265, 0, OutlineOpt);
		pageContentContext->DrawRectangle(35, 765, 480, 0, OutlineOpt);
		pageContentContext->DrawRectangle(300, 787, 50, 0, OutlineOpt);
		pageContentContext->DrawRectangle(350, 720, 0, 80, OutlineOpt);
		pageContentContext->DrawRectangle(300, 720, 0, 80, OutlineOpt);
		pageContentContext->DrawRectangle(175, 752, 0, 13, OutlineOpt);
		//Separating line
		pageContentContext->DrawRectangle(35, 670, 480, 0, OutlineBoldOpt);
		//Billing table
		pageContentContext->DrawRectangle(35, 515, 480, 55, OutlineBoldOpt);
		pageContentContext->DrawRectangle(35, 555, 480, 0, OutlineOpt);
		pageContentContext->DrawRectangle(57, 515, 0, 55, OutlineOpt);
		pageContentContext->DrawRectangle(305, 515, 0, 55, OutlineOpt);
		pageContentContext->DrawRectangle(345, 515, 0, 55, OutlineOpt);
		pageContentContext->DrawRectangle(378, 515, 0, 55, OutlineOpt);
		pageContentContext->DrawRectangle(440, 515, 0, 55, OutlineOpt);
		//Separating line
		pageContentContext->DrawRectangle(35, 360, 480, 0, OutlineBoldOpt);
		//Signature blanks
		pageContentContext->DrawRectangle(135, 325, 185, 0, OutlineOpt);
		pageContentContext->DrawRectangle(378, 325, 137, 0, OutlineOpt);

		// Insert texts
		//Fill in banking table		
		pageContentContext->WriteText(36, 791, u8"АО \"Стоун банк\" Г. МОСКВА", BodyTextOpt);
		pageContentContext->WriteText(36, 768, u8"Банк получателя", NoteTextOpt);
		pageContentContext->WriteText(36, 755, u8"ИНН   7722737766", BodyTextOpt);
		pageContentContext->WriteText(176, 755, u8"КПП   772201001", BodyTextOpt);
		pageContentContext->WriteText(36, 743, u8"ПАО\"Василек\"", BodyTextOpt);
		pageContentContext->WriteText(36, 723, u8"Получатель", NoteTextOpt);
		pageContentContext->WriteText(301, 791, u8"БИК", BodyTextOpt);
		pageContentContext->WriteText(301, 778, u8"Сч. №", BodyTextOpt);
		pageContentContext->WriteText(301, 755, u8"Сч. №", BodyTextOpt);
		pageContentContext->WriteText(351, 791, u8"044525700", BodyTextOpt);
		pageContentContext->WriteText(351, 778, u8"30101810200000000700", BodyTextOpt);
		pageContentContext->WriteText(351, 755, u8"40702810900000002453", BodyTextOpt);
		//Title
		pageContentContext->WriteText(36, 690, u8"Счет на оплату № 82 от 01 июля 2016 г.", TitleBoldTextOpt);
		//Billing description
		pageContentContext->WriteText(36, 655, u8"Провайдер:", BodyTextOpt);
		pageContentContext->WriteText(91, 655, u8"ПАО\"ВАСИЛЕК\", ИНН 7722737753, КПП 773301001, 109052, Москва г.", BodyBoldTextOpt);
		pageContentContext->WriteText(91, 645, u8"ДОБРЫНИНСКАЯ ул., дом № 70, корпус 2", BodyBoldTextOpt);
		pageContentContext->WriteText(36, 620, u8"Клиент:", BodyTextOpt);
		pageContentContext->WriteText(36, 585, u8"Договор:", BodyTextOpt);
		pageContentContext->WriteText(91, 585, u8"№ 20022016 от 12.02.2016", BodyBoldTextOpt);
		//Fill in billing table
		pageContentContext->WriteText(40, 558, u8"№", BodyBoldTextOpt);
		pageContentContext->WriteText(160, 558, u8"Услуги", BodyBoldTextOpt);
		pageContentContext->WriteText(310, 558, u8"Кол-во", BodyBoldTextOpt);
		pageContentContext->WriteText(355, 558, u8"Ед.", BodyBoldTextOpt);
		pageContentContext->WriteText(400, 558, u8"Цена", BodyBoldTextOpt);
		pageContentContext->WriteText(460, 558, u8"Сумма", BodyBoldTextOpt);
		//Total and notes
		pageContentContext->WriteText(410, 500, u8"Итого:", BodyBoldTextOpt);
		pageContentContext->WriteText(359, 485, u8"В том числе НДС:", BodyBoldTextOpt);
		pageContentContext->WriteText(371, 470, u8"Всего к оплате:", BodyBoldTextOpt);
		pageContentContext->WriteText(36, 393, u8"Внимание!", NoteTextOpt);
		pageContentContext->WriteText(36, 383, u8"Для корректного и своевременного зачисления платежа", NoteTextOpt);
		pageContentContext->WriteText(36, 373, u8"в назначении платежного документа необходимо указывать:", NoteTextOpt);
		pageContentContext->WriteText(36, 363, u8"Оплата за услуги связи по договору 20022016 от 12.02.2016", NoteTextOpt);
		//Signatures
		pageContentContext->WriteText(36, 340, u8"Директор отдела", BodyBoldTextOpt);
		pageContentContext->WriteText(36, 328, u8"\"Забота о клиентах\"", BodyBoldTextOpt);
		pageContentContext->WriteText(273, 328, u8"Семенов Д.А.", NoteTextOpt);
		pageContentContext->WriteText(335, 328, u8"Клиент", BodyBoldTextOpt);
		//Draw a signature
		pageContentContext->DrawImage(160, 323, "res/signature.png", ImgSignatureOpt);
		
		//Insert results
		if (currWnd->m_nMode == IDC_RMOBILE)
		{
			//Write details on customer
			CString cszTemp = currWnd->m_curMobileClient.szPhone;
			string szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(91, 620, szTemp, BodyBoldTextOpt);
			//Write details on incoming calls
			pageContentContext->WriteText(43, 548, "1", NoteTextOpt);
			pageContentContext->WriteText(60, 548, u8"Входящие звонки", NoteTextOpt);
			cszTemp.Format(_T("%.2f"), currWnd->m_curMobileClient.fInDur);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(315, 548, szTemp, NoteTextOpt);
			pageContentContext->WriteText(353, 548, u8"мин.", NoteTextOpt);
			szTemp = to_string(Mobile::m_pricing.nInPerMin);
			pageContentContext->WriteText(405, 548, szTemp, NoteTextOpt);
			cszTemp.Format(_T("%.2f"), currWnd->m_curMobileBilling.fIncoming);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(470, 548, szTemp, NoteTextOpt);
			//Write details on outgoing calls
			pageContentContext->WriteText(43, 538, "2", NoteTextOpt);
			pageContentContext->WriteText(60, 538, u8"Исходящие звонки", NoteTextOpt);
			pageContentContext->WriteText(90, 528, u8"Безплатно", NoteTextOpt);
			cszTemp.Format(_T("%.2f"), currWnd->m_curMobileClient.fOutDur);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(315, 538, szTemp, NoteTextOpt);
			szTemp = to_string(Mobile::m_pricing.nFreeOutDur);
			pageContentContext->WriteText(315, 528, szTemp, NoteTextOpt);
			pageContentContext->WriteText(353, 538, u8"мин.", NoteTextOpt);
			szTemp = to_string(Mobile::m_pricing.nOutPerMin);
			pageContentContext->WriteText(405, 538, szTemp, NoteTextOpt);
			cszTemp.Format(_T("%.2f"), currWnd->m_curMobileBilling.fOutgoing);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(467, 538, szTemp, NoteTextOpt);
			//Write details on SMS
			pageContentContext->WriteText(43, 518, "3", NoteTextOpt);
			pageContentContext->WriteText(60, 518, u8"СМС", NoteTextOpt);
			szTemp = to_string(currWnd->m_curMobileClient.nMsgQty);
			pageContentContext->WriteText(315, 518, szTemp, NoteTextOpt);
			pageContentContext->WriteText(353, 518, u8"шт.", NoteTextOpt);
			szTemp = to_string(Mobile::m_pricing.nSMSPerMsg);
			pageContentContext->WriteText(405, 518, szTemp, NoteTextOpt);
			cszTemp.Format(_T("%.2f"), currWnd->m_curMobileBilling.fSMS);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(467, 518, szTemp, NoteTextOpt);
			//Write total
			cszTemp.Format(_T("%.2f"), currWnd->m_curMobileBilling.fTotal);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(467, 500, szTemp, BodyBoldTextOpt);
			pageContentContext->WriteText(467, 470, szTemp, BodyBoldTextOpt);
		}
		else
		{
			//Write details on customer
			CString cszTemp = currWnd->m_curInternetClient.szIPAddress;
			string szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(91, 620, szTemp, BodyBoldTextOpt);
			//Write details on receiving
			pageContentContext->WriteText(43, 548, "1", NoteTextOpt);
			pageContentContext->WriteText(60, 548, u8"Входящий объём", NoteTextOpt);
			cszTemp.Format(_T("%.2f"), (FLOAT)currWnd->m_curInternetClient.nInBytes*8/1000000);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(315, 548, szTemp, NoteTextOpt);
			pageContentContext->WriteText(353, 548, u8"Мбит", NoteTextOpt);
			szTemp = to_string(Internet::m_pricing.nPerMb);
			pageContentContext->WriteText(405, 548, szTemp, NoteTextOpt);
			cszTemp.Format(_T("%.2f"), currWnd->m_curInternetBilling.fReceiving);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(470, 548, szTemp, NoteTextOpt);
			//Write details on sending
			pageContentContext->WriteText(43, 538, "2", NoteTextOpt);
			pageContentContext->WriteText(60, 538, u8"Исходящий объём", NoteTextOpt);
			cszTemp.Format(_T("%.2f"), (FLOAT)currWnd->m_curInternetClient.nOutBytes * 8 / 1000000);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(315, 538, szTemp, NoteTextOpt);
			pageContentContext->WriteText(353, 538, u8"Мбит", NoteTextOpt);
			szTemp = to_string(Internet::m_pricing.nPerMb);
			pageContentContext->WriteText(405, 538, szTemp, NoteTextOpt);
			cszTemp.Format(_T("%.2f"), currWnd->m_curInternetBilling.fSending);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(470, 538, szTemp, NoteTextOpt);
			//Write total
			cszTemp.Format(_T("%.2f"), currWnd->m_curInternetBilling.fTotal);
			szTemp = (CT2CA)cszTemp;
			pageContentContext->WriteText(467, 500, szTemp, BodyBoldTextOpt);
			pageContentContext->WriteText(467, 470, szTemp, BodyBoldTextOpt);
		}
		
		// End content context, and write the page
		eStatCode = pdfWriter.EndPageContentContext(pageContentContext);
		if (eStatCode != eSuccess)
			break;
		eStatCode = pdfWriter.WritePageAndRelease(pdfPage);
		if (eStatCode != eSuccess)
			break;
		eStatCode = pdfWriter.EndPDF();
	} while (false);

	// Inform the result
	CString* szStatus;
	if (eStatCode == eSuccess)
		szStatus = new CString(_T("Succeeded in creating PDF file\r\n"));
	else
	{
		AfxMessageBox(_T("ERROR!"));
		szStatus = new CString(_T("Failed in creating PDF file\r\n"));
	}
	currWnd->PostMessageW(WM_PRINTPDF, 0, (LPARAM)(LPCTSTR)szStatus);
	return 0;
}