//Vidám Szabolcs 514/2 vsim2414

#include <iostream>
#include "DBLLinkedList.h" //Own custom datatype

#include <fstream> //Reading from file
#include <sstream> //Detecting input with delimiter

#include <cctype> //For transform()
#include <algorithm> //For ::tolower

#include <ctime> //For time()
#include <cstdlib> //For rand() and srand()
#define SHUFFLE_NUM 100 //How many swaps to make in a shuffle

using namespace std;

DBLLinkedList::DBLLinkedList(int size) //Creates a doubly linked list of length 'size'
{
	DBLLinkedList::size = size;

	DBLLinkedList::Node* headNode = new DBLLinkedList::Node();
	headNode->previous = nullptr;

	DBLLinkedList::Node* currentNode = headNode;
	for (int i = 0; i < size - 1; i++)
	{
		DBLLinkedList::Node* newNode = new DBLLinkedList::Node();
		newNode->previous = currentNode;
		currentNode->next = newNode;
		currentNode = newNode;
	}

	currentNode->next = nullptr;
	DBLLinkedList::headNode = headNode;
}

DBLLinkedList::DBLLinkedList(wifstream& inputFile) : size(0), headNode(nullptr)
{
	headNode = new Node;
	headNode->previous = nullptr;
	headNode->next = nullptr;

	Node* currentNode = headNode;
	wstring line;

	if (inputFile.is_open())
	{
		while (getline(inputFile, line))
		{
			wstringstream ss(line);
			currentNode->data = Node::Song(); // Initialize data to avoid garbage values
		
			getline(ss, currentNode->data.title, L'|');
			getline(ss, currentNode->data.artist, L'|');

			wstring path; //Auxiliary
			getline(ss, path, L'|');
			currentNode->data.path = string(path.begin(), path.end()); //Convert wstring to string

			if (!inputFile.eof())
			{
				currentNode->next = new Node; // Allocate memory for the next node
				currentNode->next->previous = currentNode; // Set the previous pointer of the next node
				currentNode = currentNode->next;
			}

			size++;
		}
		inputFile.close();
	}
	else
	{
		cerr << "Error: Unable to open file." << endl;
	}
}

DBLLinkedList::~DBLLinkedList()
{
	DBLLinkedList::Node* CurrentNode = headNode;
	while (CurrentNode)
	{
		DBLLinkedList::Node* NextNode = CurrentNode->next;
		delete CurrentNode;
		CurrentNode = NextNode;
	}
	headNode = nullptr;
	size = 0;
}

void DBLLinkedList::PrintList()
{
	if (IsEmpty())
	{
		cout << "There are no songs in the playlist..." << endl;
		return;
	}

	locale::global(locale(""));
	DBLLinkedList::Node* currentNode = headNode;
	int k = 1;
	while (currentNode)
	{
		wcout << k << ". " << currentNode->data.title << " by " << currentNode->data.artist
			<< ": ";
		cout << currentNode->data.path << endl;
		currentNode = currentNode->next;
		k++;
	}
	cout << endl;
}

void DBLLinkedList::PrintListCursor(DBLLinkedList::Node* cursorNode)
{
	if (IsEmpty())
	{
		cout << "There are no songs in the playlist..." << endl;
		return;
	}

	locale::global(locale(""));
	DBLLinkedList::Node* currentNode = headNode;
	int k = 1;
	while (currentNode)
	{
		if (currentNode == cursorNode)
		{
			cout << " > ";
		}
		else
		{
			cout << k << ". ";
		}
		wcout << currentNode->data.title << " by " << currentNode->data.artist
			<< ": ";
		cout << currentNode->data.path << endl;
		currentNode = currentNode->next;
		k++;
	}
	cout << endl;
}

void DBLLinkedList::PrintListReverse()
{
	if (IsEmpty())
	{
		cout << "There are no songs in the playlist..." << endl;
		return;
	}

	DBLLinkedList::Node* currentNode = headNode;
	do
	{
		currentNode = currentNode->next;
	} while (currentNode->next);
	
	while (currentNode)
	{
		wcout << currentNode->data.title << " by " << currentNode->data.artist
			<< ": ";
		cout << currentNode->data.path << endl;
		currentNode = currentNode->previous;
	}
	cout << endl;
}

