// TrainingListMatrix.cpp: файл реализации
//

#include "pch.h"
#include "TrainingList.h"
#include "TrainingListDlg.h"
#include "TrainingListMatrix.h"
#include "afxdialogex.h"


// Диалоговое окно TrainingListMatrix

IMPLEMENT_DYNAMIC(TrainingListMatrix, CDialogEx)

TrainingListMatrix::TrainingListMatrix(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAININGLIST_MATRIX, pParent)
{

}

TrainingListMatrix::~TrainingListMatrix()
{
}

void TrainingListMatrix::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB2, skill_group);
	DDX_Control(pDX, IDC_TAB1, department);
	DDX_Control(pDX, IDC_LIST4, employee);
	DDX_Control(pDX, IDC_LIST7, course);
	DDX_Control(pDX, IDC_LIST5, available_skill);
	DDX_Control(pDX, IDC_LIST9, employee_skill);
	DDX_Control(pDX, IDC_LIST3, received_skill);
	DDX_Control(pDX, IDC_LIST2, requirement_skill);
}


BEGIN_MESSAGE_MAP(TrainingListMatrix, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &TrainingListMatrix::OnTcnSelchangeTab1)
	ON_NOTIFY(NM_CLICK, IDC_LIST4, &TrainingListMatrix::OnNMClickList4)
	ON_LBN_SELCHANGE(IDC_LIST5, &TrainingListMatrix::OnLbnSelchangeList5)
	ON_NOTIFY(NM_CLICK, IDC_LIST7, &TrainingListMatrix::OnNMClickList7)
	ON_BN_CLICKED(IDC_BUTTON1, &TrainingListMatrix::OnBnClickedButton1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &TrainingListMatrix::OnTcnSelchangeTab2)
END_MESSAGE_MAP()

// Функция для передачи в этот класс БД
void TrainingListMatrix::SetDB(CDatabase* database) {
	this->database = database;
}

// Инициация диалога
BOOL TrainingListMatrix::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CTrainingListDlg mainDlg;
	UpdateData(false);
	

	TRY{
		FillDepartments();
		RedrawDepartmentTab();
		FillSkillGroups();
		RedrawSkillGroupTab();
		FillEmployees();
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillDepartments();
			RedrawDepartmentTab();
			FillSkillGroups();
			RedrawSkillGroupTab();
			FillEmployees();
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
	UpdateData(false);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// Стилизация окна списка работников
void TrainingListMatrix::RedrawDepartmentTab() {
	SetRedraw(FALSE);
	CRect rcWindow, Rect;
	department.GetClientRect(&Rect);
	department.AdjustRect(FALSE, &Rect);
	department.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);
	Rect.OffsetRect(rcWindow.left, rcWindow.top);
	employee.MoveWindow(&Rect);
	employee.InsertColumn(0, L"ФИО");
	employee.SetColumnWidth(0, Rect.Width() / 2.02);
	employee.InsertColumn(1, L"Должность");
	employee.SetColumnWidth(1, Rect.Width() / 2.02);
	employee.SetExtendedStyle(LVS_EX_GRIDLINES);
	employee.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	course.GetClientRect(&Rect);
	course.InsertColumn(0, L"Название");
	course.SetColumnWidth(0, Rect.Width() * 0.5);
	course.InsertColumn(1, L"Продолжительность (дней)");
	course.SetColumnWidth(1, Rect.Width() * 0.5);
	course.SetExtendedStyle(LVS_EX_GRIDLINES);
	course.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	course.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	SetRedraw(TRUE);
}

// Стилизация окна списка работников
void TrainingListMatrix::RedrawSkillGroupTab() {
	SetRedraw(FALSE);
	CRect rcWindow, Rect;
	skill_group.GetClientRect(&Rect);
	skill_group.AdjustRect(FALSE, &Rect);
	skill_group.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);
	Rect.OffsetRect(rcWindow.left, rcWindow.top);
	available_skill.MoveWindow(&Rect);
	SetRedraw(TRUE);
}

// Заполнение табов-подразделений
void TrainingListMatrix::FillDepartments() {
	CRecordset cr(database);
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
		i++;
	}

	cr.Close();
}

