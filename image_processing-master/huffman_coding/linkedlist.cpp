#include "linkedlist.h"

LinkedList::LinkedList() {
	Head = nullptr;
	Tail = nullptr;
}
void LinkedList::Insert_Head(Node* node) {
	if (Head == nullptr) {
		Head = node;
		Tail = node;
	}
	else {
		node->Set_next(Head);
		Head->Set_prev(node);
		Head = node;
	}
}
void LinkedList::Insert_Tail(Node* node) {
	if (Tail == nullptr) {
		Head = node;
		Tail = node;
	}
	else {
		node->Set_prev(Tail);
		Tail->Set_next(node);
		Tail = node;
	}
}
void LinkedList::Insert_Front(Node* new_node, Node* base_node) {
	new_node->Set_next(base_node);
	new_node->Set_prev(base_node->prev);
	base_node->prev->Set_next(new_node);
	base_node->Set_prev(new_node);
	if (base_node == Head)
		Head = new_node;
}
void LinkedList::Remove(Node* node) {
	if (node == Head) {
		node->next->Set_prev(nullptr);
		Head = node->next;
	}
	else if (node == Tail){
		node->prev->Set_next(nullptr);
		Tail = node->prev;
	}
	else {
	}
	delete node;
}
void LinkedList::Change(Node* first_node, Node* second_node) {
	Data f_d = first_node->Get_Data();
	Data s_d = second_node->Get_Data();
	first_node->Set_Data(s_d);
	second_node->Set_Data(f_d);
}
Node* LinkedList::Search_Node(Data data) {
	Node* searchNode = Head;
	do {
		if (searchNode->Get_Data() == data) {
			return searchNode;
		}
		searchNode = searchNode->next;
	} while (searchNode != Tail->next);
	return Tail;
}

void LinkedList::Sorting() {
	Node* I_cursor = Head;
	Node* J_cursor = nullptr;
	int index = 0;
	do {
		J_cursor = I_cursor->next;
		do {
			if (I_cursor->Get_hist() < J_cursor->Get_hist()) {
				Change(I_cursor, J_cursor);
			}
			J_cursor = J_cursor->next;
		} while (J_cursor != Tail->next);
		if(I_cursor->next != NULL)
		I_cursor = I_cursor->next;
	} while (I_cursor != Tail);
}
void LinkedList::ShowList_FromHead() {
	Node* Cursor = Head;
	do {
		cout << Cursor->Get_prob() << endl;
		Cursor = Cursor->next;
	} while (Cursor != Tail->next);
}
Node* LinkedList::Get_Parent(Node* left, Node* right) {
	Node* parent = new Node;
	Data p_data;
	p_data.Hist_Value = 0;
	p_data.Pixel_Value = 0;
	p_data.Prob_Value = left->Get_prob() + right->Get_prob();
	parent->Set_Data(p_data);
	parent->Set_left_child(left);
	parent->Set_right_child(right);
	return parent;
}
Node* LinkedList::Find_Parent_Next(Node* node) {
	Node* search_node = Head;
	if (search_node->Get_prob() < node->Get_prob()) {

	}
	do {
		if (search_node->Get_prob() <= node->Get_prob()) {
			//Tail = node->left_child->prev;
			return search_node;
		}
		search_node = search_node->next;
	} while (search_node != nullptr);
	return nullptr;
}
void LinkedList::Link_Parent(Node* left_child, Node* right_child) {
	Node *parent = Get_Parent(left_child, right_child);
	Node *base = Find_Parent_Next(parent);
	if (base == nullptr) {
		Insert_Tail(parent);
	}
	else if(base == Head) {
		Insert_Head(parent);
		Tail = left_child->prev;
	}
	else {
		Insert_Front(parent, base);
		Tail = left_child->prev;
	}
	left_child->Set_next(nullptr);
	left_child->Set_prev(nullptr);
	right_child->Set_next(nullptr);
	right_child->Set_prev(nullptr);
}