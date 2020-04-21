#pragma once

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

	void SetIPAddress(CString);
	void SetInBytes(CString);
	void SetOutBytes(CString);

	void SetBilling();
	CString GetBillingE();
	//CString GetBillingCSV();

	FLOAT GetTotal();

	void SearchGraph(CString, std::vector<UINT>&, std::vector<COleDateTime>&,
						std::vector<UINT>&, std::vector<COleDateTime>&);
};



