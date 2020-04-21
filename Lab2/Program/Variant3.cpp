#include "pch.h"
#include "Variant3.h"

Variant3::Variant3()
{
	m_client = { _T(""), 0, 0 };
	m_billing = { 0, 0, 0 };
}

Variant3::~Variant3()
{
	m_client = { _T(""), 0, 0 };
	m_billing = { 0, 0, 0 };

	//m_processor = nullptr;
}

/*
Variant3* Variant3::GetInstance()
{
	if (m_processor == nullptr)
		m_processor = new Variant3();
	return m_processor;
}
*/

void Variant3::SearchClient(CString szRow)
{
	int curPos = 0;
	
	CString szCurrCell;
	for (int i = 0; i < 2; i++)
		szCurrCell = szRow.Tokenize(_T(","), curPos);

	CString szDetails = szRow.Mid(curPos);
	szDetails.TrimLeft(_T("\t "));

	curPos = szDetails.Find(m_client.szIPAddress, 0);
	if (!curPos)
	{
		while (szDetails.Find(_T(",")) != -1)
		{
			curPos = 0;
			szCurrCell = szDetails.Tokenize(_T(","), curPos);
			szDetails = szDetails.Mid(curPos);
		}
		szCurrCell = szDetails.TrimLeft(_T("\t "));
		SetOutBytes(szCurrCell);
	}
	else if (curPos > 0)
	{
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		szDetails = szDetails.Mid(curPos);
		szCurrCell = szDetails.TrimLeft(_T("\t "));
		SetInBytes(szCurrCell);
	}
}


void Variant3::SetIPAddress(CString szIPAddress)
{
	m_client.szIPAddress = szIPAddress;
}

void Variant3::SetInBytes(CString szInBytes)
{
	UINT nBytes;
	if (szInBytes.Find(_T('M')) == -1)
		_stscanf_s(szInBytes, _T("%u"), &nBytes);
	else
	{
		szInBytes.Remove(_T('M'));
		szInBytes.Trim(_T("\t "));
		FLOAT fMBytes;
		_stscanf_s(szInBytes, _T("%f"), &fMBytes);
		nBytes = (UINT)(fMBytes * 1000000);
		nBytes *= 1000000;
	}
	m_client.nInBytes += nBytes;
}

void Variant3::SetOutBytes(CString szOutBytes)
{
	UINT nBytes;
	if (szOutBytes.Find(_T('M')) == -1)
		_stscanf_s(szOutBytes, _T("%u"), &nBytes);
	else
	{
		szOutBytes.Remove(_T('M'));
		szOutBytes.Trim(_T("\t "));
		FLOAT fMBytes;
		_stscanf_s(szOutBytes, _T("%f"), &fMBytes);
		nBytes = (UINT)(fMBytes * 1000000);
	}
	m_client.nOutBytes += nBytes;
}

void Variant3::SetBilling()
{
	m_billing.fSending = m_pricing.nPerMb * ((FLOAT)m_client.nInBytes * 8 / 1000000);
	m_billing.fReceiving = m_pricing.nPerMb * ((FLOAT)m_client.nOutBytes * 8 / 1000000);

	m_billing.fTotal = m_billing.fSending + m_billing.fReceiving;
}

CString Variant3::GetBillingE()
{
	CString szBilling;
	szBilling.Format(_T("Billing for IP Address %s:\r\n"), m_client.szIPAddress);
	szBilling.Append(_T("Category\t\tMegabits\t\tRubles\r\n"));
	szBilling.AppendFormat(_T("Sending\t\t%.2f\t\t%.2f\r\n"), (FLOAT)m_client.nInBytes*8/1000000, m_billing.fSending);
	szBilling.AppendFormat(_T("Receiving\t\t%.2f\t\t%.2f\r\n"), (FLOAT)m_client.nOutBytes*8/1000000, m_billing.fReceiving);
	szBilling.AppendFormat(_T("Total\t\t\t\t%.2f\r\n"), m_billing.fTotal);
	return szBilling;
}


FLOAT Variant3::GetTotal()
{
	return m_billing.fTotal;
}


