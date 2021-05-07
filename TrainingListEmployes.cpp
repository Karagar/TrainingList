// TrainingListEmployes.cpp: файл реализации
//

#include "pch.h"
#include "TrainingList.h"
#include "TrainingListDlg.h"
#include "TrainingListEmployes.h"
#include "TrainingListCreateEmployee.h"
#include "afxdialogex.h"



TrainingListCreateEmployee create_employee_page;

IMPLEMENT_DYNAMIC(TrainingListEmployes, CDialogEx)

TrainingListEmployes::TrainingListEmployes(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAININGLIST_EMPLOYES, pParent)
{

}

TrainingListEmployes::~TrainingListEmployes()
{
}

void TrainingListEmployes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST4, employee);
	DDX_Control(pDX, IDC_TAB1, department);
}


BEGIN_MESSAGE_MAP(TrainingListEmployes, CDialogEx)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &TrainingListEmployes::OnChangeActiveTab)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &TrainingListEmployes::OnLvnItemchangedList4)
END_MESSAGE_MAP()

// Инициация диалога
BOOL TrainingListEmployes::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CTrainingListDlg mainDlg;

	TRY{
		FillDepartments(database);
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillDepartments(database);
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
	UpdateData(false);
	RedrawTab();
	PaintEmployes();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// Функция для передачи в этот класс БД
void TrainingListEmployes::SetDB(CDatabase *database) {
	this->database = database;
}

// Смена активного таба-подразделения
void TrainingListEmployes::OnChangeActiveTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	PaintEmployes();
	*pResult = 0;
}

// Заполнение табов-подразделений
void TrainingListEmployes::FillDepartments(CDatabase* db) {
	CRecordset cr(db);
	CString vtval;

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from department");
	cr.GetFieldValue(L"cnt", vtval);
	int rowCount = _ttoi(vtval);
	cr.Close();

	cr.Open(CRecordset::forwardOnly, L"Select department_id, name from department order by 1 desc");
	departments = new Department[rowCount];
	int i = 0;

	while (!cr.IsEOF())
	{
		Department tmp{};
		cr.GetFieldValue(L"department_id", tmp.department_id);
		cr.GetFieldValue(L"name", tmp.department);
		cr.MoveNext();
		department.InsertItem(i, tmp.department);
		departments[i] = tmp;
		i++;
	}

	cr.Close();
	department.SetCurSel(0);
}

// Отрисовка списка работников
void TrainingListEmployes::PaintEmployes()
{
	const int len = 256;
	CString departmentID;
	CTrainingListDlg mainDlg;
	TCITEM tcItem{ 0 };
	tcItem.mask = TCIF_TEXT;
	tcItem.cchTextMax = len;
	TCHAR buf[len] = { 0 };
	tcItem.pszText = buf;
	departmentID = departments[department.GetCurSel()].department_id;

	TRY{
		FillEmployees(database, departmentID);
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillEmployees(database, departmentID);
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
	UpdateData(false);
}

// Заполнение списка работников в зависимости от подразделения
void TrainingListEmployes::FillEmployees(CDatabase* db, CString departmentID) {
	CRecordset cr(db);
	CString qStr = L"Select \
				e.name employee, \
				p.name position \
				from employee e \
				left join position p on e.position_id = p.position_id \
				left join department d on p.department_id = d.department_id \
				where d.department_id = " + departmentID;
	cr.Open(CRecordset::forwardOnly, qStr);
	short nFields = cr.GetODBCFieldCount();
	CString employeeValue, positionValue;

	employee.DeleteAllItems();
	int cIndex = 0;
	while (!cr.IsEOF())
	{
		cr.GetFieldValue(L"employee", employeeValue);
		cr.GetFieldValue(L"position", positionValue);
		cr.MoveNext();
		employee.InsertItem(cIndex, (LPCTSTR)employeeValue);
		employee.SetItemText(cIndex, 1, positionValue);
		cIndex++;
	}
	cr.Close();
}

// Стилизация окна списка работников
void TrainingListEmployes::RedrawTab() {
	SetRedraw(FALSE);
	CRect rcWindow, Rect;
	department.GetClientRect(&Rect);
	department.AdjustRect(FALSE, &Rect);
	department.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);
	Rect.OffsetRect(rcWindow.left, rcWindow.top);
	employee.MoveWindow(&Rect);
	employee.InsertColumn(0, L"ФИО");
	employee.SetColumnWidth(0, Rect.Width()/2.01);
	employee.InsertColumn(1, L"Должность");
	employee.SetColumnWidth(1, Rect.Width()/2.01);
	employee.SetExtendedStyle(LVS_EX_GRIDLINES);
	employee.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	SetRedraw(TRUE);
}

// Выбор конкретного работника
void TrainingListEmployes::OnLvnItemchangedList4(NMHDR* pNMHDR, LRESULT* pResult)
{
	INT_PTR returnCode = -1;
	create_employee_page.employe_id = "0";
	returnCode = create_employee_page.DoModal();
}
