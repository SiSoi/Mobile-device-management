#pragma once

typedef struct
{
	UINT nInPerMin;
	UINT nFreeInDur;
	UINT nOutPerMin;
	UINT nSMSPerMsg;
} Pricing;

const Pricing m_pricing{ 2, 20, 0, 2 };

typedef struct
{
	CString szPhone;
	FLOAT fInDur;
	FLOAT fOutDur;
	UINT nMsgQty;
} Client;

typedef struct
{
	FLOAT fRemFreeInDur;
	FLOAT fIncoming;
	FLOAT fOutgoing;
	FLOAT fSMS;
	FLOAT fTotal;
} Billing;

class Variant3
{
private:
	//static Variant3* m_processor;
	Client m_client;
	Billing m_billing;
public:
	Variant3();
	~Variant3();
	//static Variant3* GetInstance();

	void SearchClient(CString);
	void SetPhone(CString);
	void SetInDur(CString);
	void SetOutDur(CString);
	void SetMsgQty(CString);

	void SetBilling();
	CString GetBillingE();
	//CString GetBillingCSV();
};



