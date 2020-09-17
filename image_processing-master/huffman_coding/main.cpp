#include "linkedlist.h"
#include <vector>
#include <fstream>


struct Encoding {
	int pixel;
	string Bits;
};
int count_ = 0;
vector<Encoding> vec;
int bit_size;
unsigned char** MemAlloc2D(int Height, int Width, int Value) {
	unsigned char** result = new unsigned char*[Height];

	for (int h = 0; h < Height; h++) {
		result[h] = new unsigned char[Width];
		memset(result[h], Value, sizeof(unsigned char)*Width);
	}
	return result;
}
void MemFree2D(unsigned char** Mem, int nHeight) {
	for (int n = 0; n < nHeight; n++) {
		delete[] Mem[n];
	}
	delete[] Mem;
}
void Setting_Bits(Node* parent, string bit) {
	
	string my_bit = bit + parent->Get_Bit();

	if (parent->left_child == nullptr && parent->right_child == nullptr) {
		parent->Set_myBit(my_bit);
		Encoding enc;
		enc.pixel = parent->Get_pixel();
		enc.Bits = parent->Get_myBit();
		vec.push_back(enc);
		//cout << my_bit << " : " << parent->Get_prob() << endl;
		count_ += my_bit.length()*parent->Get_hist();
		return;
	}
	Setting_Bits(parent->left_child, my_bit);
	Setting_Bits(parent->right_child, my_bit);
}
void Set(Node* Cur) {
	while (Cur->left_child != nullptr && Cur->right_child != nullptr) {
		Cur->right_child->Set_Bit("0");
		Cur->left_child->Set_Bit("1");
		if (Cur->left_child != nullptr){
			Node* temp = Cur->right_child;
			Cur = Cur->left_child;
			Set(Cur);
			Set(temp);
		}
	}
}

