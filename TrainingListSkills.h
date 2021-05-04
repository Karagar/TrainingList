#include "odbcinst.h"
#include "afxdb.h"
#pragma once


// Диалоговое окно TrainingListSkills

class TrainingListSkills : public CDialogEx
{
	DECLARE_DYNAMIC(TrainingListSkills)

public:
	TrainingListSkills(CWnd* pParent = nullptr);   // стандартный конструктор
	CDatabase* database;
	SkillGroup* skill_groups;
	Skill* skills;
	virtual ~TrainingListSkills();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_SKILLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListBox skill_group;
	CListBox skill;
	CString skill_group_edit;
	CString skill_edit;
	void SetDB(CDatabase* database);
	CString UpdateSkillGroup(CString skill_group_id, CString to);
	CString DeleteSkillGroup(CString skill_group_id);
	CString UpdateSkill(CString skill_id, CString to);
	CString DeleteSkill(CString skill_id);
	void FillSkillGroups();
	void FillSkills(CString skill_group_id);
	void ResetControls(CString control_type);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();
};
