//Vidám Szabolcs 514/2 vsim2414

#ifndef __DBL__LINKED__LIST__H__
#define __DBL__LINKED__LIST__H__

#include <string>
#include <fstream>

class DBLLinkedList
{
private:
	int size;
public:
	struct Node
	{
		struct Song
		{
			std::wstring title;
			std::wstring artist;
			std::string path;
		} data;
		Node* previous = nullptr;
		Node* next = nullptr;
	};
	Node* headNode;
	
	DBLLinkedList() : size(0), headNode(nullptr) {};
	DBLLinkedList(std::wifstream&);
	DBLLinkedList(int);
	~DBLLinkedList();

	void PrintList();
	void PrintListCursor(DBLLinkedList::Node*);
	void PrintListReverse();

	Node* GetNode(int);
	DBLLinkedList::Node::Song GetData(int);
	void SetValue(int, DBLLinkedList::Node::Song);
	
	void InsertValue(int, DBLLinkedList::Node::Song);
	void DeleteNode(int);
	void SwapValues(int, int);
	
	bool IsEmpty();
	int GetSize();
	int GetIndex(DBLLinkedList::Node*);
	
	void ShuffleList();
	bool IsSorted(const std::string&);
	void SortList(const std::string&);
};

#endif