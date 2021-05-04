#include "odbcinst.h"
#include "afxdb.h"
#pragma once


// Диалоговое окно TrainingListCourses

class TrainingListCourses : public CDialogEx
{
	DECLARE_DYNAMIC(TrainingListCourses)

public:
	TrainingListCourses(CWnd* pParent = nullptr);   // стандартный конструктор
	CDatabase* database;
	Course* courses;
	Skill *requirement_skills, *received_skills;
	virtual ~TrainingListCourses();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_COURSES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void SetDB(CDatabase* database);
	void FillCourses();
	void FillSkills(CString course_id);
	void ResetControls(CString control_type);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CListBox course;
	CListBox requirement_skill;
	CListBox received_skill;
	afx_msg void OnLbnSelchangeList1();
};
