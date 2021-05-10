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
ON_NOTIFY(NM_CLICK, IDC_LIST4, &TrainingListEmployes::OnNMClickList4)
ON_BN_CLICKED(IDC_BUTTON1, &TrainingListEmployes::OnBnClickedButton1)
END_MESSAGE_MAP()

// Инициация диалога
BOOL TrainingListEmployes::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CTrainingListDlg mainDlg;
	employee.SetExtendedStyle(LVS_EX_FULLROWSELECT);

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
	create_employee_page.SetDB(database);
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
	CString varValue = L" from department where department_id in ( \
			select distinct department_id from employee e \
			left join position p on e.position_id = p.position_id )";

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt " + varValue);
	cr.GetFieldValue(L"cnt", vtval);
	int rowCount = _ttoi(vtval);
	cr.Close();

	cr.Open(CRecordset::forwardOnly, L"Select department_id, name " + varValue);
	departments = new Department[rowCount];
	int i = 0;
	department.DeleteAllItems();
	while (!cr.IsEOF())
	{
		Department tmp{};
		cr.GetFieldValue(L"department_id", tmp.department_id);
		cr.GetFieldValue(L"name", tmp.department);
		cr.MoveNext();
		department.InsertItem(i, tmp.department);
		departments[i] = tmp;
		if (tmp.department_id == last_department_id) {
			department.SetCurSel(i);
		}
		i++;
	}

	cr.Close();
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
	CString nFields;
	CString varValue = L"from employee e \
				left join position p on e.position_id = p.position_id \
				where p.department_id = " + departmentID;

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt " + varValue);
	cr.GetFieldValue(L"cnt", nFields);
	int rowCount = _ttoi(nFields);
	cr.Close();

	CString qStr = L"Select \
				e.employee_id, \
				e.name employee, \
				p.name position " + varValue;
	cr.Open(CRecordset::forwardOnly, qStr);
	employees_all = new Employee[rowCount];

	int i = 0;
	employee.DeleteAllItems();
	while (!cr.IsEOF())
	{
		Employee tmp{};
		cr.GetFieldValue(L"employee_id", tmp.employee_id);
		cr.GetFieldValue(L"employee", tmp.employee);
		cr.GetFieldValue(L"position", tmp.position);
		cr.MoveNext();
		employees_all[i] = tmp;
		employee.InsertItem(i, (LPCTSTR)tmp.employee);
		employee.SetItemText(i, 1, tmp.position);
		i++;
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


void TrainingListEmployes::OnNMClickList4(NMHDR* pNMHDR, LRESULT* pResult)
{
	INT_PTR returnCode = -1;
	CString tmpVal = L"0";

	if (employee.GetSelectionMark() >= 0) {
		tmpVal = employees_all[employee.GetSelectionMark()].employee_id;
	}
	create_employee_page.employee_id = tmpVal;
	returnCode = create_employee_page.DoModal();
	last_department_id = departments[department.GetCurSel()].department_id;
	FillDepartments(database);
	UpdateData(false);
	FillEmployees(database, last_department_id);
}


void TrainingListEmployes::OnBnClickedButton1()
{
	INT_PTR returnCode = -1;
	create_employee_page.employee_id = L"0";
	returnCode = create_employee_page.DoModal();
	last_department_id = departments[department.GetCurSel()].department_id;
	FillDepartments(database);
	UpdateData(false);
	FillEmployees(database, last_department_id);
}
