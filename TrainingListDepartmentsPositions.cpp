// TrainingListDepartmentsPositions.cpp: файл реализации
//

#include "pch.h"
#include "TrainingList.h"
#include "TrainingListDlg.h"
#include "TrainingListDepartmentsPositions.h"
#include "afxdialogex.h"


// Диалоговое окно TrainingListDepartmentsPositions

IMPLEMENT_DYNAMIC(TrainingListDepartmentsPositions, CDialogEx)

TrainingListDepartmentsPositions::TrainingListDepartmentsPositions(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAININGLIST_DEPARTMENTS_POSITIONS, pParent)
	, department_edit(_T(""))
	, position_edit(_T(""))
{

}

TrainingListDepartmentsPositions::~TrainingListDepartmentsPositions()
{
}

void TrainingListDepartmentsPositions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, department);
	DDX_Control(pDX, IDC_LIST2, position);
	DDX_Text(pDX, IDC_EDIT1, department_edit);
	DDX_Text(pDX, IDC_EDIT2, position_edit);
}


BEGIN_MESSAGE_MAP(TrainingListDepartmentsPositions, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &TrainingListDepartmentsPositions::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &TrainingListDepartmentsPositions::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &TrainingListDepartmentsPositions::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &TrainingListDepartmentsPositions::OnBnClickedButton3)
	ON_LBN_SELCHANGE(IDC_LIST2, &TrainingListDepartmentsPositions::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON2, &TrainingListDepartmentsPositions::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &TrainingListDepartmentsPositions::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT2, &TrainingListDepartmentsPositions::OnEnChangeEdit2)
END_MESSAGE_MAP()

// Инициация диалога
BOOL TrainingListDepartmentsPositions::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CTrainingListDlg mainDlg;

	TRY{
		FillDepartments();
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillDepartments();
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
	UpdateData(false);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// Функция для передачи в этот класс БД
void TrainingListDepartmentsPositions::SetDB(CDatabase* database) {
	this->database = database;
}

// Заполнение списка подразделений
void TrainingListDepartmentsPositions::FillDepartments() {
	CRecordset cr(database);
	CString varValue;

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from department");
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	cr.Open(CRecordset::forwardOnly, L"Select department_id, name from department order by 1 desc");
	departments = new Department[rowCount];
	department.ResetContent();
	int i = 0;

	while (!cr.IsEOF())
	{
		Department tmp{};
		cr.GetFieldValue(L"department_id", tmp.department_id);
		cr.GetFieldValue(L"name", tmp.department);
		cr.MoveNext();
		department.AddString(tmp.department);
		departments[i] = tmp;
		i++;
	}
	cr.Close();
	ResetControls(L"department");
}

// Заполнение списка должностей
void TrainingListDepartmentsPositions::FillPositions(CString department_id) {
	CRecordset cr(database);
	CString varValue;

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from position where department_id = " + department_id);
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	cr.Open(CRecordset::forwardOnly, L"Select position_id, name from position where department_id = " + department_id + " order by 1 desc");
	positions = new Position[rowCount];
	position.ResetContent();
	int i = 0;

	while (!cr.IsEOF())
	{
		Position tmp{};
		cr.GetFieldValue(L"position_id", tmp.position_id);
		cr.GetFieldValue(L"name", tmp.position);
		cr.MoveNext();
		position.AddString(tmp.position);
		positions[i] = tmp;
		i++;
	}
	cr.Close();
	ResetControls(L"position");
}

// Обновление подразделения
CString TrainingListDepartmentsPositions::UpdateDepartment(CString department_id, CString to) {
	CString SqlString, err = L"";
	to.Replace(L"\\", L"\\\\");
	to.Replace(L"\'", L"\\\'");
	SqlString = department_id.GetLength() ?
		L"UPDATE department SET name = '" + to + L"' where department_id = " + department_id :
		L"INSERT INTO department (name) VALUES ('" + to + L"')";

	TRY{
		database->ExecuteSQL(SqlString);
	} CATCH(CDBException, e) {
		CTrainingListDlg mainDlg;
		mainDlg.ReconnectDB();
		TRY{
			database->ExecuteSQL(SqlString);
		} CATCH(CDBException, e) {
			err = e->m_strError;
		}
		END_CATCH;
	}
	END_CATCH;
	return err;
}

// Удаление подразделения
CString TrainingListDepartmentsPositions::DeleteDepartment(CString department_id) {
	CString firstSqlString, secondSqlString, ThirdSqlString, err = L"";
	firstSqlString = L"DELETE FROM department where department_id = " + department_id;
	secondSqlString = L"DELETE FROM position_skill where position_id in ( \
		select position_id FROM position where department_id = " + department_id + L")";
	ThirdSqlString = L"DELETE FROM position where department_id = " + department_id;


	TRY{
		database->ExecuteSQL(firstSqlString);
		database->ExecuteSQL(secondSqlString);
		database->ExecuteSQL(ThirdSqlString);
	} CATCH(CDBException, e) {
		CTrainingListDlg mainDlg;
		mainDlg.ReconnectDB();
		TRY{
			database->ExecuteSQL(firstSqlString);
			database->ExecuteSQL(secondSqlString);
			database->ExecuteSQL(ThirdSqlString);
		} CATCH(CDBException, e) {
			err = e->m_strError;
		}
		END_CATCH;
	}
	END_CATCH;
	return err;
}

// Обновление должности
CString TrainingListDepartmentsPositions::UpdatePosition(CString position_id, CString to) {
	CString SqlString, err = L"";
	CString department_id = departments[department.GetCurSel()].department_id;
	to.Replace(L"\\", L"\\\\");
	to.Replace(L"\'", L"\\\'");
	SqlString = position_id.GetLength() ?
		L"UPDATE position SET name = '" + to + L"' where position_id = " + position_id :
		L"INSERT INTO position (name, department_id) VALUES ('" + to + L"'," + department_id + L")";

	TRY{
		database->ExecuteSQL(SqlString);
	} CATCH(CDBException, e) {
		CTrainingListDlg mainDlg;
		mainDlg.ReconnectDB();
		TRY{
			database->ExecuteSQL(SqlString);
		} CATCH(CDBException, e) {
			err = e->m_strError;
		}
		END_CATCH;
	}
	END_CATCH;
	return err;
}

// Удаление подразделения
CString TrainingListDepartmentsPositions::DeletePosition(CString position_id) {
	CString firstSqlString, secondSqlString, err = L"";
	firstSqlString = L"DELETE FROM position where position_id = " + position_id;
	secondSqlString = L"DELETE FROM position_skill where position_id = " + position_id;

	TRY{
		database->ExecuteSQL(firstSqlString);
		database->ExecuteSQL(secondSqlString);
	} CATCH(CDBException, e) {
		CTrainingListDlg mainDlg;
		mainDlg.ReconnectDB();
		TRY{
			database->ExecuteSQL(firstSqlString);
			database->ExecuteSQL(secondSqlString);
		} CATCH(CDBException, e) {
			err = e->m_strError;
		}
		END_CATCH;
	}
	END_CATCH;
	return err;
}

// Сброс контроллов
void TrainingListDepartmentsPositions::ResetControls(CString control_type) {
	position.SetCurSel(-1);
	position_edit = L"";
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);

	if (control_type == L"department")
	{
		department.SetCurSel(-1);
		department_edit = L"";
		GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
		position.ResetContent();
	}
}

