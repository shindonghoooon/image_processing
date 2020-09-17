#pragma once
#include <string>
#include <iostream>

using namespace std;

class Data {
public:
	string Huff_bit;
	string my_bit;
	int Pixel_Value;
	int Hist_Value;
	float Prob_Value;
	bool operator ==(const Data data);
};
class Node {
private:
	Data data_field;
public:
	Node *next, *prev, *left_child, *right_child;
	Node();
	Node(Data data);
	int Get_pixel();
	int Get_hist();
	float Get_prob();
	Data Get_Data();
	string Get_Bit();
	string Get_myBit();
	void Set_Bit(string bit);
	void Set_myBit(string bit);
	void Set_Data(Data data);
	void Set_next(Node* node);
	void Set_prev(Node* node);
	void Set_left_child(Node* node);
	void Set_right_child(Node* node);
};