// TrainingListCreateEmployee.cpp: файл реализации
//

#include "pch.h"
#include "TrainingList.h"
#include "TrainingListDlg.h"
#include "TrainingListCreateEmployee.h"
#include "afxdialogex.h"


// Диалоговое окно TrainingListCreateEmployee

IMPLEMENT_DYNAMIC(TrainingListCreateEmployee, CDialogEx)

TrainingListCreateEmployee::TrainingListCreateEmployee(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAININGLIST_CREATE_EMPLOYEE, pParent)
	, employee_name_edit(_T(""))
	, position_edit(_T(""))
{

}

TrainingListCreateEmployee::~TrainingListCreateEmployee()
{
}

void TrainingListCreateEmployee::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, employee_name_edit);
	DDX_Control(pDX, IDC_TAB1, skill_group);
	DDX_Control(pDX, IDC_LIST5, skill_all);
	DDX_Control(pDX, IDC_TAB2, department);
	DDX_Control(pDX, IDC_LIST6, position);
	DDX_Control(pDX, IDC_LIST2, skill_employee);
	DDX_Text(pDX, IDC_EDIT5, position_edit);
}


BEGIN_MESSAGE_MAP(TrainingListCreateEmployee, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &TrainingListCreateEmployee::OnTcnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &TrainingListCreateEmployee::OnTcnSelchangeTab2)
	ON_LBN_SELCHANGE(IDC_LIST5, &TrainingListCreateEmployee::OnLbnSelchangeList5)
	ON_LBN_SELCHANGE(IDC_LIST2, &TrainingListCreateEmployee::OnLbnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST6, &TrainingListCreateEmployee::OnLbnSelchangeList6)
	ON_BN_CLICKED(IDC_BUTTON2, &TrainingListCreateEmployee::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &TrainingListCreateEmployee::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &TrainingListCreateEmployee::OnBnClickedButton5)
END_MESSAGE_MAP()


// Инициация диалога
BOOL TrainingListCreateEmployee::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CTrainingListDlg mainDlg;
	TRY{
		FillSkillGroups(L"0");
		FillAllSkills();
		FillInputs();
		FillEmployeSkills();
		FillDepartment(L"0");
		FillPosition();
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillSkillGroups(L"0");
			FillAllSkills();
			FillInputs();
			FillEmployeSkills();
			FillDepartment(L"0");
			FillPosition();
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
	UpdateData(false);
	RedrawTab();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}


// Закрытие диалога
void TrainingListCreateEmployee::OnCancel() {
	UpdateData(true);
	CString employeeSqlString = L"",
			skillSqlString = L"";
	int positionID = _ttoi(position_id);
	

	if (employee_name_edit.GetLength() == 0) {
		CDialogEx::OnCancel();
	}
	else {
		employee_name_edit.Replace(L"\\", L"\\\\");
		employee_name_edit.Replace(L"\'", L"\\\'");
		position_id.Format(L"%d", positionID);
		UpdateData(false);

		if (_ttoi(employee_id) > 0) {
			employeeSqlString = L"Update employee set name = '" + employee_name_edit + L"', position_id = " + position_id + L" where employee_id = " + employee_id;
		}
		else {
			employeeSqlString = L"Insert into employee(name, position_id) values ('" + employee_name_edit + L"'," + position_id + L");";
			skillSqlString = L"Update employee_skill set employee_id = (select max(employee_id) from employee) where employee_id = 0;";
		}

		TRY{
			database->ExecuteSQL(employeeSqlString);
			if (_ttoi(employee_id) == 0) {
				database->ExecuteSQL(skillSqlString);
			}
		} CATCH(CDBException, e) {
			CTrainingListDlg mainDlg;
			mainDlg.ReconnectDB();
			TRY{
				database->ExecuteSQL(employeeSqlString);
				if (_ttoi(employee_id) == 0) {
					database->ExecuteSQL(skillSqlString);
				}
			} CATCH(CDBException, e) {
				AfxMessageBox(L"Database error: " + e->m_strError);
			}
			END_CATCH;
		}
		END_CATCH;
		CDialogEx::OnCancel();
	}
}

// Стилизация окна списка работников
void TrainingListCreateEmployee::RedrawTab() {
	SetRedraw(FALSE);
	CRect rcWindow, Rect;
	skill_group.GetClientRect(&Rect);
	skill_group.AdjustRect(FALSE, &Rect);
	skill_group.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);
	Rect.OffsetRect(rcWindow.left, rcWindow.top);
	skill_all.MoveWindow(&Rect);
	department.GetClientRect(&Rect);
	department.AdjustRect(FALSE, &Rect);
	department.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);
	Rect.OffsetRect(rcWindow.left, rcWindow.top);
	position.MoveWindow(&Rect);
	SetRedraw(TRUE);
}

