// TrainingListCourses.cpp: файл реализации
//

#include "pch.h"
#include "TrainingList.h"
#include "TrainingListDlg.h"
#include "TrainingListCourses.h"
#include "afxdialogex.h"


// Диалоговое окно TrainingListCourses

IMPLEMENT_DYNAMIC(TrainingListCourses, CDialogEx)

TrainingListCourses::TrainingListCourses(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAININGLIST_COURSES, pParent)
{

}

TrainingListCourses::~TrainingListCourses()
{
}

void TrainingListCourses::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, course);
	DDX_Control(pDX, IDC_LIST2, requirement_skill);
	DDX_Control(pDX, IDC_LIST3, received_skill);
}


BEGIN_MESSAGE_MAP(TrainingListCourses, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &TrainingListCourses::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &TrainingListCourses::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &TrainingListCourses::OnBnClickedButton3)
END_MESSAGE_MAP()


// Инициация диалога
BOOL TrainingListCourses::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CTrainingListDlg mainDlg;

	TRY{
		FillCourses();
	} CATCH(CDBException, e) {
		mainDlg.ReconnectDB();
		TRY{
			FillCourses();
		} CATCH(CDBException, e) {
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;
	}
	END_CATCH;
	UpdateData(false);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// Функция для передачи в этот класс БД
void TrainingListCourses::SetDB(CDatabase* database) {
	this->database = database;
}

// Заполнение списка подразделений
void TrainingListCourses::FillCourses() {
	CRecordset cr(database);
	CString varValue;

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from course");
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	cr.Open(CRecordset::forwardOnly, L"Select course_id, name from course order by 1 desc");
	courses = new Course[rowCount];
	course.ResetContent();
	int i = 0;

	while (!cr.IsEOF())
	{
		Course tmp{};
		cr.GetFieldValue(L"course_id", tmp.course_id);
		cr.GetFieldValue(L"name", tmp.course);
		cr.MoveNext();
		course.AddString(tmp.course);
		courses[i] = tmp;
		i++;
	}
	cr.Close();
	ResetControls(L"course");
}

// Сброс контроллов
void TrainingListCourses::ResetControls(CString control_type) {
	if (control_type == L"course")
	{
		course.SetCurSel(-1);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
	}
}

void TrainingListCourses::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void TrainingListCourses::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void TrainingListCourses::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
}
