#pragma once
#include <string>
#include <sstream>
using namespace std;
class DA2dims
{
	//dynamic two dimention array structure
	/* this structure is used to store input data- input matrix*/
private: 
	int ** _matrix;
	int _row; // number of rows
	int _col; // number of column
public:
	void init(int col,int row); //declares an instance of input matrix
	void free(); // free data
	// business functions
	int getrownum();
	int getcolnum();
	void SetSpecEle (int colpoint,int rowpoint, int elevalue);
	int GetSpecEle (int colpoint,int rowpoint);

	vector<unsigned int> getrow(unsigned int Index);
	string To_String();
	// import data from file
	void bi_dataimport (string filename);// binary data file
	// get support
	unsigned int get_support_positive_frequence(vector<unsigned int> frequence);
	unsigned int get_support_negative_positive_frequence(vector<unsigned int> Pos_items, vector<unsigned int> Neg_items);

	void get_support_confident_positive_rule(vector<unsigned int> frequence, unsigned int Y,int Y_value, unsigned int* Supp, unsigned int* Conf);
	void get_support_confident_negative_positive_rule(vector<unsigned int> Pos_items, vector<unsigned int> Neg_items, unsigned int Y, int Y_value, unsigned int* Supp, unsigned int* Conf);
public:
	DA2dims(void);
	void inition(DA2dims Ins);
public:
	~DA2dims(void);
};
