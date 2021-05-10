// TrainingListCreateCourse.cpp: файл реализации
//

#include "pch.h"
#include "TrainingList.h"
#include "TrainingListDlg.h"
#include "TrainingListCreateCourse.h"
#include "afxdialogex.h"


// Диалоговое окно TrainingListCreateCourse

IMPLEMENT_DYNAMIC(TrainingListCreateCourse, CDialogEx)

TrainingListCreateCourse::TrainingListCreateCourse(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAININGLIST_CREATE_COURSE, pParent)
	, course_name_edit(_T(""))
	, course_duration_edit(_T(""))
{

}

TrainingListCreateCourse::~TrainingListCreateCourse()
{
}

void TrainingListCreateCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, skill_group);
	DDX_Control(pDX, IDC_LIST3, received_skill_selected);
	DDX_Control(pDX, IDC_LIST2, requirement_skill_selected);
	DDX_Control(pDX, IDC_LIST5, skill_all);
	DDX_Text(pDX, IDC_EDIT4, course_name_edit);
	DDX_Text(pDX, IDC_EDIT3, course_duration_edit);
}


BEGIN_MESSAGE_MAP(TrainingListCreateCourse, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &TrainingListCreateCourse::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &TrainingListCreateCourse::OnBnClickedButton5)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &TrainingListCreateCourse::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON2, &TrainingListCreateCourse::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &TrainingListCreateCourse::OnBnClickedButton6)
	ON_LBN_SELCHANGE(IDC_LIST2, &TrainingListCreateCourse::OnLbnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST3, &TrainingListCreateCourse::OnLbnSelchangeList3)
	ON_LBN_SELCHANGE(IDC_LIST5, &TrainingListCreateCourse::OnLbnSelchangeList5)
END_MESSAGE_MAP()

// Инициация диалога
BOOL TrainingListCreateCourse::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CTrainingListDlg mainDlg;
	TRY{
		FillSkillGroups(L"0");
		FillAllSkills();
		FillRequirementSkills();
		FillReceivedSkills();
		FillInputs();
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillSkillGroups(L"0");
			FillAllSkills();
			FillRequirementSkills();
			FillReceivedSkills();
			FillInputs();
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
void TrainingListCreateCourse::OnCancel() {
	UpdateData(true);
	CString courseSqlString = L"",
		requirementSqlString = L"",
		receivedSqlString = L"";
	int courseDuration = _ttoi(course_duration_edit);

	if (course_name_edit.GetLength() == 0) {
		CDialogEx::OnCancel();
	}
	else {
		course_name_edit.Replace(L"\\", L"\\\\");
		course_name_edit.Replace(L"\'", L"\\\'");
		course_duration_edit.Format(L"%d", courseDuration);
		UpdateData(false);

		if (_ttoi(course_id) > 0) {
			courseSqlString = L"Update course set name = '" + course_name_edit + L"', duration = " + course_duration_edit + L" where course_id = " + course_id;
		}
		else {
			courseSqlString = L"Insert into course(name, duration) values ('" + course_name_edit + L"'," + course_duration_edit + L");";
			requirementSqlString = L"Update requirement_skill set course_id = (select max(course_id) from course) where course_id = 0;";
			receivedSqlString = L"Update received_skill set course_id = (select max(course_id) from course) where course_id = 0;";
		}

		TRY{
			database->ExecuteSQL(courseSqlString);
			if (_ttoi(course_id) == 0) {
				database->ExecuteSQL(requirementSqlString);
				database->ExecuteSQL(receivedSqlString);
			}
			CDialogEx::OnCancel();
		} CATCH(CDBException, e) {
			CTrainingListDlg mainDlg;
			mainDlg.ReconnectDB();
			TRY{
				database->ExecuteSQL(courseSqlString);
				if (_ttoi(course_id) == 0) {
					database->ExecuteSQL(requirementSqlString);
					database->ExecuteSQL(receivedSqlString);
				}
				CDialogEx::OnCancel();
			} CATCH(CDBException, e) {
				AfxMessageBox(L"Sorry, we've got an error: " + e->m_strError);
			}
			END_CATCH;
		}
		END_CATCH;
	}
}

// Стилизация окна списка работников
void TrainingListCreateCourse::RedrawTab() {
	SetRedraw(FALSE);
	CRect rcWindow, Rect;
	skill_group.GetClientRect(&Rect);
	skill_group.AdjustRect(FALSE, &Rect);
	skill_group.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);
	Rect.OffsetRect(rcWindow.left, rcWindow.top);
	skill_all.MoveWindow(&Rect);
	SetRedraw(TRUE);
}