// Выбор конкретного подразделения
void TrainingListDepartmentsPositions::OnLbnSelchangeList1()
{
	if (department.GetCurSel() >= 0) {
		TRY{
			FillPositions(departments[department.GetCurSel()].department_id);
		} CATCH(CDBException, e) {
			CTrainingListDlg mainDlg;
			mainDlg.ReconnectDB();
			TRY{
				FillPositions(departments[department.GetCurSel()].department_id);
			} CATCH(CDBException, e) {
				AfxMessageBox(L"Database error: " + e->m_strError);
			}
			END_CATCH;
		}
		END_CATCH;
		department_edit = departments[department.GetCurSel()].department;
		UpdateData(false);

		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
	}
}

// Выбор конкретной должности
void TrainingListDepartmentsPositions::OnLbnSelchangeList2()
{
	if (position.GetCurSel() >= 0) {
		position_edit = positions[position.GetCurSel()].position;
		UpdateData(false);

		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(true);
	}
}

// Сохранить
void TrainingListDepartmentsPositions::OnBnClickedButton1()
{
	UpdateData(true);
	CString department_id = L"";
	CTrainingListDlg mainDlg;

	if (department.GetCurSel() >= 0) {
		department_id = departments[department.GetCurSel()].department_id;
	}
	CString err = UpdateDepartment(department_id, department_edit);

	if (!err.GetLength()) {
		ResetControls(L"department");
		FillDepartments();
		UpdateData(false);
	} else {
		AfxMessageBox(L"Database error: " + err);
	}
}

// Редактировать
void TrainingListDepartmentsPositions::OnBnClickedButton2()
{
	UpdateData(true);
	CString position_id = L"";
	CTrainingListDlg mainDlg;

	if (position.GetCurSel() >= 0) {
		position_id = positions[position.GetCurSel()].position_id;
	}
	CString err = UpdatePosition(position_id, position_edit);

	if (!err.GetLength()) {
		ResetControls(L"position");
		FillPositions(departments[department.GetCurSel()].department_id);
		UpdateData(false);
	} else {
		AfxMessageBox(L"Database error: " + err);
	}
}

// Удалить
void TrainingListDepartmentsPositions::OnBnClickedButton3()
{
	UpdateData(true);
	CString department_id = L"";
	CTrainingListDlg mainDlg;

	if (department.GetCurSel() >= 0) {
		department_id = departments[department.GetCurSel()].department_id;
	}
	CString err = DeleteDepartment(department_id);

	if (!err.GetLength()) {
		ResetControls(L"department");
		FillDepartments();
		UpdateData(false);
	}
	else {
		AfxMessageBox(L"Database error: " + err);
	}
}

// Изменение поля ввода
void TrainingListDepartmentsPositions::OnEnChangeEdit1()
{
	UpdateData(true);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(department_edit.GetLength());
}

void TrainingListDepartmentsPositions::OnBnClickedButton4()
{
	UpdateData(true);
	CString position_id = L"";
	CTrainingListDlg mainDlg;

	if (position.GetCurSel() >= 0) {
		position_id = positions[position.GetCurSel()].position_id;
	}
	CString err = DeletePosition(position_id);

	if (!err.GetLength()) {
		ResetControls(L"position");
		FillPositions(departments[department.GetCurSel()].department_id);
		UpdateData(false);
	}
	else {
		AfxMessageBox(L"Database error: " + err);
	}
}


void TrainingListDepartmentsPositions::OnEnChangeEdit2()
{
	UpdateData(true);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(position_edit.GetLength());
}