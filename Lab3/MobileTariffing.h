#pragma once

namespace Mobile
{
	typedef struct
	{
		UINT nInPerMin;
		UINT nFreeOutDur;
		UINT nOutPerMin;
		UINT nSMSPerMsg;
	} Pricing;

	const Pricing m_pricing{ 0, 20, 2, 2 };

	typedef struct
	{
		CString szPhone;
		FLOAT fInDur;
		FLOAT fOutDur;
		UINT nMsgQty;
	} Client;

	typedef struct
	{
		FLOAT fRemFreeOutDur;
		FLOAT fIncoming;
		FLOAT fOutgoing;
		FLOAT fSMS;
		FLOAT fTotal;
	} Billing;
}

class MobileTariffing
{
private:
	Mobile::Client m_client;
	Mobile::Billing m_billing;
public:
	MobileTariffing();
	~MobileTariffing();

	void SearchClient(CString);
	void SetPhone(CString);
	void SetInDur(CString);
	void SetOutDur(CString);
	void SetMsgQty(CString);

	void SetBilling();
	void GetBilling(Mobile::Client&, Mobile::Billing&);
	CString GetBillingE();
};