// Стилизация окна списка работников
void TrainingListCreateCourse::FillSkillGroups(CString skill_group_id) {
	CRecordset cr(database);
	CString varVal, qStr;
	int rowCount, itemCount;

	qStr = L"Select count(DISTINCT skill_group_id) cnt from skill where skill_id not in ( \
					select skill_id from requirement_skill where course_id = " + course_id + L" \
					union \
					select skill_id from received_skill where course_id = " + course_id + L")";
	cr.Open(CRecordset::forwardOnly, qStr);
	cr.GetFieldValue(L"cnt", varVal);
	rowCount = _ttoi(varVal);
	cr.Close();

	qStr = L"Select skill_group_id, name from skill_group where skill_group_id in ( \
					select DISTINCT skill_group_id from skill where skill_id not in ( \
						select skill_id from requirement_skill where course_id = " + course_id + L" \
						union \
						select skill_id from received_skill where course_id = " + course_id + L") \
				)";
	cr.Open(CRecordset::forwardOnly, qStr);
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
		if (tmp.skill_group_id == skill_group_id) {
			skill_group.SetCurSel(i);
		}
		i++;
	}
	cr.Close();
}

// Заполнение окна умений
void TrainingListCreateCourse::FillAllSkills() {
	if (skill_group.GetCurSel() >= 0) {
		CRecordset cr(database);
		CString varValue, qStr;
		qStr = L"from skill where skill_group_id \
		= " + skill_groups[skill_group.GetCurSel()].skill_group_id + L" \
		and skill_id not in ( \
			select skill_id from requirement_skill where course_id = " + course_id + L" \
			union \
			select skill_id from received_skill where course_id = " + course_id + L") \
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
	}
}

// Заполнение окна умений
void TrainingListCreateCourse::FillRequirementSkills() {
	CRecordset cr(database);
	CString varValue, firstQueryString, secondQueryString;
	firstQueryString = L"Select s.skill_id, s.name from ";
	secondQueryString = L" rs \
			left join skill s on rs.skill_id = s.skill_id \
			where rs.course_id = " + course_id + L" \
			order by 1 desc";

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from requirement_skill where course_id = " + course_id);
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	requirement_skill_selected.ResetContent();
	if (rowCount > 0) {
		cr.Open(CRecordset::forwardOnly, firstQueryString + L"requirement_skill" + secondQueryString);
		requirement_skills_selected = new Skill[rowCount];
		int i = 0;

		while (!cr.IsEOF())
		{
			Skill tmp{};
			cr.GetFieldValue(L"skill_id", tmp.skill_id);
			cr.GetFieldValue(L"name", tmp.skill);
			cr.MoveNext();
			requirement_skill_selected.AddString(tmp.skill);
			requirement_skills_selected[i] = tmp;
			i++;
		}
		cr.Close();
	}
}

// Заполнение окна умений
void TrainingListCreateCourse::FillReceivedSkills() {
	CRecordset cr(database);
	CString varValue, firstQueryString, secondQueryString;
	firstQueryString = L"Select s.skill_id, s.name from ";
	secondQueryString = L" rs \
			left join skill s on rs.skill_id = s.skill_id \
			where rs.course_id = " + course_id + L" \
			order by 1 desc";

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from received_skill where course_id = " + course_id);
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	received_skill_selected.ResetContent();
	if (rowCount > 0) {
		cr.Open(CRecordset::forwardOnly, firstQueryString + L"received_skill" + secondQueryString);
		received_skills_selected = new Skill[rowCount];
		int i = 0;

		while (!cr.IsEOF())
		{
			Skill tmp{};
			cr.GetFieldValue(L"skill_id", tmp.skill_id);
			cr.GetFieldValue(L"name", tmp.skill);
			cr.MoveNext();
			received_skill_selected.AddString(tmp.skill);
			received_skills_selected[i] = tmp;
			i++;
		}
		cr.Close();
	}
}