void Variant3::SearchGraph(CString szRow, std::vector<UINT>& vtInBps, std::vector<COleDateTime>& vtInTime,
							std::vector<UINT>& vtOutBps, std::vector<COleDateTime>& vtOutTime)
{
	int curPos = 0;

	CString szDetails = szRow;
	szDetails.TrimLeft(_T("\t "));

	curPos = szDetails.Find(m_client.szIPAddress, 0);
	if (!curPos)	//Sending
	{
		CString szCurrCell;

		//Jump to 'bps' column
		for (int i = 0; i < 3; i++)
			szCurrCell = szDetails.Tokenize(_T(","), curPos);
		//Get 'bps' value
		szCurrCell.TrimLeft(_T("\t "));
		//Convert 'bps' format: CString to UINT
		UINT nOutBps;
		if (szCurrCell.Find(_T('M')) == -1)
			_stscanf_s(szCurrCell, _T("%d"), &nOutBps);
		else
		{
			szCurrCell.Remove(_T('M'));
			szCurrCell.Trim(_T("\t "));
			FLOAT fOutBps;
			_stscanf_s(szCurrCell, _T("%f"), &fOutBps);
			nOutBps = (UINT)(fOutBps * 1000000);
		}
		//Push 'bps' to vector
		vtOutBps.push_back(nOutBps);

		//Jump to 'td' column
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		//Get 'td' value
		CString szDuration = szCurrCell.TrimLeft(_T("\t "));
		
		//Jump to 'ts' column
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		//Get 'ts'value
		szCurrCell.TrimLeft(_T("\t "));
		//Convert 'ts' format: CString to COleDateTime
		COleDateTime timeOut;
		CString szOutTime = szCurrCell.Left(szCurrCell.GetLength() - 4);
		szOutTime.Replace(_T('-'), _T('/'));
		timeOut.ParseDateTime(szOutTime);
		/*
			//Need to fix so that GetSecond() + 1 won't turn 60
			//szSecDecimal = szCurrCell.Right(3);
			//if (szSecDecimal >= _T("500"))
			//	timeIn.SetTime(timeIn.GetHour(), timeIn.GetMinute(), timeIn.GetSecond() + 1);
		*/
		//Push 'ts' to vector
		vtOutTime.push_back(timeOut);

		//Add a point if duration > 0
		if (szDuration != _T("0"))
		{
			//Push 'bps' to vector
			vtOutBps.push_back(nOutBps);
			//Jump to 'te' column
			szCurrCell = szDetails.Tokenize(_T(","), curPos);
			//Get 'te'value
			szCurrCell.TrimLeft(_T("\t "));
			//Convert 'te' format: CString to COleDateTime
			szOutTime = szCurrCell.Left(szCurrCell.GetLength() - 4);
			szOutTime.Replace(_T('-'), _T('/'));
			timeOut.ParseDateTime(szOutTime);
			/*
				//Need to fix so that GetSecond() + 1 won't turn 60
				//szSecDecimal = szCurrCell.Right(3);
				//if (szSecDecimal >= _T("500"))
				//	timeIn.SetTime(timeIn.GetHour(), timeIn.GetMinute(), timeIn.GetSecond() + 1);
			*/
			//Push 'te' to vector
			vtOutTime.push_back(timeOut);
		}

	}
	else if (curPos > 0) //Receiving
	{
		CString szCurrCell;

		//Jump to 'bps' column
		for (int i = 0; i < 2; i++)
			szCurrCell = szDetails.Tokenize(_T(","), curPos);
		//Get 'bps' value
		szCurrCell.TrimLeft(_T("\t "));
		//Convert 'bps' format: CString to UINT
		UINT nInBps;
		if (szCurrCell.Find(_T('M')) == -1)
			_stscanf_s(szCurrCell, _T("%d"), &nInBps);
		else
		{
			szCurrCell.Remove(_T('M'));
			szCurrCell.Trim(_T("\t "));
			FLOAT fInBps;
			_stscanf_s(szCurrCell, _T("%f"), &fInBps);
			nInBps = (UINT)(fInBps * 1000000);
		}
		//Push 'bps' to vector
		vtInBps.push_back(nInBps);

		//Jump to 'td' column
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		//Get 'td' value
		CString szDuration = szCurrCell.TrimLeft(_T("\t "));

		//Jump to 'ts' column
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		//Get 'ts'value
		szCurrCell.TrimLeft(_T("\t "));
		//Convert 'ts' format: CString to COleDateTime
		COleDateTime timeIn;
		CString szInTime = szCurrCell.Left(szCurrCell.GetLength() - 4);
		szInTime.Replace(_T('-'), _T('/'));
		timeIn.ParseDateTime(szInTime);
		/*
			//Need to fix so that GetSecond() + 1 won't turn 60
			//szSecDecimal = szCurrCell.Right(3);
			//if (szSecDecimal >= _T("500"))
			//	timeIn.SetTime(timeIn.GetHour(), timeIn.GetMinute(), timeIn.GetSecond() + 1);
		*/
		//Push 'ts' to vector
		vtInTime.push_back(timeIn);

		//Add a point if duration > 0
		if (szDuration != _T("0"))
		{
			//Push 'bps' to vector
			vtInBps.push_back(nInBps);
			//Jump to 'te' column
			szCurrCell = szDetails.Tokenize(_T(","), curPos);
			//Get 'te'value
			szCurrCell.TrimLeft(_T("\t "));
			//Convert 'te' format: CString to COleDateTime
			szInTime = szCurrCell.Left(szCurrCell.GetLength() - 4);
			szInTime.Replace(_T('-'), _T('/'));
			timeIn.ParseDateTime(szInTime);
			/*
			//Need to fix so that GetSecond() + 1 won't turn 60
			//szSecDecimal = szCurrCell.Right(3);
			//if (szSecDecimal >= _T("500"))
			//	timeIn.SetTime(timeIn.GetHour(), timeIn.GetMinute(), timeIn.GetSecond() + 1);
			*/
			//Push 'te' to vector
			vtInTime.push_back(timeIn);
		}
	}
}
