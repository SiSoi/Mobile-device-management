#pragma once
class CSVProcessor
{
private:
	CString m_szFileName;
public:
	std::vector<CString> m_CSVRow;
    CSVProcessor(CString);
    ~CSVProcessor();
	void CSVRow2Vector();
};