DBLLinkedList::Node* DBLLinkedList::GetNode(int index)
{
	DBLLinkedList::Node* currentNode = headNode;
	for (int i = 0; i < index; i++)
	{
		currentNode = currentNode->next;
	}

	return currentNode;
}

DBLLinkedList::Node::Song DBLLinkedList::GetData(int index)
{
	DBLLinkedList::Node* CurrentNode = headNode;
	for (int i = 0; i < index; i++)
	{
		CurrentNode = CurrentNode->next;
	}

	return CurrentNode->data;
}

void DBLLinkedList::SetValue(int index, DBLLinkedList::Node::Song AuxSong)
{
	DBLLinkedList::Node* CurrentNode = headNode;
	for (int i = 0; i < index; i++)
	{
		CurrentNode = CurrentNode->next;
	}

	CurrentNode->data.title = AuxSong.title;
	CurrentNode->data.artist = AuxSong.artist;
	CurrentNode->data.path = AuxSong.path;
}

void DBLLinkedList::InsertValue(int index, DBLLinkedList::Node::Song SongToInsert)
{
	if (index < 0 || index > size + 1)
	{
		return;
	}

	DBLLinkedList::Node* NewNode = new DBLLinkedList::Node();
	NewNode->data.title = SongToInsert.title;
	NewNode->data.artist = SongToInsert.artist;
	NewNode->data.path = SongToInsert.path;
	
	if (index == 0)
	{
		NewNode->previous = nullptr;
		NewNode->next = headNode;
		if (headNode)
		{
			headNode->previous = NewNode;
		}
		headNode = NewNode;
		size++;
	
		return;
	}

	DBLLinkedList::Node* CurrentNode = headNode;
	for (int i = 0; i < index - 1 && CurrentNode != nullptr; i++)
	{
		CurrentNode = CurrentNode->next;
	}

	if (CurrentNode)
	{
		NewNode->next = CurrentNode->next;
		NewNode->previous = CurrentNode;
		if (CurrentNode->next)
		{
			CurrentNode->next->previous = NewNode;
		}
		CurrentNode->next = NewNode;
		size++;
	}
	else
	{
		delete NewNode;
	}
}

void DBLLinkedList::DeleteNode(int index)
{
	if (index < 0 || index > size)
	{
		return;
	}

	if (index == 0)
	{
		DBLLinkedList::Node* NodeToDelete = headNode;
		headNode = headNode->next;
		if (headNode)
		{
			headNode->previous = nullptr;
		}
		delete NodeToDelete;
		size--;
		
		return;
	}

	DBLLinkedList::Node* CurrentNode = headNode;
	for (int i = 0; i < index - 1; i++)
	{
		CurrentNode = CurrentNode->next;
	}
	DBLLinkedList::Node* NodeToDelete = CurrentNode->next;
	CurrentNode->next = NodeToDelete->next;
	if (NodeToDelete->next)
	{
		NodeToDelete->next->previous = CurrentNode;
	}
	delete NodeToDelete;
	size--;
}

void DBLLinkedList::SwapValues(int index1, int index2)
{
	DBLLinkedList::Node* Node1 = headNode;
	for (int i = 0; i < index1; i++)
	{
		Node1 = Node1->next;
	}
	DBLLinkedList::Node* Node2 = headNode;
	for (int i = 0; i < index2; i++)
	{
		Node2 = Node2->next;
	}

	swap(Node1->data, Node2->data);
}

bool DBLLinkedList::IsEmpty()
{
	return (size == 0);
}

int DBLLinkedList::GetSize()
{
	return size;
}

int DBLLinkedList::GetIndex(DBLLinkedList::Node* node) // Indexing by 1
{
	DBLLinkedList::Node* currentNode = headNode;
	int index = 1;

	while (currentNode != nullptr)
	{
		if (currentNode->data.title == node->data.title && currentNode->data.artist == node->data.artist
			&& currentNode->data.path == node->data.path)
		{
			return index;
		}
		currentNode = currentNode->next;
		index++;
	}

	// If the node is not found
	return -1;
}

