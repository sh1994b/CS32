#include "Map.h"

Map::Map()
{
	m_size = 0;
}

bool Map::empty() const
{
	if (m_size == 0)
		return true;
	else
		return false;
}

int Map::size() const
{
	return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	bool alreadyExists = 0;
	for (int i = 0; i < m_size && alreadyExists == 0; i++)
	{
		if (pair[i].key == key)
		{
			alreadyExists = 1;
		}
	}
	if (alreadyExists == 0)
	{
		if (m_size < DEFAULT_MAX_ITEMS)
		{
			pair[m_size].key = key;
			pair[m_size].val = value;
			m_size++;
			return true;
		}
		else
			return false;
	}
	else
		return false;

}

bool Map::update(const KeyType& key, const ValueType& value)
{
	bool alreadyExists = 0;
	for (int i = 0; i < m_size && alreadyExists == 0; i++)
	{
		if (pair[i].key == key)
		{
			alreadyExists = 1;
			pair[i].val = value;
		}
	}
	if (alreadyExists == 1)
		return true;
	else
		return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	bool alreadyExists = 0;
	for (int i = 0; i < m_size && alreadyExists == 0; i++)
	{
		if (pair[i].key == key)
		{
			alreadyExists = 1;
			pair[i].val = value;
		}
	}
	if (alreadyExists == 0)
	{
		if (m_size < DEFAULT_MAX_ITEMS)
		{
			pair[m_size].key = key;
			pair[m_size].val = value;
			m_size++;
			return true;
		}
		else
			return false;
	}
	else return true;

}

bool Map::erase(const KeyType& key)
{
	//check for bugs caused by this!

	bool alreadyExists = 0;
	for (int i = 0; i < m_size && alreadyExists == 0; i++)
	{
		if (pair[i].key == key)
		{
			alreadyExists = 1;
			SType swap;
			swap.key = pair[i].key;
			swap.val = pair[i].val;
			pair[i].key = pair[m_size - 1].key;
			pair[i].val = pair[m_size - 1].val;
			pair[m_size - 1].key = swap.key;
			pair[m_size - 1].val = swap.val;
			m_size--;
		}
	}
	if (alreadyExists == 1)
		return true;
	else
		return false;
}

bool Map::contains(const KeyType& key) const
{
	bool alreadyExists = 0;
	for (int i = 0; i < m_size && alreadyExists == 0; i++)
	{
		if (pair[i].key == key)
		{
			alreadyExists = 1;
		}
	}
	if (alreadyExists == 1)
		return true;
	else
		return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	bool alreadyExists = 0;
	for (int i = 0; i < m_size && alreadyExists == 0; i++)
	{
		if (pair[i].key == key)
		{
			alreadyExists = 1;
			value = pair[i].val;
		}
	}
	if (alreadyExists == 1)
		return true;
	else
		return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i >= 0 && i < m_size)
	{
		key = pair[i].key;
		value = pair[i].val;
		return true;
	}
	else
		return false;
}

void Map::swap(Map& other)
{
	int max_size;
	SType dummy;
	int dummy_int;
	if (m_size > other.m_size)
		max_size = m_size;
	else
		max_size = other.m_size;
	for (int i = 0; i < max_size; i++)
	{
		dummy = pair[i];
		pair[i] = other.pair[i];
		other.pair[i] = dummy;
	}
	dummy_int = m_size;
	m_size = other.m_size;
	other.m_size = dummy_int;
}