// Функция для передачи в этот класс БД
void TrainingListCreateEmployee::SetDB(CDatabase* database) {
	this->database = database;
}

// Заполнение групп умений
void TrainingListCreateEmployee::FillSkillGroups(CString skill_group_id) {
	CRecordset cr(database);
	CString varVal, qStr;
	int rowCount;
	qStr = L"from skill where skill_id not in ( \
		select skill_id from employee_skill where employee_id = " + employee_id + L")";

	cr.Open(CRecordset::forwardOnly, L"Select count(DISTINCT skill_group_id) cnt " + qStr);
	cr.GetFieldValue(L"cnt", varVal);
	rowCount = _ttoi(varVal);
	cr.Close();

	qStr = L"Select skill_group_id, name from skill_group where skill_group_id in ( \
					select DISTINCT skill_group_id " + qStr + L")";
	cr.Open(CRecordset::forwardOnly, qStr);
	skill_groups = new SkillGroup[rowCount];
	int i = 0;

	skill_group.DeleteAllItems();
	while (!cr.IsEOF())
	{
		SkillGroup tmp{};
		cr.GetFieldValue(L"skill_group_id", tmp.skill_group_id);
		cr.GetFieldValue(L"name", tmp.skill_group);
		cr.MoveNext();
		skill_group.InsertItem(i, tmp.skill_group);
		skill_groups[i] = tmp;
		if (tmp.skill_group_id == skill_group_id) {
			skill_group.SetCurSel(i);
		}
		i++;
	}
	cr.Close();
	UpdateData(false);
}

// Заполнение подразделений
void TrainingListCreateEmployee::FillDepartment(CString department_id) {
	CRecordset cr(database);
	CString vtval;
	CString varValue = L" from department";
	if (position_id != L"") {
		varValue += L" where department_id in ( \
			select distinct department_id from position p \
			where p.position_id != " + position_id + L")";
	}

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
		i++;
	}
	cr.Close();
	UpdateData(false);
}

// Заполнение должностей
void TrainingListCreateEmployee::FillPosition() {
	CRecordset cr(database);
	CString nFields;
	CString varValue = L" from position p \
				where p.department_id = " + departments[department.GetCurSel()].department_id;
	if (position_id != L"") {
		varValue += L" and p.position_id != " + position_id;
	}

	cr.Open(CRecordset::forwardOnly, L"Select count(distinct p.position_id) cnt " + varValue);
	cr.GetFieldValue(L"cnt", nFields);
	int rowCount = _ttoi(nFields);
	cr.Close();

	int i = 0;
	position.ResetContent();
	if (rowCount > 0) {
		cr.Open(CRecordset::forwardOnly, L"Select distinct p.position_id, p.name position " + varValue);
		positions = new Position[rowCount];
		int i = 0;

		while (!cr.IsEOF())
		{
			Position tmp{};
			cr.GetFieldValue(L"position_id", tmp.position_id);
			cr.GetFieldValue(L"position", tmp.position);
			cr.MoveNext();
			position.AddString(tmp.position);
			positions[i] = tmp;
			i++;
		}
		cr.Close();
	}
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
}

// Заполнение окна умений
void TrainingListCreateEmployee::FillAllSkills() {
	CRecordset cr(database);
	CString varValue, qStr;
	qStr = L"from skill where skill_group_id \
		= " + skill_groups[skill_group.GetCurSel()].skill_group_id + L" \
		and skill_id not in ( \
			select skill_id from employee_skill where employee_id = " + employee_id + L") \
		order by 1 desc";

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt " + qStr);
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	skill_all.ResetContent();
	if (rowCount > 0) {
		cr.Open(CRecordset::forwardOnly, L"Select skill_id, name " + qStr);
		skills_all = new Skill[rowCount];
		int i = 0;

		while (!cr.IsEOF())
		{
			Skill tmp{};
			cr.GetFieldValue(L"skill_id", tmp.skill_id);
			cr.GetFieldValue(L"name", tmp.skill);
			cr.MoveNext();
			skill_all.AddString(tmp.skill);
			skills_all[i] = tmp;
			i++;
		}
		cr.Close();
	}
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
}

