#include "odbcinst.h"
#include "afxdb.h"
#pragma once


// Диалоговое окно TrainingListDepartmentsPositions

class TrainingListDepartmentsPositions : public CDialogEx
{
	DECLARE_DYNAMIC(TrainingListDepartmentsPositions)

public:
	TrainingListDepartmentsPositions(CWnd* pParent = nullptr);   // стандартный конструктор
	CDatabase* database;
	Department* departments;
	Position* positions;
	virtual ~TrainingListDepartmentsPositions();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_DEPARTMENTS_POSITIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListBox department;
	CListBox position;
	CString department_edit;
	CString position_edit;
	void SetDB(CDatabase* database);
	CString UpdateDepartment(CString department_id, CString to);
	CString DeleteDepartment(CString department_id);
	CString UpdatePosition(CString position_id, CString to);
	CString DeletePosition(CString position_id);
	void ResetControls(CString control_type);
	void FillDepartments();
	void FillPositions(CString department_id);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
