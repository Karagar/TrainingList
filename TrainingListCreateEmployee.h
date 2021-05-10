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
	Skill *skills_employee, *skills_all;
	Department *departments;
	Position *positions;
	virtual ~TrainingListCreateEmployee();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_CREATE_EMPLOYEE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	CString employee_id;
	CString position_id;
	void SetDB(CDatabase* database);
	void RedrawTab();
	void FillSkillGroups(CString skill_group_id);
	void FillDepartment(CString skill_group_id);
	void FillPosition();
	void FillAllSkills();
	void FillEmployeSkills();
	void FillInputs();
	CString employee_name_edit;
	CTabCtrl skill_group;
	CListBox skill_all;
	CTabCtrl department;
	CListBox position;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	CListBox skill_employee;
	CString position_edit;
	afx_msg void OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLbnSelchangeList5();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLbnSelchangeList6();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
};
