#pragma once

#include "KHashTable.h"

template <class T>
class Model
{
private:
	KHashTable<T> data;

public:
	Model() {}

	KHashTable<T> GetData()
	{
		return data;
	}

	void add_entry(int key, T value)
	{
		data.Insert(key, value);
	}

	void delete_entry(int key)
	{
		data.Delete(key);
	}

	T min_v()
	{
		return data.vmin();
	}

	~Model() {}
};

