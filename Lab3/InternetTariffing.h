#pragma once


namespace Internet
{
	typedef struct
	{
		UINT nPerMb;
	} Pricing;

	const Pricing m_pricing{ 1 };

	typedef struct
	{
		CString szIPAddress;
		UINT nInBytes;
		UINT nOutBytes;
	} Client;

	typedef struct
	{
		FLOAT fSending;
		FLOAT fReceiving;
		FLOAT fTotal;
	} Billing;
}


class InternetTariffing
{
private:
	Internet::Client m_client;
	Internet::Billing m_billing;
public:
	InternetTariffing();
	~InternetTariffing();

	void SearchClient(CString);

	void SetIPAddress(CString);
	void SetInBytes(CString);
	void SetOutBytes(CString);

	void SetBilling();
	void GetBilling(Internet::Client&, Internet::Billing&);
	CString GetBillingE();
};



