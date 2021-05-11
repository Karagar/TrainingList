
// TrainingListDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "TrainingList.h"
#include "TrainingListDlg.h"
#include "TrainingListEmployes.h"
#include "TrainingListDepartmentsPositions.h"
#include "TrainingListCreateCourse.h"
#include "TrainingListSkills.h"
#include "TrainingListMatrix.h"
#include "TrainingListCourses.h"
#include "afxdialogex.h"
#include "odbcinst.h"
#include "afxdb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDatabase database;
CString connection_string = L"Driver={MySQL ODBC 8.0 Unicode Driver};Server=153.92.7.101;Database=u251775558_training_list;UID=u251775558_admin;PWD=Admin123";
TrainingListEmployes employes_page;
TrainingListDepartmentsPositions departments_positions_page;
TrainingListSkills skills_page;
TrainingListMatrix matrix_page;
TrainingListCourses courses_page;

// Диалоговое окно CAboutDlg используется для описания сведений о приложении
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CTrainingListDlg
CTrainingListDlg::CTrainingListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAININGLIST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CTrainingListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CTrainingListDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTrainingListDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTrainingListDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTrainingListDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CTrainingListDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CTrainingListDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

// Обработчики сообщений CTrainingListDlg
BOOL CTrainingListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию



	// Build ODBC connection string
	TRY{
		// Open the database
		database.Open(NULL,false,false,connection_string);
	} CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;

	employes_page.SetDB(&database);
	departments_positions_page.SetDB(&database);
	skills_page.SetDB(&database);
	courses_page.SetDB(&database);
	matrix_page.SetDB(&database);
	//database.Close();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CTrainingListDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTrainingListDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTrainingListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTrainingListDlg::ReconnectDB()
{
	database.Close();
	TRY{
		database.Open(NULL,false,false,connection_string);
	} CATCH(CDBException, e) {
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
}

void CTrainingListDlg::OnBnClickedButton1()
{
	INT_PTR returnCode = -1;
	returnCode = matrix_page.DoModal();
}


void CTrainingListDlg::OnBnClickedButton2()
{
	INT_PTR returnCode = -1;
	returnCode = employes_page.DoModal();
}

void CTrainingListDlg::OnBnClickedButton3()
{
	INT_PTR returnCode = -1;
	returnCode = departments_positions_page.DoModal();
}


void CTrainingListDlg::OnBnClickedButton4()
{
	INT_PTR returnCode = -1;
	returnCode = skills_page.DoModal();
}


void CTrainingListDlg::OnBnClickedButton5()
{
	INT_PTR returnCode = -1;
	returnCode = courses_page.DoModal();
}