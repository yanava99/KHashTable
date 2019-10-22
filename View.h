#pragma once

#include "Model.h"
#include <Windows.h>
#include "resource.h"


template <class T>
class View
{
private:
	Model<T>* model;
	HWND hwnd;

public:
	View(Model<T>& model, HWND hwnd) :model(&model), hwnd(hwnd) {}
	void SetHWND(HWND hwnd)
	{
		this->hwnd = hwnd;
	}


	void Update()
	{
		HWND hwndList = GetDlgItem(hwnd, IDC_LIST);
		SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
		KHashTable<T> Table(model->GetData());
		int i = 0;
		for (auto it = Table.begin(); it != Table.end(); ++it)
		{
			SendMessage(hwndList, LB_INSERTSTRING, 0, (LPARAM)((*it).ToString().c_str()));
			++i;
		}
		stringstream ss;
		ss << model->min_v();
		string str;
		ss >> str;
		SetWindowText(GetDlgItem(hwnd, IDC_STATIC_MIN), str.c_str());
	}

	~View() {}
};
