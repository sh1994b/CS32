#include <algorithm>
#include <string>
#include <iostream>
using namespace std;

template <class KeyType, class ValueType>
class MyHash
{
public:
	MyHash(double maxLoadFactor = 0.5);
	~MyHash();
	void reset();
	void associate(const KeyType& key, const ValueType& value);
	int getNumItems() const;
	double getLoadFactor() const;

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyHash(const MyHash&) = delete;
	MyHash& operator=(const MyHash&) = delete;


private:
	unsigned int getBucketNumber(const KeyType& key)    const
	{
		unsigned int myHash(const KeyType& k);        //  prototype; had to change hash to myHash because of std
		unsigned int h = myHash(key);
		return h % m_arraySize;
	}
	void cleanUp();
	struct Node
	{
		KeyType   m_key;
		ValueType m_value;
		Node*     m_next;
	};
	void addNode(Node* &head, const KeyType key, const ValueType value);
	double m_maxLoadFactor;
	Node** m_buckets;
	int m_elementsNo;
	int m_arraySize;
};


template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
	:m_arraySize(100), m_elementsNo(0)
{
	m_buckets = new Node*[100];

	for (int i = 0; i < 100; i++)	//initialize hash table to nullptr
		m_buckets[i] = nullptr;

	if (maxLoadFactor <= 0)
		m_maxLoadFactor = .5;
	else if (maxLoadFactor > 2)
		m_maxLoadFactor = 2.0;
	else
		m_maxLoadFactor = maxLoadFactor;

}


template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::cleanUp()
{
	for (int i = 0; i < m_arraySize; i++)    //deletes linked list inside each bucket
	{
		Node* p = m_buckets[i]; //p points to head of linked list
		while (p != nullptr)
		{
			m_buckets[i] = p->m_next;
			delete p;
			p = m_buckets[i];
		}
	}

	delete[] m_buckets;
	m_elementsNo = 0;
}


template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
	cleanUp();
}


template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
	cleanUp();  //deallocate current hashtable
	m_buckets = new Node*[100];     //make new empty hashtable of 100
	for (int i = 0; i < 100; i++)
		m_buckets[i] = nullptr;
	m_arraySize = 100;
}


template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	ValueType* valPtr = find(key);
	if (valPtr != nullptr)  //key exists so we must update its value
		*valPtr = value;


	else    //key doesn't exist so we must add it 
	{
		//if adding one key causes load factor to exceed max load factor:
		if ((double)(m_elementsNo + 1) / (double)m_arraySize > m_maxLoadFactor)
		{
			Node** newHash = new Node*[m_arraySize * 2];    //make a new hashable double the size
			for (int i = 0; i < m_arraySize * 2; i++)    //initialize the new hashtable to nullptr's
				newHash[i] = nullptr;
			Node** temp = m_buckets;    //temporary pointer to old hashtable
			m_buckets = newHash;    //data member m_buckets points to the new hashtable
			m_elementsNo = 0;       //currently the hashtable is empty
			m_arraySize *= 2;
			for (int k = 0; k < m_arraySize / 2; k++)
			{
				//call associate for each item in old hashtable
				Node* q = temp[k];
				while (q != nullptr)
				{
					associate(q->m_key, q->m_value);  //add each pair to new hashtable
					temp[k] = q->m_next;
					delete q;       //then delete each copied node
					q = temp[k];
				}
			}
			delete[] temp;

		}

		int i = getBucketNumber(key);   //number of bucket it should be added to
		addNode(m_buckets[i], key, value);
	}
}


template <typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
	int i = getBucketNumber(key);   //find out what bucket to look into
									//loop thru linked list of that bucket to find key
	Node* p;
	for (p = m_buckets[i]; p != nullptr; p = p->m_next)
	{
		if (p->m_key == key)
			break;
	}
	if (p == nullptr)   //key wasn't found
		return nullptr;
	else    //key was found and p is pointing to its Node
	{
		ValueType* valPtr = &(p->m_value);
		return valPtr;
	}
}


template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::addNode(Node* &head, KeyType key, ValueType value)
{
	m_elementsNo++;
	Node* newNode = new Node;
	newNode->m_key = key;
	newNode->m_value = value;
	newNode->m_next = nullptr;

	if (head == nullptr)    //if list is empty
	{
		head = newNode;
		return;
	}
	Node* p = nullptr;
	for (p = head; p->m_next != nullptr; p = p->m_next)
		;   //get to last Node of linked list
	p->m_next = newNode;
}


template <typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
	return m_elementsNo;
}

template <typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
	return (double)m_elementsNo / (double)m_arraySize;
}