#pragma once
#include "node.h"

class LinkedList {
public:
	Node *Head, *Tail;

	LinkedList();
	void Insert_Head(Node* node);
	void Insert_Tail(Node* node);
	void Insert_Front(Node* new_node, Node* base_node);
	void Remove(Node* node);
	void Change(Node* first_node, Node* second_node);
	Node* Search_Node(Data data);
	
	void Sorting();
	void ShowList_FromHead();
	Node* Get_Parent(Node* left, Node* right);
	Node* Find_Parent_Next(Node* node);
	void Link_Parent(Node* left_child, Node* right_child);
};