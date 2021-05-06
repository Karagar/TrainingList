#include "odbcinst.h"
#include "afxdb.h"
#pragma once


// Диалоговое окно TrainingListCreateCourse

class TrainingListCreateCourse : public CDialogEx
{
	DECLARE_DYNAMIC(TrainingListCreateCourse)

public:
	TrainingListCreateCourse(CWnd* pParent = nullptr);   // стандартный конструктор
	CDatabase* database;
	SkillGroup* skill_groups;
	Skill *requirement_skills_selected, *skills_all, *received_skills_selected;
	virtual ~TrainingListCreateCourse();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_CREATE_COURSE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString course_id;
	void SetDB(CDatabase* database);
	void RedrawTab();
	void FillSkillGroups(CDatabase* database);
	void FillSkills(CDatabase* database);
	CTabCtrl skill_group;
	CListBox received_skill_selected;
	CListBox requirement_skill_selected;
	CListBox skill_all;
};
