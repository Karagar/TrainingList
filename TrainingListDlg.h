
// TrainingListDlg.h: файл заголовка
//

#pragma once


// Диалоговое окно CTrainingListDlg
class CTrainingListDlg : public CDialogEx
{
// Создание
public:
	CTrainingListDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	void ReconnectDB();
};

struct Department
{
	CString department_id;
	CString department;
};

struct Position
{
	CString position_id;
	CString position;
};

struct Employee
{
	CString employee_id;
	CString position;
	CString employee;
};

struct SkillGroup
{
	CString skill_group_id;
	CString skill_group;
};

struct Skill
{
	CString skill_id;
	CString skill;
};

struct Course
{
	CString course_id;
	CString duration;
	CString course;
};