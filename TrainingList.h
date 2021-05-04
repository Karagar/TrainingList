
// TrainingList.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTrainingListApp:
// Сведения о реализации этого класса: TrainingList.cpp
//

class CTrainingListApp : public CWinApp
{
public:
	CTrainingListApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTrainingListApp theApp;
