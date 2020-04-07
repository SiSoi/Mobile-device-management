#include "pch.h"
#include "Variant3.h"

Variant3::Variant3()
{
	m_client = { _T(""), 0, 0, 0 };
	m_billing = { 20, 0, 0, 0, 0 };
}

Variant3::~Variant3()
{
	m_client = { _T(""), 0, 0, 0 };
	m_billing = { 0, 0, 0, 0, 0 };

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
	
	CString szCurrCell = szRow.Tokenize(_T(","), curPos);
	CString szDetails = szRow.Mid(curPos);

	curPos = szDetails.Find(m_client.szPhone, 0);
	if (!curPos)
	{
		curPos = (m_client.szPhone.GetLength() + 1) * 2;
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		SetInDur(szCurrCell);
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		SetMsgQty(szCurrCell);
	}
	if (curPos > 0)
	{
		curPos = m_client.szPhone.GetLength() + 1;
		szCurrCell = szDetails.Tokenize(_T(","), curPos);
		SetOutDur(szCurrCell);
	}
}

void Variant3::SetPhone(CString szPhone)
{
	m_client.szPhone = szPhone;
}

void Variant3::SetInDur(CString szInDur)
{
	FLOAT fMin;
	_stscanf_s(szInDur, _T("%2f"), &fMin);
	m_client.fInDur += fMin;
}

void Variant3::SetOutDur(CString szOutDur)
{
	FLOAT fMin;
	_stscanf_s(szOutDur, _T("%2f"), &fMin);
	m_client.fOutDur += fMin;
}

void Variant3::SetMsgQty(CString szMsgQty)
{
	UINT nMsg;
	_stscanf_s(szMsgQty, _T("%u"), &nMsg);
	m_client.nMsgQty += nMsg;
}

void Variant3::SetBilling()
{
	if (m_client.fInDur < m_pricing.nFreeInDur)
	{
		m_billing.fRemFreeInDur -= m_client.fInDur;
	}
	else
	{
		m_billing.fRemFreeInDur = 0;
		m_billing.fIncoming = m_pricing.nInPerMin * (m_client.fInDur - m_pricing.nFreeInDur);
	}
	m_billing.fOutgoing = m_pricing.nOutPerMin * m_client.fOutDur;
	m_billing.fSMS = m_pricing.nSMSPerMsg * m_client.nMsgQty;

	m_billing.fTotal = m_billing.fIncoming + m_billing.fOutgoing + m_billing.fSMS;
}

CString Variant3::GetBillingE()
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