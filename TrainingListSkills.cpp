// TrainingListSkills.cpp: файл реализации
//

#include "pch.h"
#include "TrainingList.h"
#include "TrainingListDlg.h"
#include "TrainingListSkills.h"
#include "afxdialogex.h"


// Диалоговое окно TrainingListSkills

IMPLEMENT_DYNAMIC(TrainingListSkills, CDialogEx)

TrainingListSkills::TrainingListSkills(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAININGLIST_SKILLS, pParent)
	, skill_group_edit(_T(""))
	, skill_edit(_T(""))
{

}

TrainingListSkills::~TrainingListSkills()
{
}

void TrainingListSkills::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, skill_group);
	DDX_Control(pDX, IDC_LIST2, skill);
	DDX_Text(pDX, IDC_EDIT1, skill_group_edit);
	DDX_Text(pDX, IDC_EDIT2, skill_edit);
}


BEGIN_MESSAGE_MAP(TrainingListSkills, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &TrainingListSkills::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &TrainingListSkills::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &TrainingListSkills::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &TrainingListSkills::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT1, &TrainingListSkills::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &TrainingListSkills::OnEnChangeEdit2)
	ON_LBN_SELCHANGE(IDC_LIST1, &TrainingListSkills::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &TrainingListSkills::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// Инициация диалога
BOOL TrainingListSkills::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CTrainingListDlg mainDlg;

	TRY{
		FillSkillGroups();
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillSkillGroups();
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
void TrainingListSkills::SetDB(CDatabase* database) {
	this->database = database;
}

// Заполнение списка групп скилов
void TrainingListSkills::FillSkillGroups() {
	CRecordset cr(database);
	CString varValue;

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from skill_group");
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	cr.Open(CRecordset::forwardOnly, L"Select skill_group_id, name from skill_group order by 1 desc");
	skill_groups = new SkillGroup[rowCount];
	skill_group.ResetContent();
	int i = 0;

	while (!cr.IsEOF())
	{
		SkillGroup tmp{};
		cr.GetFieldValue(L"skill_group_id", tmp.skill_group_id);
		cr.GetFieldValue(L"name", tmp.skill_group);
		cr.MoveNext();
		skill_group.AddString(tmp.skill_group);
		skill_groups[i] = tmp;
		i++;
	}
	cr.Close();
	ResetControls(L"skill_group");
}

// Заполнение списка скилов
void TrainingListSkills::FillSkills(CString skill_group_id) {
	CRecordset cr(database);
	CString varValue;

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from skill where skill_group_id = " + skill_group_id);
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	cr.Open(CRecordset::forwardOnly, L"Select skill_id, name from skill where skill_group_id = " + skill_group_id + " order by 1 desc");
	skills = new Skill[rowCount];
	skill.ResetContent();
	int i = 0;

	while (!cr.IsEOF())
	{
		Skill tmp{};
		cr.GetFieldValue(L"skill_id", tmp.skill_id);
		cr.GetFieldValue(L"name", tmp.skill);
		cr.MoveNext();
		skill.AddString(tmp.skill);
		skills[i] = tmp;
		i++;
	}
	cr.Close();
	ResetControls(L"skill");
}

// Обновление группы скилов
CString TrainingListSkills::UpdateSkillGroup(CString skill_group_id, CString to) {
	CString SqlString, err = L"";
	to.Replace(L"\\", L"\\\\");
	to.Replace(L"\'", L"\\\'");
	SqlString = skill_group_id.GetLength() ?
		L"UPDATE skill_group SET name = '" + to + L"' where skill_group_id = " + skill_group_id :
		L"INSERT INTO skill_group (name) VALUES ('" + to + L"')";

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

// Удаление группы скилов
CString TrainingListSkills::DeleteSkillGroup(CString skill_group_id) {
	CString firstSqlString, secondSqlString, err = L"";
	firstSqlString = L"DELETE FROM skill_group where skill_group_id = " + skill_group_id;
	secondSqlString = L"DELETE FROM skill where skill_group_id = " + skill_group_id;

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

// Обновление скила
CString TrainingListSkills::UpdateSkill(CString skill_id, CString to) {
	CString SqlString, err = L"";
	CString skill_group_id = skill_groups[skill_group.GetCurSel()].skill_group_id;
	to.Replace(L"\\", L"\\\\");
	to.Replace(L"\'", L"\\\'");
	SqlString = skill_id.GetLength() ?
		L"UPDATE skill SET name = '" + to + L"' where skill_id = " + skill_id :
		L"INSERT INTO skill (name, skill_group_id) VALUES ('" + to + L"'," + skill_group_id + L")";

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

// Удаление скила
CString TrainingListSkills::DeleteSkill(CString skill_id) {
	CString SqlString, err = L"";
	SqlString = L"DELETE FROM skill where skill_id = " + skill_id;

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

// Сброс контроллов
void TrainingListSkills::ResetControls(CString control_type) {
	skill.SetCurSel(-1);
	skill_edit = L"";
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);

	if (control_type == L"skill_group")
	{
		skill_group.SetCurSel(-1);
		skill_group_edit = L"";
		GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
		skill.ResetContent();
	}
}

void TrainingListSkills::OnBnClickedButton1()
{
	UpdateData(true);
	CString skill_group_id = L"";
	CTrainingListDlg mainDlg;

	if (skill_group.GetCurSel() >= 0) {
		skill_group_id = skill_groups[skill_group.GetCurSel()].skill_group_id;
	}
	CString err = UpdateSkillGroup(skill_group_id, skill_group_edit);

	if (!err.GetLength()) {
		ResetControls(L"skill_group");
		FillSkillGroups();
		UpdateData(false);
	}
	else {
		AfxMessageBox(L"Database error: " + err);
	}
}


void TrainingListSkills::OnBnClickedButton2()
{
	UpdateData(true);
	CString skill_id = L"";
	CTrainingListDlg mainDlg;

	if (skill.GetCurSel() >= 0) {
		skill_id = skills[skill.GetCurSel()].skill_id;
	}
	CString err = UpdateSkill(skill_id, skill_edit);

	if (!err.GetLength()) {
		ResetControls(L"skill");
		FillSkills(skill_groups[skill_group.GetCurSel()].skill_group_id);
		UpdateData(false);
	}
	else {
		AfxMessageBox(L"Database error: " + err);
	}
}


void TrainingListSkills::OnBnClickedButton3()
{
	UpdateData(true);
	CString skill_group_id = L"";
	CTrainingListDlg mainDlg;

	if (skill_group.GetCurSel() >= 0) {
		skill_group_id = skill_groups[skill_group.GetCurSel()].skill_group_id;
	}
	CString err = DeleteSkillGroup(skill_group_id);

	if (!err.GetLength()) {
		ResetControls(L"skill_group");
		FillSkillGroups();
		UpdateData(false);
	}
	else {
		AfxMessageBox(L"Database error: " + err);
	}
}


void TrainingListSkills::OnBnClickedButton4()
{
	UpdateData(true);
	CString skill_id = L"";
	CTrainingListDlg mainDlg;

	if (skill.GetCurSel() >= 0) {
		skill_id = skills[skill.GetCurSel()].skill_id;
	}
	CString err = DeleteSkill(skill_id);

	if (!err.GetLength()) {
		ResetControls(L"skill");
		FillSkills(skill_groups[skill_group.GetCurSel()].skill_group_id);
		UpdateData(false);
	}
	else {
		AfxMessageBox(L"Database error: " + err);
	}
}


void TrainingListSkills::OnEnChangeEdit1()
{
	UpdateData(true);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(skill_group_edit.GetLength());
}


void TrainingListSkills::OnEnChangeEdit2()
{
	UpdateData(true);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(skill_edit.GetLength());
}


void TrainingListSkills::OnLbnSelchangeList1()
{
	if (skill_group.GetCurSel() >= 0) {
		TRY{
			FillSkills(skill_groups[skill_group.GetCurSel()].skill_group_id);
		} CATCH(CDBException, e) {
			CTrainingListDlg mainDlg;
			mainDlg.ReconnectDB();
			TRY{
				FillSkills(skill_groups[skill_group.GetCurSel()].skill_group_id);
			} CATCH(CDBException, e) {
				AfxMessageBox(L"Database error: " + e->m_strError);
			}
			END_CATCH;
		}
		END_CATCH;
		skill_group_edit = skill_groups[skill_group.GetCurSel()].skill_group;
		UpdateData(false);

		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
	}
}


void TrainingListSkills::OnLbnSelchangeList2()
{
	if (skill.GetCurSel() >= 0) {
		skill_edit = skills[skill.GetCurSel()].skill;
		UpdateData(false);

		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(true);
	}
}