void DBLLinkedList::ShuffleList()
{
	if (size <= 2)
	{
		return; // No need to shuffle if there is only one element or none
	}

	// Seed the random number generator
	srand((time(NULL)));

	for (int i = 0; i < SHUFFLE_NUM; i++)
	{
		Node* lastNode = headNode;
		while (lastNode->next)
		{
			lastNode = lastNode->next;
		}

		//Traverse backward and shuffle
		for (Node* current = lastNode; current != headNode; current = current->previous)
		{
			int randomIndex = rand() % size;

			// Find the node at the random index
			Node* randomNode = GetNode(randomIndex);

			// Swap the data of current node and the randomly chosen node
			Node::Song temp = current->data;
			current->data = randomNode->data;
			randomNode->data = temp;
		}
	}
}

bool DBLLinkedList::IsSorted(const string& angleBracket)
{
	if (IsEmpty())
	{
		return true;
	}

	DBLLinkedList::Node* i = headNode;

	if (angleBracket == "<")
	{
		while (i->next != nullptr)
		{
			string currentTitle = i->data.path;
			transform(currentTitle.begin(), currentTitle.end(), currentTitle.begin(), ::tolower);
			string nextTitle = i->next->data.path;
			transform(nextTitle.begin(), nextTitle.end(), nextTitle.begin(), ::tolower);
			if (currentTitle > nextTitle)
			{
				return false;
			}
			i = i->next;
		}
	}
	else if (angleBracket == ">")
	{
		while (i->next != nullptr)
		{
			string currentTitle = i->data.path;
			transform(currentTitle.begin(), currentTitle.end(), currentTitle.begin(), ::tolower);
			string nextTitle = i->next->data.path;
			transform(nextTitle.begin(), nextTitle.end(), nextTitle.begin(), ::tolower);
			if (currentTitle < nextTitle)
			{
				return false;
			}
			i = i->next;
		}
	}

	return true;
}

DBLLinkedList::Node* HoarePartition(DBLLinkedList::Node* StartNode, DBLLinkedList::Node* EndNode,
	const string& angleBracket)
{
	string PivotData = EndNode->data.path;
	DBLLinkedList::Node* i = StartNode->previous;

	if (angleBracket == "<")
	{
		for (DBLLinkedList::Node* j = StartNode; j != nullptr && j != EndNode; j = j->next)
		{
			if (j->data.path <= PivotData)
			{
				if (i)
				{
					i = i->next;
				}
				else
				{
					i = StartNode;
				}
				if (i)
				{
					swap(i->data, j->data);
				}
			}
		}
	}
	else if (angleBracket == ">")
	{
		for (DBLLinkedList::Node* j = StartNode; j != nullptr && j != EndNode; j = j->next)
		{
			if (j->data.path >= PivotData)
			{
				if (i)
				{
					i = i->next;
				}
				else
				{
					i = StartNode;
				}
				if (i)
				{
					swap(i->data, j->data);
				}
			}
		}
	}

	if (i)
	{
		i = i->next;
	}
	else
	{
		i = StartNode;
	}

	if (i)
	{
		swap(i->data, EndNode->data);
	}
	return i;
}

void QuickSort(DBLLinkedList::Node* StartNode, DBLLinkedList::Node* EndNode, const string& AngleBracket)
{
	if (EndNode != nullptr && StartNode != EndNode && StartNode != EndNode->next)
	{
		DBLLinkedList::Node* MiddleNode = HoarePartition(StartNode, EndNode, AngleBracket);
		QuickSort(StartNode, MiddleNode->previous, AngleBracket);
		QuickSort(MiddleNode->next, EndNode, AngleBracket);
	}
}

void DBLLinkedList::SortList(const string& AngleBracket) //Quicksort using Hoare partitioning
{
	if (!headNode)
	{
		cerr << "List is empty..." << endl;
		return;
	}

	if (IsSorted(AngleBracket))
	{
		cout << "List already sorted..." << endl;
		return;
	}

	DBLLinkedList::Node* LastNode = headNode;
	for (int i = 0; i < GetSize() - 1; i++)
	{
		LastNode = LastNode->next;
	}

	QuickSort(headNode, LastNode, AngleBracket);
}