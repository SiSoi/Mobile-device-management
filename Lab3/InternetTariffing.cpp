#include "pch.h"
#include "InternetTariffing.h"

InternetTariffing::InternetTariffing()
{
	m_client = { _T(""), 0, 0 };
	m_billing = { 0, 0, 0 };
}

InternetTariffing::~InternetTariffing()
{
	m_client = { _T(""), 0, 0 };
	m_billing = { 0, 0, 0 };
}


void InternetTariffing::SearchClient(CString szRow)
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


void InternetTariffing::SetIPAddress(CString szIPAddress)
{
	m_client.szIPAddress = szIPAddress;
}

void InternetTariffing::SetInBytes(CString szInBytes)
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

void InternetTariffing::SetOutBytes(CString szOutBytes)
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

void InternetTariffing::SetBilling()
{
	m_billing.fSending = Internet::m_pricing.nPerMb * ((FLOAT)m_client.nInBytes * 8 / 1000000);
	m_billing.fReceiving = Internet::m_pricing.nPerMb * ((FLOAT)m_client.nOutBytes * 8 / 1000000);

	m_billing.fTotal = m_billing.fSending + m_billing.fReceiving;
}


void InternetTariffing::GetBilling(Internet::Client& client, Internet::Billing& billing)
{
	client.szIPAddress = m_client.szIPAddress;
	client.nInBytes = m_client.nInBytes;
	client.nOutBytes = m_client.nOutBytes;
	billing.fReceiving = m_billing.fReceiving;
	billing.fSending = m_billing.fSending;
	billing.fTotal = m_billing.fTotal;
}


CString InternetTariffing::GetBillingE()
{
	CString szBilling;
	szBilling.Format(_T("Billing for IP Address %s:\r\n"), m_client.szIPAddress);
	szBilling.Append(_T("Category\t\tMegabits\t\tRubles\r\n"));
	szBilling.AppendFormat(_T("Sending\t\t%.2f\t\t%.2f\r\n"), (FLOAT)m_client.nInBytes*8/1000000, m_billing.fSending);
	szBilling.AppendFormat(_T("Receiving\t\t%.2f\t\t%.2f\r\n"), (FLOAT)m_client.nOutBytes*8/1000000, m_billing.fReceiving);
	szBilling.AppendFormat(_T("Total\t\t\t\t%.2f\r\n"), m_billing.fTotal);
	return szBilling;
}
