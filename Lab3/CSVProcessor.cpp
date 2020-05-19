#include "pch.h"
#include "CSVProcessor.h"

CSVProcessor::CSVProcessor(CString szFileName)
{
	m_szFileName = szFileName;
}

CSVProcessor::~CSVProcessor()
{
	m_CSVRow.empty();
	m_szFileName.Empty();
}


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
	if (m_CSVRow[m_CSVRow.size() - 1] == _T(""))
		m_CSVRow.pop_back();
}