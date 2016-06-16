
// MFCApplication2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include <afxdb.h>
#include <odbcinst.h>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication2Dlg dialog



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
	, m_username(_T("cj"))
	, m_password(_T("pass1"))
	, m_dbpath(_T("d:\\test.accdb"))
	, m_odbc_dsn(_T("try"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listControl);
	DDX_Text(pDX, EDIT_USERNAME, m_username);
	DDX_Text(pDX, EDIT_PASSWORD, m_password);
	DDX_Control(pDX, TEXT_LOGIN, m_login);
	DDX_Text(pDX, EDIT_DBPATH, m_dbpath);
	DDX_Text(pDX, EDIT_ODBCDSN, m_odbc_dsn);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCApplication2Dlg message handlers

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication2Dlg::OnBnClickedButton1() {
	
	const CString driver = L"Microsoft Access Driver (*.mdb, *.accdb)";
	const CString dbFile = m_dbpath;
	CString dataSourceName;
	
	CDatabase database;
	CString sqlStatement;

	UpdateData(TRUE);

	// makes sure that fields are not empty
	if (m_username.IsEmpty() || m_password.IsEmpty()) return;

	try {
		// tries to open the database
		bool successful = false;
		if (m_odbc_dsn.IsEmpty()) {
			dataSourceName.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", driver, dbFile);
			successful = (0 != database.Open(NULL, false, false, dataSourceName));
		}
		else {
			dataSourceName.Format(L"ODBC;UID=%s", m_username);
			successful = (0 != database.Open(m_odbc_dsn, false, false, dataSourceName));
			if (!successful) MessageBox(L"Damn it");
		}
		if (!successful) {
			m_login.SetWindowTextW(L"Failed \:(");
			return;
		}
		
		// set up variables
		sqlStatement.Format(L"SELECT * FROM users WHERE username='%s' AND password='%s';",
			m_username, m_password);
		CRecordset recordSet(&database);

		// tries to query
		successful = (0 != recordSet.Open
			(CRecordset::forwardOnly, sqlStatement, CRecordset::readOnly));
		if (!successful) return;

		// check query results
		if (recordSet.GetRecordCount() == 1) {
			m_login.SetWindowTextW(L"Sucess!");
		}
		else {
			m_login.SetWindowTextW(L"Failed \:(");
		}

		database.Close();
	}catch(CDBException& e) {
		AfxMessageBox(L"Database error: " + e.m_strError);
	}
}