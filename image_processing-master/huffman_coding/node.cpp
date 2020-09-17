#include "node.h"

bool Data::operator ==(const Data data) {
	if ((Pixel_Value == data.Pixel_Value) && (Hist_Value == data.Hist_Value) && (Prob_Value == data.Prob_Value))
		return true;
	else
		return false;
}
Node::Node() {
	next = nullptr;
	prev = nullptr;
	left_child = nullptr;
	right_child = nullptr;
}
Node::Node(Data data) {
	data_field.Pixel_Value = data.Pixel_Value;
	data_field.Hist_Value = data.Hist_Value;
	data_field.Prob_Value = data.Prob_Value;
	next = nullptr;
	prev = nullptr;
	left_child = nullptr;
	right_child = nullptr;
}
int Node::Get_pixel() {
	return data_field.Pixel_Value;
}
int Node::Get_hist() {
	return data_field.Hist_Value;
}
float Node::Get_prob() {
	return data_field.Prob_Value;
}


string Node::Get_Bit() {
	return data_field.Huff_bit;
}
void Node::Set_Bit(string bit) {
	data_field.Huff_bit = bit;
}
void Node::Set_myBit(string mybit) {
	data_field.my_bit = mybit;
}
string Node::Get_myBit() {
	return data_field.my_bit;
}


Data Node::Get_Data() {
	return data_field;
}
void Node::Set_Data(Data data) {
	data_field = data;
}
void Node::Set_next(Node* node) {
	next = node;
}
void Node::Set_prev(Node* node) {
	prev = node;
}
void Node::Set_left_child(Node* node) {
	left_child = node;
}
void Node::Set_right_child(Node* node) {
	right_child = node;
}