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
{

}

TrainingListCreateEmployee::~TrainingListCreateEmployee()
{
}

void TrainingListCreateEmployee::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TrainingListCreateEmployee, CDialogEx)
END_MESSAGE_MAP()


// Инициация диалога
BOOL TrainingListCreateEmployee::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CTrainingListDlg mainDlg;
	TRY{
		FillSkillGroups(L"0");
		FillAllSkills();
		FillEmployeSkills();
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillSkillGroups(L"0");
			FillAllSkills();
			FillEmployeSkills();
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

// Стилизация окна списка работников
void TrainingListCreateEmployee::RedrawTab() {
	/*SetRedraw(FALSE);
	CRect rcWindow, Rect;
	skill_group.GetClientRect(&Rect);
	skill_group.AdjustRect(FALSE, &Rect);
	skill_group.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);
	Rect.OffsetRect(rcWindow.left, rcWindow.top);
	skill_all.MoveWindow(&Rect);
	SetRedraw(TRUE);*/
}

// Стилизация окна списка работников
void TrainingListCreateEmployee::FillSkillGroups(CString skill_group_id) {
	/*CRecordset cr(database);
	CString varVal, qStr;
	int rowCount;

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
	UpdateData(false);*/
}

// Заполнение окна умений
void TrainingListCreateEmployee::FillAllSkills() {
	/*CRecordset cr(database);
	CString varValue, qStr, firstQueryString, secondQueryString;
	qStr = L"from skill where skill_group_id \
		= " + skill_groups[skill_group.GetCurSel()].skill_group_id + L" \
		and skill_id not in ( \
			select skill_id from requirement_skill where course_id = " + course_id + L" \
			union \
			select skill_id from received_skill where course_id = " + course_id + L") \
		order by 1 desc";
	firstQueryString = L"Select s.skill_id, s.name from ";
	secondQueryString = L" rs \
			left join skill s on rs.skill_id = s.skill_id \
			where rs.course_id = " + course_id + L" \
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
	}*/
}

// Заполнение окна умений
void TrainingListCreateEmployee::FillEmployeSkills() {
	/*CRecordset cr(database);
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
	}*/
}