// Заполнение списка работников в зависимости от подразделения
void TrainingListMatrix::FillEmployees() {
	CRecordset cr(database);
	CString nFields;
	CString departmentID = departments[department.GetCurSel()].department_id;
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

// При смене активного окна подразделения
void TrainingListMatrix::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CTrainingListDlg mainDlg;
	ClearAllWindows();
	TRY{
		FillEmployees();
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillEmployees();
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
	
}

// Стилизация окна списка работников
void TrainingListMatrix::FillSkillGroups() {
	CRecordset cr(database);
	CString varVal, qStr;
	int rowCount, itemCount;
	if (_ttoi(employee_id) > 0) {
		qStr = L" from skill where skill_id in ( \
						SELECT skill_id FROM `skill` WHERE skill_id not in ( \
							select skill_id FROM employee_skill WHERE employee_id = " + employee_id + L" \
						) and skill_id in( \
							select skill_id FROM received_skill WHERE course_id not in( \
								select course_id from requirement_skill where skill_id not in \
								(SELECT skill_id FROM `employee_skill` WHERE employee_id = " + employee_id + L") \
							) \
						) \
					)";
		cr.Open(CRecordset::forwardOnly, L"Select count(DISTINCT skill_group_id) cnt" + qStr);
		cr.GetFieldValue(L"cnt", varVal);
		rowCount = _ttoi(varVal);
		cr.Close();

		varVal = L"Select skill_group_id, name from skill_group where skill_group_id in ( \
					select skill_group_id " + qStr + L")";
		cr.Open(CRecordset::forwardOnly, varVal);
		skill_groups = new SkillGroup[rowCount];
		int i = 0;

		skill_group.DeleteAllItems();
		if (rowCount == 0)
		{
			skill_groups = new SkillGroup[1];
			SkillGroup tmp{};
			tmp.skill_group_id = L"0";
			tmp.skill_group = L"";
			skill_group.InsertItem(0, tmp.skill_group);
			skill_groups[0] = tmp;
			skill_group.SetCurSel(0);
		}
		while (!cr.IsEOF())
		{
			SkillGroup tmp{};
			cr.GetFieldValue(L"skill_group_id", tmp.skill_group_id);
			cr.GetFieldValue(L"name", tmp.skill_group);
			cr.MoveNext();
			skill_group.InsertItem(i, tmp.skill_group);
			skill_groups[i] = tmp;
			i++;
		}
		cr.Close();
	} else {
		skill_group.DeleteAllItems();
		skill_groups = new SkillGroup[1];
		SkillGroup tmp{};
		tmp.skill_group_id = L"0";
		tmp.skill_group = L"";
		skill_group.InsertItem(0, tmp.skill_group);
		skill_groups[0] = tmp;
		skill_group.SetCurSel(0);
	}
}

// Заполнение окна доступных умений
void TrainingListMatrix::FillAllSkills() {
	if (skill_group.GetCurSel() >= 0) {
		CRecordset cr(database);
		CString varValue, qStr;
		qStr = L"from skill where skill_group_id = " + skill_groups[skill_group.GetCurSel()].skill_group_id + L" \
		and skill_id in(\
			SELECT skill_id FROM `skill` WHERE skill_id not in(\
				select skill_id FROM employee_skill WHERE employee_id = " + employee_id + L" \
			) and skill_id in(\
				select skill_id FROM received_skill WHERE course_id not in(\
					select course_id from requirement_skill where skill_id not in \
					(SELECT skill_id FROM `employee_skill` WHERE employee_id = " + employee_id + L") \
				) \
			) \
		)";

		cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt " + qStr);
		cr.GetFieldValue(L"cnt", varValue);
		int rowCount = _ttoi(varValue);
		cr.Close();

		available_skill.ResetContent();
		if (rowCount > 0) {
			cr.Open(CRecordset::forwardOnly, L"Select skill_id, name " + qStr);
			available_skills = new Skill[rowCount];
			int i = 0;

			while (!cr.IsEOF())
			{
				Skill tmp{};
				cr.GetFieldValue(L"skill_id", tmp.skill_id);
				cr.GetFieldValue(L"name", tmp.skill);
				cr.MoveNext();
				available_skill.AddString(tmp.skill);
				available_skills[i] = tmp;
				i++;
			}
			cr.Close();
		}
	}
}

// Заполнение окна умений работника
void TrainingListMatrix::FillEmployeeSkills() {
	if (_ttoi(employee_id) > 0) 
	{
		CRecordset cr(database);
		CString varValue, qStr;
		qStr = L" from skill where skill_id in(\
				select skill_id FROM employee_skill WHERE employee_id = " + employee_id + L" \
			)";

		cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt " + qStr);
		cr.GetFieldValue(L"cnt", varValue);
		int rowCount = _ttoi(varValue);
		cr.Close();

		employee_skill.ResetContent();
		if (rowCount > 0) {
			cr.Open(CRecordset::forwardOnly, L"Select skill_id, name " + qStr);
			employee_skills = new Skill[rowCount];
			int i = 0;

			while (!cr.IsEOF())
			{
				Skill tmp{};
				cr.GetFieldValue(L"skill_id", tmp.skill_id);
				cr.GetFieldValue(L"name", tmp.skill);
				cr.MoveNext();
				employee_skill.AddString(tmp.skill);
				employee_skills[i] = tmp;
				i++;
			}
			cr.Close();
		}
	}
}

// Заполнение списка курсов
void TrainingListMatrix::FillCourses() {
	CRecordset cr(database);
	CString varValue, qStr;

	qStr = L" from course where course_id in(\
				select course_id FROM received_skill WHERE skill_id = " + skill_id + L" \
			) and course_id not in(\
				select course_id from requirement_skill where skill_id not in \
				(SELECT skill_id FROM `employee_skill` WHERE employee_id = " + employee_id + L") \
			)";
	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt " + qStr);
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	cr.Open(CRecordset::forwardOnly, L"Select course_id, name, duration " + qStr);
	courses = new Course[rowCount];
	course.DeleteAllItems();
	int i = 0;

	while (!cr.IsEOF())
	{
		Course tmp{};
		cr.GetFieldValue(L"course_id", tmp.course_id);
		cr.GetFieldValue(L"name", tmp.course);
		cr.GetFieldValue(L"duration", tmp.duration);
		cr.MoveNext();
		course.InsertItem(i, (LPCTSTR)tmp.course);
		course.SetItemText(i, 1, tmp.duration);
		courses[i] = tmp;
		i++;
	}
	cr.Close();
}

// Заполнение окон умений курса
void TrainingListMatrix::FillCourseSkills() {
	if (_ttoi(course_id) > 0)
	{
		CRecordset cr(database);
		CString varValue, received_qStr, requirement_qStr;
		int rowCount;
		received_qStr = L" from skill where skill_id in(\
				select skill_id FROM received_skill WHERE course_id = " + course_id + L" \
			)";
		requirement_qStr = L" from skill where skill_id in(\
				select skill_id FROM requirement_skill WHERE course_id = " + course_id + L" \
			)";

		cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt " + received_qStr);
		cr.GetFieldValue(L"cnt", varValue);
		rowCount = _ttoi(varValue);
		cr.Close();

		received_skill.ResetContent();
		if (rowCount > 0) {
			cr.Open(CRecordset::forwardOnly, L"Select skill_id, name " + received_qStr);
			received_skills = new Skill[rowCount];
			int i = 0;

			while (!cr.IsEOF())
			{
				Skill tmp{};
				cr.GetFieldValue(L"skill_id", tmp.skill_id);
				cr.GetFieldValue(L"name", tmp.skill);
				cr.MoveNext();
				received_skill.AddString(tmp.skill);
				received_skills[i] = tmp;
				i++;
			}
			cr.Close();
		}

		cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt " + requirement_qStr);
		cr.GetFieldValue(L"cnt", varValue);
		rowCount = _ttoi(varValue);
		cr.Close();

		requirement_skill.ResetContent();
		if (rowCount > 0) {
			cr.Open(CRecordset::forwardOnly, L"Select skill_id, name " + requirement_qStr);
			requirement_skills = new Skill[rowCount];
			int i = 0;

			while (!cr.IsEOF())
			{
				Skill tmp{};
				cr.GetFieldValue(L"skill_id", tmp.skill_id);
				cr.GetFieldValue(L"name", tmp.skill);
				cr.MoveNext();
				requirement_skill.AddString(tmp.skill);
				requirement_skills[i] = tmp;
				i++;
			}
			cr.Close();
		}
	}
}

// Функция для очистки всех списков умений и курсов
void TrainingListMatrix::ClearAllWindows() {
	employee_skill.ResetContent();
	available_skill.ResetContent();
	skill_group.DeleteAllItems();
	skill_groups = new SkillGroup[1];
	SkillGroup tmp{};
	tmp.skill_group_id = L"0";
	tmp.skill_group = L"";
	skill_group.InsertItem(0, tmp.skill_group);
	skill_groups[0] = tmp;
	skill_group.SetCurSel(0);
	requirement_skill.ResetContent();
	received_skill.ResetContent();
	course.DeleteAllItems();
}

// Функция для очистки списков курсов и связанных умений
void TrainingListMatrix::ClearBottomWindows() {
	requirement_skill.ResetContent();
	received_skill.ResetContent();
	course.DeleteAllItems();
}


void TrainingListMatrix::OnNMClickList4(NMHDR* pNMHDR, LRESULT* pResult)
{
	CTrainingListDlg mainDlg;
	INT_PTR returnCode = -1;
	ClearBottomWindows();

	if (employee.GetSelectionMark() >= 0) {
		employee_id = employees_all[employee.GetSelectionMark()].employee_id;
	} else 
	{
		employee_id = L"";
	}

	TRY{
		FillSkillGroups();
		FillAllSkills();
		FillEmployeeSkills();
		UpdateData(false);
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillSkillGroups();
			FillAllSkills();
			FillEmployeeSkills();
			UpdateData(false);
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
}


void TrainingListMatrix::OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult)
{
	CTrainingListDlg mainDlg;
	ClearBottomWindows();

	if (employee.GetSelectionMark() >= 0) {
		employee_id = employees_all[employee.GetSelectionMark()].employee_id;
	}
	else
	{
		employee_id = L"";
	}

	TRY{
		FillAllSkills();
		FillEmployeeSkills();
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillAllSkills();
			FillEmployeeSkills();
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
	UpdateData(false);
}


void TrainingListMatrix::OnLbnSelchangeList5()
{
	if (available_skill.GetCurSel() >= 0) {
		skill_id = available_skills[available_skill.GetCurSel()].skill_id;
		FillCourses();
	}
}


void TrainingListMatrix::OnNMClickList7(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (course.GetSelectionMark() >= 0) {
		course_id = courses[course.GetSelectionMark()].course_id;
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);

		TRY{
			FillCourseSkills();
		} CATCH(CDBException, e) {
			CTrainingListDlg mainDlg;
			mainDlg.ReconnectDB();
			TRY{
				FillCourseSkills();
			} CATCH(CDBException, e) {
				AfxMessageBox(L"Database error: " + e->m_strError);
			}
			END_CATCH;
		}
		END_CATCH;
		UpdateData(false);
	}
}


void TrainingListMatrix::OnBnClickedButton1()
{
	if ((_ttoi(employee_id) > 0)&&(_ttoi(course_id) > 0)) {
		CString SqlString, err = L"";
		SqlString = L"INSERT INTO employee_skill (employee_id, skill_id) \
			select " + employee_id + L", skill_id from received_skill \
			where course_id = " + course_id + L" and skill_id not in( \
			select skill_id from employee_skill where employee_id = " + employee_id + L" \
			)";

		TRY{
			database->ExecuteSQL(SqlString);
			FillSkillGroups();
			FillAllSkills();
			FillEmployeeSkills();
			UpdateData(false);
		} CATCH(CDBException, e) {
			CTrainingListDlg mainDlg;
			mainDlg.ReconnectDB();
			TRY{
				database->ExecuteSQL(SqlString);
				FillSkillGroups();
				FillAllSkills();
				FillEmployeeSkills();
				UpdateData(false);
			} CATCH(CDBException, e) {
				err = e->m_strError;
			}
			END_CATCH;
		}
		END_CATCH;

		if (err == "") {
			ClearBottomWindows();
		}
	}

	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
}
