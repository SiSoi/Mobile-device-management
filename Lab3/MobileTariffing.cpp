#include "pch.h"
#include "MobileTariffing.h"

MobileTariffing::MobileTariffing()
{
	m_client = { _T(""), 0, 0, 0 };
	m_billing = { 20, 0, 0, 0, 0 };
}

MobileTariffing::~MobileTariffing()
{
	m_client = { _T(""), 0, 0, 0 };
	m_billing = { 0, 0, 0, 0, 0 };
}

void MobileTariffing::SearchClient(CString szRow)
{
	int curPos = 0;
	
	CString szCurrCell = szRow.Tokenize(_T(","), curPos);
	CString szDetails = szRow.Mid(curPos);

	curPos = szDetails.Find(m_client.szPhone, 0);
	if (!curPos)
	{
		curPos = (m_client.szPhone.GetLength() + 1) * 2;
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		SetOutDur(szCurrCell);
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		SetMsgQty(szCurrCell);
	}
	else if (curPos > 0)
	{
		curPos += m_client.szPhone.GetLength() + 1;
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		SetInDur(szCurrCell);
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		SetMsgQty(szCurrCell);
	}
}

void MobileTariffing::SetPhone(CString szPhone)
{
	m_client.szPhone = szPhone;
}

void MobileTariffing::SetInDur(CString szInDur)
{
	FLOAT fMin;
	_stscanf_s(szInDur, _T("%f"), &fMin);
	m_client.fInDur += fMin;
}

void MobileTariffing::SetOutDur(CString szOutDur)
{
	FLOAT fMin;
	_stscanf_s(szOutDur, _T("%f"), &fMin);
	m_client.fOutDur += fMin;
}

void MobileTariffing::SetMsgQty(CString szMsgQty)
{
	UINT nMsg;
	_stscanf_s(szMsgQty, _T("%u"), &nMsg);
	m_client.nMsgQty += nMsg;
}

void MobileTariffing::SetBilling()
{
	if (m_client.fOutDur < Mobile::m_pricing.nFreeOutDur)
	{
		m_billing.fRemFreeOutDur -= m_client.fOutDur;
	}
	else
	{
		m_billing.fRemFreeOutDur = 0;
		m_billing.fOutgoing = Mobile::m_pricing.nOutPerMin * (m_client.fOutDur - Mobile::m_pricing.nFreeOutDur);
	}
	m_billing.fIncoming = Mobile::m_pricing.nInPerMin * m_client.fInDur;
	m_billing.fSMS = (FLOAT)Mobile::m_pricing.nSMSPerMsg * m_client.nMsgQty;

	m_billing.fTotal = m_billing.fIncoming + m_billing.fOutgoing + m_billing.fSMS;
}


void MobileTariffing::GetBilling(Mobile::Client& client, Mobile::Billing& billing)
{
	client.szPhone = m_client.szPhone;
	client.fInDur = m_client.fInDur;
	client.fOutDur = m_client.fOutDur;
	client.nMsgQty = m_client.nMsgQty;
	billing.fRemFreeOutDur = m_billing.fRemFreeOutDur;
	billing.fIncoming = m_billing.fIncoming;
	billing.fOutgoing = m_billing.fOutgoing;
	billing.fSMS = m_billing.fSMS;
	billing.fTotal = m_billing.fTotal;
}


CString MobileTariffing::GetBillingE()
{
	CString szBilling;
	szBilling.Format(_T("Billing for phone number %s:\r\n"), m_client.szPhone);
	szBilling.Append(_T("Category\t\tMinute(s)/Quantity\t\t  Total\r\n"));
	szBilling.AppendFormat(_T("Incoming calls\t    %.2f\t\t\t%.2f RUB\r\n"), m_client.fInDur, m_billing.fIncoming);
	szBilling.AppendFormat(_T("Outgoing calls\t    %.2f\t\t\t%.2f RUB\r\n"), m_client.fOutDur, m_billing.fOutgoing);
	szBilling.AppendFormat(_T("SMS\t\t      %u\t\t\t%.2f RUB\r\n"), m_client.nMsgQty, m_billing.fSMS);
	szBilling.AppendFormat(_T("Total\t\t\t\t\t%.2f RUB\r\n"), m_billing.fTotal);
	return szBilling;
}