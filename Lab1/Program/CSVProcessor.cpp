#include "pch.h"
#include "CSVProcessor.h"

CSVProcessor::CSVProcessor()
{
	m_szFileName = _T("res\\data.csv");
}

CSVProcessor::~CSVProcessor()
{
	m_CSVRow.empty();
	//m_CSVColumn.empty();
	m_szFileName.Empty();

	//m_CSVProcessor = nullptr;
}

/*
CSVProcessor* CSVProcessor::GetInstance()
{
	if (m_CSVProcessor == nullptr)
		m_CSVProcessor = new CSVProcessor();
	return m_CSVProcessor;
}
*/

/*
void CSVProcessor::CSVTokenise(vector<CString>& vtDest, const CString& szSrc, CString szToken)
{
	int curPos = 0;
	while (curPos != -1)
	{
		CString szRow = szSrc.Tokenize(szToken, curPos);
		vtDest.push_back(szRow);
	}
}
*/

void CSVProcessor::CSVRow2Vector()
{
    CString szData; //read the whole file in to one string "source"
	CStdioFile currFile;
	CFileException errFile;
	if (!currFile.Open(m_szFileName, CFile::modeRead, &errFile))
	{
		TCHAR szError[1024];
		errFile.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError, MB_ICONSTOP);
	}
	else
	{
		TCHAR buf[512];
		while (currFile.ReadString(buf, 511) != nullptr)
			szData += buf;
	}
	currFile.Close();

	int curPos = 0;
	while (curPos != -1)
	{
		CString szRow = szData.Tokenize(_T("\r\n"), curPos); //tokenise rows and store in vector
		m_CSVRow.push_back(szRow);
	}
}