#pragma once
class CSVProcessor
{
private:
	//static CSVProcessor* m_CSVProcessor;
	CString m_szFileName;
public:
	std::vector<CString> m_CSVRow;
	//std::vector<CString> m_CSVColumn;
    CSVProcessor(CString);
    ~CSVProcessor();
    //static CSVProcessor* GetInstance();
	//void CSVTokenise(vector<CString>&, const CString&, CString);
	void CSVRow2Vector();
};
