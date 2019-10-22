#pragma once
#include <assert.h>
#include <initializer_list>
#include <iostream>
#include <string>
#include <sstream>


#define TABLE_CAPACITY 16

using namespace std;



template <class T>
struct KHashEntry
{
	int key;
	T value;

	KHashEntry(int key, T value) :key(key), value(value) {}
	KHashEntry(const KHashEntry& Entry) :key(Entry.key), value(Entry.value) {}
	string ToString()
	{
		stringstream ss;
		ss << key << '\t' << value;
		string str_key, str_value;
		ss >> str_key >> str_value;
		str_key += "    " + str_value;
		return str_key;
	}
};





int hash_func(int key)
{
	return key % TABLE_CAPACITY;
}



template <class T>
class Visitor;

template <class T>
class MinVisitor;

template <class T>
class MaxVisitor;


template <class T>
class KHashTable
{
public:
	KHashEntry<T> * *table;
	int size;
	int used[TABLE_CAPACITY];

	KHashTable()
	{
		table = new KHashEntry<T>*[TABLE_CAPACITY];
		for (int i = 0; i < TABLE_CAPACITY; ++i)
		{
			table[i] = NULL;
			used[i] = false;
		}
		size = 0;
	}


	KHashTable(const KHashTable& Table)
	{
		table = new KHashEntry<T>*[TABLE_CAPACITY];
		for (int i = 0; i < TABLE_CAPACITY; ++i)
		{
			if (Table.table[i] == NULL)
				table[i] = NULL;
			else
				table[i] = new KHashEntry<T>(*Table.table[i]);
			used[i] = Table.used[i];
		}
		size = Table.size;
	}


	KHashTable(KHashTable&& Table)
	{
		KHashTable();
		Swap(*this, Table);
	}


	KHashTable(const initializer_list<T>& List)
	{
		assert(List.size() <= TABLE_CAPACITY);     //список инициализации не может быть длиннее TABLE_CAPACITY
		table = new KHashEntry<T>*[TABLE_CAPACITY];
		for (int i = 0; i < TABLE_CAPACITY; ++i)
		{
			table[i] = NULL;
		}
		int i = 0;
		for (auto it = List.begin(); it != List.end(); ++it)
		{
			Insert(i, *it);
			++i;
		}
		size = List.size();
	}


	bool Insert(int key, T value)
	{
		if (size < TABLE_CAPACITY)
		{
			int hash = hash_func(key);
			while (table[hash] != NULL && table[hash]->key != key)
				hash = hash_func(hash + 1);
			if (table[hash] != NULL)
				delete table[hash];
			table[hash] = new KHashEntry<T>(key, value);
			++size;
			used[hash] = true;
			return true;
		}
		else
		{
			return false;
		}
	}


	bool Insert(const KHashEntry<T>& Entry)
	{
		if (size < TABLE_CAPACITY)
		{
			int hash = hash_func(Entry.key);
			while (table[hash] != NULL && table[hash]->key != Entry.key)
				hash = hash_func(hash + 1);
			if (table[hash] != NULL)
			{
				delete table[hash];
				--size;
			}
			table[hash] = new KHashEntry<T>(Entry);
			++size;
			return true;
		}
		else
		{
			return false;
		}
	}


	template<class ... Args>
	bool Emplace(int key, const Args& ... args)
	{
		if (size < TABLE_CAPACITY)
		{
			Insert(key, T(args...));
			return true;
		}
		else
			return false;
	}


	int Size()
	{
		return size;
	}


	~KHashTable()
	{
		for (int i = 0; i < TABLE_CAPACITY; ++i)
			if (table[i] != NULL)
				delete table[i];
		delete[] table;
	}


	void Clear()
	{
		for (int i = 0; i < TABLE_CAPACITY; ++i)
		{
			if (table[i] != NULL)
			{
				delete table[i];
				table[i] = NULL;
			}
			used[i] = false;
		}
		size = 0;
	}


	bool IsEmpty()
	{
		if (size)
			return false;
		else
			return true;
	}


	bool IsFull()
	{
		if (size == TABLE_CAPACITY)
			return true;
		else
			return false;
	}


	T Delete(const int key)
	{
		int hash = hash_func(key);
		while ((used[hash] && table[hash] == NULL) || table[hash] != NULL && table[hash]->key != key)
			hash = hash_func(hash + 1);
		assert(table[hash] != NULL);   //нет значения с таким ключом
		T return_value = table[hash]->value;
		delete table[hash];
		table[hash] = NULL;
		--size;
		return return_value;
	}


	T& Find(const int& key)
	{
		int hash = hash_func(key);
		while ((used[hash] && table[hash] == NULL) || table[hash] != NULL && table[hash]->key != key)
			hash = hash_func(hash + 1);
		assert(table[hash] != NULL);   //нет значения с таким ключом
		return table[hash]->value;
	}


	KHashTable& operator=(const KHashTable& Table)
	{
		if (*this != Table) {
			Clear();
			table = new KHashEntry<T>*[TABLE_CAPACITY];
			for (int i = 0; i < TABLE_CAPACITY; ++i)
			{
				if (Table.table[i] == NULL)
					table[i] = NULL;
				else
					table[i] = new KHashEntry<T>(*Table.table[i]);
				used[i] = Table.used[i];
			}
			size = Table.size;
		}
		return *this;
	}


	KHashTable& operator=(KHashTable&& Table)
	{
		if (*this != Table)
		{
			Clear();
			Swap(*this, Table);
		}
		return *this;
	}


