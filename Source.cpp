#include "Controller.h"

#include <string>
#include "resource.h"
#include <Windows.h>
#include <windowsx.h>

using namespace std;



class Student
{
	string name;

public:
	Student()
	{
		name = "\n";
	}
	Student(string name) :name(name) {}

	bool operator>(const Student& st)
	{
		return name > st.name;
	}

	bool operator<(const Student& st)
	{
		return name < st.name;
	}

	friend ostream& operator<<(ostream& os, const Student& st)
	{
		os << st.name;
		return os;
	}

	friend istream& operator>>(istream& is, Student& st)
	{
		is >> st.name;
		return is;
	}
};


HINSTANCE hInst;
HWND hwndList;
Model<Student> model;
View<Student> view(model, hwndList);
Controller<Student> controller(model, view);


BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL DlgOnInit(HWND, HWND, LPARAM);
void DlgOnCommand(HWND, int, HWND, UINT);
void DlgOnClose(HWND);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	model.add_entry(1, Student("Alice"));
	model.add_entry(2, Student("Mary"));
	model.add_entry(3, Student("John"));
	model.add_entry(4, Student("Kate"));
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL, DlgProc);
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hwnd, WM_INITDIALOG, DlgOnInit);
		HANDLE_MSG(hwnd, WM_COMMAND, DlgOnCommand);
		HANDLE_MSG(hwnd, WM_CLOSE, DlgOnClose);
		HANDLE_MSG(hwnd, WM_DESTROY, DlgOnClose);

	default:
		return FALSE;
	}
	return TRUE;
}

BOOL DlgOnInit(HWND hwnd, HWND, LPARAM)
{
	hwndList = GetDlgItem(hwnd, IDC_LIST);
	view.SetHWND(hwnd);
	view.Update();
	return TRUE;
}

void DlgOnCommand(HWND hwnd, int id, HWND, UINT)
{
	switch (id)
	{
	case IDC_BUTTON_ADD:
	{
		int key;
		string name;
		char buf[250] = "";
		HWND hEd = GetDlgItem(hwnd, IDC_EDIT_KEY);
		GetWindowText(hEd, buf, 250);
		key = atoi(buf);
		hEd = GetDlgItem(hwnd, IDC_EDIT_VALUE);
		GetWindowText(hEd, buf, 250);
		name = buf;
		controller.add_controller(key, Student(name));
	}
	break;
	case IDC_BUTTON_DELETE:
	{
		if (SendMessage(hwndList, LB_GETCURSEL, 0, 0) != LB_ERR)
		{
			int number = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
			char buf[180];
			SendMessage(hwndList, LB_GETTEXT, number, (LPARAM)buf);
			string str = buf;
			string str_key = str.substr(0, str.find(' '));
			controller.delete_controller(atoi(str_key.c_str()));
		}
	}
	break;
	default:
		break;
	}
}

void DlgOnClose(HWND hwnd)
{
	DestroyWindow(hwnd);
}