string Encoded_String(unsigned char** In, int Height, int Width, vector<Encoding> mapping) {
	string str = "";
	for (int h = 0; h < Height; h++) {
		for (int w = 0; w < Width; w++) {
			
			for (int i = 0; i < mapping.size(); i++) {
				if (In[h][w] == mapping.at(i).pixel) {
					str += mapping.at(i).Bits;
					break;
				}
			}

		}
	}
	return str;
}
unsigned char* Encode_Byte(string str) {
	int length = str.length();
	int Byte_length = (int)(length / 8) + 1;
	bit_size = Byte_length;
	int pows[8];
	for (int i = 0; i < 8; i++)
		pows[i] = 0;
	int temp = 0;
	unsigned char* bitstream = new unsigned char[Byte_length];
	
	int index = 0;
	for (int i = 0; i < length-8; i = i + 8) {
		temp = 0;
		for (int j = 0; j < 8; j++) {
			if (str.at(i + j) == '1') {
				pows[j] = 1;
			}
			else {
				pows[j] = 0;
			}
			if(pows[j] == 1) 
				temp += pow(2, (7 - j)*pows[j]);
		}
		bitstream[index] = (unsigned char)temp;
		index++;
	}
	temp = 0;
	for (int i = 0; i < length - (index * 8); i++) {
		if (str.at(index * 8 + i) == '1') {
			pows[i] = 1;
		}
		else {
			pows[i] = 0;
		}
		temp += pow(2, (7 - i)*pows[i]);
	}
	bitstream[index] = (unsigned char)temp;
	return bitstream;
}
unsigned char** Decode_Byte(string str,int Height, int Width, vector<Encoding> mapping) {
	unsigned char** Result = MemAlloc2D(Height, Width, 0);
	int length = str.length();
	cout << length << endl;
	string res = "";
	unsigned char pixel_v;
	string temp = "";
	int h = 0;
	int w = 0;
	for (int i = 0; i < length; i++) {
		temp += str.at(i);
		cout << i << endl;
		for (int j = 0; j < mapping.size(); j++) {
			//cout << j << endl;
			if (temp.compare(mapping.at(j).Bits) == 0) {
				Result[h][w] = (unsigned char)mapping.at(j).pixel;
				temp = "";
				if (w != Width - 1)
					w++;
				else {
					h++;
					w = 0;
				}
				break;
			}
		}
	}
	return Result;
}
unsigned char** Decoded_String(string str, int Height, int Width, vector<Encoding> mapping) {
	unsigned char** Result = MemAlloc2D(Height, Width, 0);
	int length = str.length();
	string res = "";
	string temp = "";
	int h = 0;
	int w = 0;
	for (int i = 0; i < length; i++) {
		temp += str.at(i);
		for (int j = 0; j < mapping.size(); j++) {
			if (temp.compare(mapping.at(j).Bits) == 0) {
				res += mapping.at(j).pixel + ",";
				cout << (int)mapping.at(j).pixel << " ";
				Result[h][w] = mapping.at(j).pixel;
				temp = "";
				if (w != Width - 1)
					w++;
				else {
					h++;
					w = 0;
				}
				break;
			}
		}
//		cout << i  << ", " << length << endl;
	}

	return Result;
}
string TenConvertTwo(int num) {
	string stream = "";
	int bits[8];
	for (int i = 0; i < 8; i++)
		bits[i] = 0;
	int index = 7;
	while (num != 0) {
		bits[index--] = num % 2;
		num /= 2;
	}
	for (int i = 0; i < 8; i++) {
		stream += to_string(bits[i]);
	}
	return stream;
}
int main() {
	FILE *fp_in = NULL, *fp_out = NULL;

	LinkedList li;
	int size = 0;
	fopen_s(&fp_in, "lena256.raw", "rb");
	fopen_s(&fp_out, "lena256_compress.raw", "wb");
	int Width = 256, Height = 256;
	unsigned char** Img = MemAlloc2D(Height, Width, 0);
	for (int h = 0; h < Height; h++) {
		fread(Img[h], sizeof(unsigned char), Width, fp_in);
	}
	int Hist[256];
	for (int i = 0; i < 256; i++)
		Hist[i] = 0;
	for (int h = 0; h < Height; h++) {
		for (int w = 0; w < Width; w++) {
			Hist[Img[h][w]]++;
		}
	}
	
	float Prob[256] = { 0.0, };
	for (int i = 0; i < 256; i++) {
		Prob[i] = (float)Hist[i] / (Height*Width);
	}
	
	Data data[256];
	for (int i = 0; i < 256; i++) {
		if (Hist[i] == 0)
			continue;
		data[i].Pixel_Value = i;
		data[i].Hist_Value = Hist[i];
		data[i].Prob_Value = Prob[i];
		Node* node = new Node(data[i]);
		li.Insert_Head(node);
		size++;
	}
	li.Sorting();
	//li.ShowList_FromHead();
	
	while (li.Tail->Get_prob() != 1.0) {
		li.Link_Parent(li.Tail->prev, li.Tail);
	}
	
	Node* cu = li.Tail;
	Set(cu);
	Setting_Bits(li.Tail, "");

	
	
	string result = Encoded_String(Img, Height, Width, vec);
	unsigned char* bitstream = Encode_Byte(result);
	FILE *fp_encoding = NULL, *fp_decoding = NULL;
	fopen_s(&fp_encoding, "lena256_compressed.txt", "wb");
	fwrite(bitstream, sizeof(unsigned char), bit_size, fp_encoding);
	fclose(fp_encoding);
	
	cout << "Encoding 완료" << endl;
	fopen_s(&fp_decoding, "lena256_compressed.txt", "rb");
	unsigned char Byte;
	string de_stream = "";
	while (true) {
		if (fread(&Byte, sizeof(unsigned char), 1, fp_decoding) == 1) {
			de_stream += TenConvertTwo((int)Byte);
		}
		else {
			cout << "end" << endl;
			break;
		}
	}
	unsigned char** Decoded_Img = Decode_Byte(de_stream, Height, Width, vec);
	FILE *fp_de = NULL;
	fopen_s(&fp_de, "lena256_decoded.raw", "wb");
	for (int h = 0; h < Height; h++) {
		fwrite(Decoded_Img[h], sizeof(unsigned char), Width, fp_de);
	}
	cout << "Decoded!" << endl;
	//fwrite(result, sizeof(string), result.length(), fp_out);
	//ofstream outf("lena256_Huff_string.txt");
	//outf << result;
	//outf.close();
	
//	ifstream inf("lena256_Huff_string.txt");
//	string str;
//	inf >> str;
//	inf.close();
	
	
//	unsigned char** Out = Decoded_String(de_stream, Height, Width, vec);
	//cout << ori;
//	for (int h = 0; h < Height; h++) {
//		fwrite(Out[h], sizeof(unsigned char), Width, fp_out);
//	}
//	cout << "Decoding 완료" << endl;
	int a;
	cin >> a;
	return 0;
}
