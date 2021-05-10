#pragma once


// Диалоговое окно TrainingListMatrix

class TrainingListMatrix : public CDialogEx
{
	DECLARE_DYNAMIC(TrainingListMatrix)

public:
	TrainingListMatrix(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~TrainingListMatrix();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAININGLIST_MATRIX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
