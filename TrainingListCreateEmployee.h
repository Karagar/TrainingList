#include "odbcinst.h"
#include "afxdb.h"
#pragma once


// Диалоговое окно TrainingListCreateEmployee

class TrainingListCreateEmployee : public CDialogEx
{
	DECLARE_DYNAMIC(TrainingListCreateEmployee)

public:
	TrainingListCreateEmployee(CWnd* pParent = nullptr);   // стандартный конструктор
	CDatabase* database;
	SkillGroup* skill_groups;
	Skill* requirement_skills_selected, * skills_all;
	virtual ~TrainingListCreateEmployee();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_CREATE_EMPLOYEE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString employe_id;
	void SetDB(CDatabase* database);
	void RedrawTab();
	void FillSkillGroups(CString skill_group_id);
	void FillAllSkills();
	void FillEmployeSkills();
};