// Заполнение окна умений
void TrainingListCreateEmployee::FillEmployeSkills() {
	CRecordset cr(database);
	CString varValue, qStr;
	qStr = L"from skill where skill_id in ( \
			select skill_id from employee_skill where employee_id = " + employee_id + L") \
		order by 1 desc";

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt " + qStr);
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	skill_employee.ResetContent();
	if (rowCount > 0) {
		cr.Open(CRecordset::forwardOnly, L"Select skill_id, name " + qStr);
		skills_employee = new Skill[rowCount];
		int i = 0;

		while (!cr.IsEOF())
		{
			Skill tmp{};
			cr.GetFieldValue(L"skill_id", tmp.skill_id);
			cr.GetFieldValue(L"name", tmp.skill);
			cr.MoveNext();
			skill_employee.AddString(tmp.skill);
			skills_employee[i] = tmp;
			i++;
		}
		cr.Close();
	}
}

// Заполнение окна умений
void TrainingListCreateEmployee::FillInputs() {
	if (_ttoi(employee_id) > 0) {
		CRecordset cr(database);
		CString varValue;

		varValue = L"Select e.name name, p.name position, p.position_id from employee e \
			left join position p on p.position_id = e.position_id \
			where employee_id = " + employee_id;
		cr.Open(CRecordset::forwardOnly, varValue);
		cr.GetFieldValue(L"name", employee_name_edit);
		cr.GetFieldValue(L"position", position_edit);
		cr.GetFieldValue(L"position_id", position_id);
		cr.Close();
	}
	else {
		employee_name_edit = L"";
		position_edit = L"";
		position_id = L"";
	}
	UpdateData(false);
}

// Смена скилл группы
void TrainingListCreateEmployee::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	TRY{
		FillAllSkills();
	} CATCH(CDBException, e) {
		CTrainingListDlg mainDlg;
		mainDlg.ReconnectDB();
		TRY{
			FillAllSkills();
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
}


void TrainingListCreateEmployee::OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult)
{
	TRY{
		FillPosition();
	} CATCH(CDBException, e) {
		CTrainingListDlg mainDlg;
		mainDlg.ReconnectDB();
		TRY{
			FillPosition();
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
}


void TrainingListCreateEmployee::OnLbnSelchangeList5()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(skill_all.GetCurSel() >= 0);
}


void TrainingListCreateEmployee::OnLbnSelchangeList2()
{
	GetDlgItem(IDC_BUTTON5)->EnableWindow(skill_employee.GetCurSel() >= 0);
}


void TrainingListCreateEmployee::OnLbnSelchangeList6()
{
	GetDlgItem(IDC_BUTTON2)->EnableWindow(position.GetCurSel() >= 0);
}


void TrainingListCreateEmployee::OnBnClickedButton2()
{
	position_id = positions[position.GetCurSel()].position_id;
	position_edit = positions[position.GetCurSel()].position;
	FillDepartment(departments[department.GetCurSel()].department_id);
	FillPosition();
	UpdateData(false);
}


void TrainingListCreateEmployee::OnBnClickedButton1()
{
	if (skill_all.GetCurSel() >= 0) {
		CString SqlString, skill_group_id, err = L"";
		CString skill_id = skills_all[skill_all.GetCurSel()].skill_id;
		SqlString = L"INSERT INTO employee_skill (employee_id, skill_id) VALUES (" + employee_id + L"," + skill_id + L")";

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

		if (err == "") {
			skill_group_id = L"";
			if (skill_group.GetCurSel() >= 0) {
				skill_group_id = skill_groups[skill_group.GetCurSel()].skill_group_id;
			}
			FillEmployeSkills();
			FillSkillGroups(skill_group_id);
			FillAllSkills();
		}
	}
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(false);
}


void TrainingListCreateEmployee::OnBnClickedButton5()
{
	if (skill_employee.GetCurSel() >= 0) {
		CString SqlString, skill_group_id, err = L"";
		CString skill_id = skills_employee[skill_employee.GetCurSel()].skill_id;
		SqlString = L"Delete from employee_skill where employee_id = " + employee_id + L" and skill_id = " + skill_id;

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

		if (err == "") {
			skill_group_id = L"";
			if (skill_group.GetCurSel() >= 0) {
				skill_group_id = skill_groups[skill_group.GetCurSel()].skill_group_id;
			}
			FillEmployeSkills();
			FillSkillGroups(skill_group_id);
			FillAllSkills();
		}
	}

	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(false);
}
