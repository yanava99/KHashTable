#pragma once

#include "View.h"

template <class T>
class Controller
{
protected:
	Model<T>* model;
	View<T>* view;
public:
	Controller(Model<T>& model, View<T>& view) :model(&model), view(&view) {}

	void add_controller(int key, T value)
	{
		model->add_entry(key, value);
		view->Update();
	}

	void delete_controller(int key)
	{
		model->delete_entry(key);
		view->Update();
	}

	~Controller() {}
};