	T& operator[](const int& key)
	{
		int hash = hash_func(key);
		while ((used[hash] && table[hash] == NULL) || table[hash] != NULL && table[hash]->key != key)
			hash = hash_func(hash + 1);
		assert(table[hash] != NULL);   //нет значения с таким ключом
		return table[hash]->value;
	}


	friend void Swap(KHashTable& Table1, KHashTable& Table2)
	{
		swap(Table1.size, Table2.size);

		swap(Table1.table, Table2.table);

		for (int i = 0; i < TABLE_CAPACITY; ++i)
			swap(Table1.used[i], Table2.used[i]);
	}


	friend ostream& operator<<(ostream& os, const KHashTable& Table)
	{
		if (Table.size)
		{
			for (int i = 0; i < TABLE_CAPACITY; ++i)
			{
				if (Table.table[i] != NULL)
					os << Table.table[i]->key << ' ' << Table.table[i]->value << '\n';
			}
		}
		else
			os << "- -\n";
		return os;
	}


	friend istream& operator>>(istream& is, KHashTable& Table)
	{
		int key, n;
		cin >> n;
		T value;
		for (int i = 0; i < n; ++i)
		{
			is >> key >> value;
			Table.Insert(key, value);
		}
		return is;
	}




	class iterator
	{
	public:

		const KHashTable * tab;
		//лучше использовать номер в таблице
			//или переделать ++
		KHashEntry<T> * ptr;


		iterator() :tab(NULL), ptr(NULL) {}
		iterator(const KHashTable *Table) : tab(Table), ptr(NULL) {}
		iterator(const KHashTable Table, const KHashEntry<T> Entry) :tab(Table), ptr(&Entry) {}
		iterator(const iterator& I) :tab(I.tab), ptr(I.ptr) {}

		bool operator==(const iterator& I) const
		{
			return (tab == I.tab && ptr == I.ptr);
		}
		bool operator!=(const iterator& I) const
		{
			return (tab != I.tab || ptr != I.ptr);
		}
		KHashEntry<T>& operator*() const
		{
			assert(tab != NULL && ptr != NULL);
			return *ptr;
		}
		iterator operator++(int)
		{
			//нужно реализовать через номер в таблице
			iterator it = *this;
			int this_hash = hash_func(ptr->key);
			int i = this_hash + 1;
			while (i < TABLE_CAPACITY)
			{
				if (tab->table[i] != NULL)
					break;
				++i;
			}
			if (i == TABLE_CAPACITY)
			{
				ptr = NULL;
				return *this;
			}
			ptr = tab->table[i];
			return it;
		}
		iterator operator++()
		{
			int this_hash = hash_func(ptr->key);
			int i = this_hash + 1;
			while (i < TABLE_CAPACITY)
			{
				if (tab->table[i] != NULL)
					break;
				++i;
			}
			if (i == TABLE_CAPACITY)
			{
				ptr = NULL;
				return *this;
			}
			ptr = tab->table[i];
			return *this;
		}
	};

	iterator CreateIterator() const
	{
		return iterator(this);
	}

	iterator begin()
	{
		int i = 0;
		while (i < TABLE_CAPACITY)
		{
			if (table[i] != NULL)
				break;
			++i;
		}
		iterator it = CreateIterator();
		if (i == TABLE_CAPACITY)
		{
			it.ptr = NULL;
			return it;
		}
		it.ptr = table[i];
		return it;
	}
	iterator end()
	{
		iterator it = CreateIterator();
		return it;
	}

	T imin()
	{
		assert(size);
		T value = (*begin()).value;
		for (iterator it = begin(); it != end(); ++it)
		{
			if ((*it).value < value)
				value = (*it).value;
		}
		return value;
	}
	T imax()
	{
		assert(size);
		T value = (*begin()).value;
		for (iterator it = begin(); it != end(); ++it)
		{
			if ((*it).value > value)
				value = (*it).value;
		}
		return value;
	}


	T accept(Visitor<T>& V)
	{
		return V.visit(*this);
	}

	T vmin()
	{
		MinVisitor<T> minvis;
		T mv = accept(minvis);
		return mv;
	}
	T vmax()
	{
		MaxVisitor<T> maxvis;
		T mv = accept(maxvis);
		return mv;
	}
};



template<class T>
class Visitor
{
public:
	virtual T visit(KHashTable<T>& T) = 0;
	virtual ~Visitor() {}

protected:
	Visitor() {}
};

template <class T>
class MinVisitor :public Visitor<T>
{
public:

	T value;
	T visit(KHashTable<T>& Table)
	{
		assert(Table.size);
		int i = 0;
		while (Table.table[i] == NULL)
			++i;
		value = Table.table[i]->value;
		while (i < TABLE_CAPACITY)
		{
			if (Table.table[i] != NULL && Table.table[i]->value < value)
				value = Table.table[i]->value;
			++i;
		}
		return value;
	}
	MinVisitor() {}
	~MinVisitor() {}
};

template <class T>
class MaxVisitor :public Visitor<T>
{
public:

	T value;
	T visit(KHashTable<T>& Table)
	{
		assert(Table.size);
		int i = 0;
		while (Table.table[i] == NULL)
			++i;
		value = Table.table[i]->value;
		while (i < TABLE_CAPACITY)
		{
			if (Table.table[i] != NULL && Table.table[i]->value > value)
				value = Table.table[i]->value;
			++i;
		}
		return value;
	}
	MaxVisitor() {}
	~MaxVisitor() {}
};
