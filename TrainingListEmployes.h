#include "odbcinst.h"
#include "afxdb.h"
#pragma once


// Диалоговое окно TrainingListEmployes

class TrainingListEmployes : public CDialogEx
{
	DECLARE_DYNAMIC(TrainingListEmployes)

public:
	TrainingListEmployes(CWnd* pParent = nullptr);   // стандартный конструктор
	CDatabase *database;
	Department *departments;
	virtual ~TrainingListEmployes();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_EMPLOYES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl employee;
	void SetDB(CDatabase* database);
	void PaintEmployes();
	void RedrawTab();
	void FillDepartments(CDatabase* db);
	void FillEmployees(CDatabase* db, CString departmentStr);
	CTabCtrl department;
	afx_msg void OnChangeActiveTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedList4(NMHDR* pNMHDR, LRESULT* pResult);
};
