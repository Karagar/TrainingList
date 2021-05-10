// TrainingListMatrix.cpp: файл реализации
//

#include "pch.h"
#include "TrainingList.h"
#include "TrainingListMatrix.h"
#include "afxdialogex.h"


// Диалоговое окно TrainingListMatrix

IMPLEMENT_DYNAMIC(TrainingListMatrix, CDialogEx)

TrainingListMatrix::TrainingListMatrix(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAININGLIST_MATRIX, pParent)
{

}

TrainingListMatrix::~TrainingListMatrix()
{
}

void TrainingListMatrix::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TrainingListMatrix, CDialogEx)
END_MESSAGE_MAP()


// Обработчики сообщений TrainingListMatrix
