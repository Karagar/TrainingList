﻿// TrainingListCourses.cpp: файл реализации
//

#include "pch.h"
#include "TrainingList.h"
#include "TrainingListDlg.h"
#include "TrainingListCourses.h"
#include "TrainingListCreateCourse.h"
#include "afxdialogex.h"


TrainingListCreateCourse create_course_page;

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
	ON_LBN_SELCHANGE(IDC_LIST1, &TrainingListCourses::OnLbnSelchangeList1)
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
	create_course_page.SetDB(database);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// Функция для передачи в этот класс БД
void TrainingListCourses::SetDB(CDatabase* database) {
	this->database = database;
}

// Заполнение списка курсов
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

// Заполнение списка скилов
void TrainingListCourses::FillSkills(CString course_id) {
	CRecordset cr(database);
	CString varValue;
	CString firstQueryString = L"Select s.skill_id, s.name from ";
	CString secondQueryString = L" rs \
		left join skill s on rs.skill_id = s.skill_id \
		where rs.course_id = " + course_id + " \
		order by 1 desc";

	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from requirement_skill where course_id = " + course_id);
	cr.GetFieldValue(L"cnt", varValue);
	int rowCount = _ttoi(varValue);
	cr.Close();

	requirement_skill.ResetContent();
	if (rowCount > 0) {
		cr.Open(CRecordset::forwardOnly, firstQueryString + L"requirement_skill" + secondQueryString);
		requirement_skills = new Skill[rowCount];
		int i = 0;

		while (!cr.IsEOF())
		{
			Skill tmp{};
			cr.GetFieldValue(L"skill_id", tmp.skill_id);
			cr.GetFieldValue(L"name", tmp.skill);
			cr.MoveNext();
			requirement_skill.AddString(tmp.skill);
			requirement_skills[i] = tmp;
			i++;
		}
		cr.Close();
	}
	
	cr.Open(CRecordset::forwardOnly, L"Select count(*) cnt from received_skill where course_id = " + course_id);
	cr.GetFieldValue(L"cnt", varValue);
	rowCount = _ttoi(varValue);
	cr.Close();

	received_skill.ResetContent();
	if (rowCount > 0) {
		cr.Open(CRecordset::forwardOnly, firstQueryString + L"received_skill" + secondQueryString);
		received_skills = new Skill[rowCount];
		int i = 0;

		while (!cr.IsEOF())
		{
			Skill tmp{};
			cr.GetFieldValue(L"skill_id", tmp.skill_id);
			cr.GetFieldValue(L"name", tmp.skill);
			cr.MoveNext();
			received_skill.AddString(tmp.skill);
			received_skills[i] = tmp;
			i++;
		}
		cr.Close();
	}
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
	INT_PTR returnCode = -1;
	create_course_page.course_id = "0";
	returnCode = create_course_page.DoModal();
}


void TrainingListCourses::OnBnClickedButton2()
{
	if (course.GetCurSel() >= 0) {
		INT_PTR returnCode = -1;
		create_course_page.course_id = courses[course.GetCurSel()].course_id;
		returnCode = create_course_page.DoModal();
	}
	else {
		GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	}
}


void TrainingListCourses::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void TrainingListCourses::OnLbnSelchangeList1()
{
	if (course.GetCurSel() >= 0) {
		TRY{
			FillSkills(courses[course.GetCurSel()].course_id);
		} CATCH(CDBException, e) {
			CTrainingListDlg mainDlg;
			mainDlg.ReconnectDB();
			TRY{
				FillSkills(courses[course.GetCurSel()].course_id);
			} CATCH(CDBException, e) {
				AfxMessageBox(L"Database error: " + e->m_strError);
			}
			END_CATCH;
		}
		END_CATCH;
		UpdateData(false);

		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
	}
}