// Заполнение окна умений
void TrainingListCreateCourse::FillInputs() {
	if (_ttoi(course_id) > 0) {
		CRecordset cr(database);
		CString varValue;
		
		varValue = L"Select name, duration from course where course_id = " + course_id;
		cr.Open(CRecordset::forwardOnly, varValue);

		cr.GetFieldValue(L"name", course_name_edit);
		cr.GetFieldValue(L"duration", course_duration_edit);

		cr.Close();
	}
	else {
		course_name_edit = L"";
		course_duration_edit = L"7";
	}
	UpdateData(false);
}

// Функция для передачи в этот класс БД
void TrainingListCreateCourse::SetDB(CDatabase* database) {
	this->database = database;
}

// Добавить скилл в требуемые
void TrainingListCreateCourse::OnBnClickedButton1()
{
	if (skill_all.GetCurSel() >= 0) {
		CString SqlString, skill_group_id, err = L"";
		CString skill_id = skills_all[skill_all.GetCurSel()].skill_id;
		SqlString = L"INSERT INTO requirement_skill (course_id, skill_id) VALUES (" + course_id + L"," + skill_id + L")";

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
			FillRequirementSkills();
			FillSkillGroups(skill_group_id);
			FillAllSkills();
		}
	}
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(false);
}

// Убрать скилл из требуемых
void TrainingListCreateCourse::OnBnClickedButton5()
{
	if (requirement_skill_selected.GetCurSel() >= 0) {
		CString SqlString, skill_group_id, err = L"";
		CString skill_id = requirement_skills_selected[requirement_skill_selected.GetCurSel()].skill_id;
		SqlString = L"Delete from requirement_skill where course_id = " + course_id + L" and skill_id = " + skill_id;

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
			FillSkillGroups(skill_group_id);
			FillRequirementSkills();
			FillAllSkills();
		}
	}

	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(false);
}

// Смена скилл группы
void TrainingListCreateCourse::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
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
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	
	*pResult = 0;
}

// Добавить скилл в получаемые
void TrainingListCreateCourse::OnBnClickedButton2()
{
	if (skill_all.GetCurSel() >= 0) {
		CString SqlString, skill_group_id, err = L"";
		CString skill_id = skills_all[skill_all.GetCurSel()].skill_id;
		SqlString = L"INSERT INTO received_skill (course_id, skill_id) VALUES (" + course_id + L"," + skill_id + L")";

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
			FillReceivedSkills();
			FillSkillGroups(skill_group_id);
			FillAllSkills();
		}
	}

	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(false);
}

// Убрать скилл из получаемых
void TrainingListCreateCourse::OnBnClickedButton6()
{
	if (received_skill_selected.GetCurSel() >= 0) {
		CString SqlString, skill_group_id, err = L"";
		CString skill_id = received_skills_selected[received_skill_selected.GetCurSel()].skill_id;
		SqlString = L"Delete from received_skill where course_id = " + course_id + L" and skill_id = " + skill_id;

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
			FillSkillGroups(skill_group_id);
			FillReceivedSkills();
			FillAllSkills();
		}
	}

	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(false);
}


void TrainingListCreateCourse::OnLbnSelchangeList2()
{
	GetDlgItem(IDC_BUTTON5)->EnableWindow(requirement_skill_selected.GetCurSel() >= 0);
}


void TrainingListCreateCourse::OnLbnSelchangeList3()
{
	GetDlgItem(IDC_BUTTON6)->EnableWindow(received_skill_selected.GetCurSel() >= 0);
}


void TrainingListCreateCourse::OnLbnSelchangeList5()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(skill_all.GetCurSel() >= 0);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(skill_all.GetCurSel() >= 0);
}
