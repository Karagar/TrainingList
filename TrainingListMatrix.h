#include "odbcinst.h"
#include "afxdb.h"
#pragma once


// Диалоговое окно TrainingListMatrix

class TrainingListMatrix : public CDialogEx
{
	DECLARE_DYNAMIC(TrainingListMatrix)

public:
	TrainingListMatrix(CWnd* pParent = nullptr);   // стандартный конструктор
	CDatabase* database;
	SkillGroup* skill_groups;
	Skill* requirement_skills, * received_skills, * employee_skills, * available_skills;
	Course* courses;
	Department* departments;
	Employee* employees_all;
	virtual ~TrainingListMatrix();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_MATRIX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString employee_id;
	CString skill_id;
	CString course_id;
	void SetDB(CDatabase* database);
	void RedrawDepartmentTab();
	void RedrawSkillGroupTab();
	void FillDepartments();
	void FillEmployees();
	void FillSkillGroups();
	void FillAllSkills();
	void FillEmployeeSkills();
	void FillCourses();
	void FillCourseSkills();
	void ClearAllWindows();
	void ClearBottomWindows();
	CTabCtrl skill_group;
	CTabCtrl department;
	CListCtrl employee;
	CListCtrl course;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickList4(NMHDR* pNMHDR, LRESULT* pResult);
	CListBox available_skill;
	CListBox employee_skill;
	afx_msg void OnLbnSelchangeList5();
	CListBox received_skill;
	CListBox requirement_skill;
	afx_msg void OnNMClickList7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult);
};
