#include "StdAfx.h"
#include "DA2dims.h"
#include <iostream>
#include <fstream>
using namespace std;
DA2dims::DA2dims(void)
{
}

DA2dims::~DA2dims(void)
{
	/*
	for( int i = 0 ; i <_col ; i++ )
		delete [] _matrix[i] ;
	delete [] _matrix ;
	*/
}
int DA2dims::getcolnum()
{
	return _col;
}

int DA2dims::getrownum()
{
	return _row;
}

void DA2dims::inition(DA2dims Ins)
{
	_row=Ins.getrownum();
	_col=Ins.getcolnum();
	_matrix = new int*[_col];
    for( int i = 0 ; i < _col ; i++ )
      _matrix[i] = new int [_row];
	for(int i=0; i<_col; i++)
		for(int j=0; j<_row; j++)
			_matrix[i][j]= Ins.GetSpecEle(i,j);
}
void DA2dims::init(int col,int row)
{
	_row=row;
	_col=col;
	_matrix = new int*[_col];
    for( int i = 0 ; i < _col ; i++ )
      _matrix[i] = new int[_row];
}

void DA2dims::SetSpecEle(int colpoint, int rowpoint, int elevalue)
{
	this->_matrix[colpoint][rowpoint] = elevalue;
}

int DA2dims::GetSpecEle(int colpoint, int rowpoint)
{
	return _matrix[colpoint][rowpoint];
}

void DA2dims::free()
{
	for( int i = 0 ; i <_col ; i++ )
		delete [] _matrix[i] ;
	delete [] _matrix ;
}

string DA2dims::To_String()
{
	// print data to special string format
	string str = "";
	for(int i=0; i< _row; i++)
	{
		for(int j=0; j<_col; j++)
		{
			ostringstream buf;
			buf << _matrix[j][i]; // GetSpecEle(j, i);
			string tempstr = buf.str();
			str.append(tempstr);
			str.append(",");
		}
		str.append(";");
	}
	return str;
}

void DA2dims::bi_dataimport(string filename)
{
		/*char tilte[256];
		int item, tid;
		FILE *fPopu;
		fPopu=fopen(filename,"r");
		fscanf(fPopu,"%d",&item);
		fscanf(fPopu,"%d",&tid);
		this->init(item,tid);

		for (int i=0; i<tid; i++)
		{
			for (int j=0; j<item;j++)
			{
				int ins;
				fscanf(fPopu," %d %1s ",&ins,tilte);
				std::cout<< ins << " : " <<tilte <<endl;
				this->SetSpecEle(j,i,ins);
			}
			fscanf(fPopu,"%1s",tilte);
		}
		fclose(fPopu);
		this->_col=item;
		this->_row=tid;*/
	ifstream ipfile(filename );
	if(!ipfile.is_open())
		cout<< "open file error."<<endl;
	int lineNum=0;
	int itemNum, traNum;
	while(ipfile.good())
	{
		string line;
		if(lineNum ==0) 
		{
			getline(ipfile,line,'\n');
			itemNum = atoi(line.c_str());
			cout <<line<<endl;
		}
		else if(lineNum ==1) 
		{
			getline(ipfile,line,'\n');
			traNum = atoi(line.c_str());
			// khoi tao mang du lieu
			this->init(itemNum,traNum);
			cout <<line<<endl;
		}
		else if(lineNum>1)
		{		
			// doc du lieu 
			getline(ipfile,line,'\n');
			size_t found;
			int count =0; 
			while(line.length()>0)
			{
				found = line.find_first_of (",");
				if(found!=string::npos)
				{
					string str;
					str.append (line,0,found);
					this->SetSpecEle(count,lineNum-2,atoi(str.c_str()));
					cout <<str <<';';
					line.erase(0,found+1);
				}
				else
				{
					string str;
					str.append (line,0,line.length());
					this->SetSpecEle(count,lineNum-2,atoi(str.c_str()));
					cout <<str <<';';
					line.erase(0,line.length());
				}	
				count++;
			}
			/*for(int i=0; i< itemNum; i++)
			{
				getline(ipfile,line,',');
				cout <<line <<';';
			}*/
			cout <<endl;
		}
		lineNum++;
	}
}


unsigned int DA2dims::get_support_positive_frequence(vector<unsigned int> Pos_items)
{
	unsigned int frequent = 0;
	for (unsigned int i = 0; i < this->_row; i++)
	{
		bool flag = true;
		for (unsigned int j = 0; j < Pos_items.size(); j++)
		{
			if (this->_matrix[Pos_items[j]][i] == 0) //GetSpecEle(Pos_items[j], i) == 0)
			{
				flag = false;
				break;
			}
		}
		if (flag) frequent++;
	}
	return frequent;
}

unsigned int DA2dims::get_support_negative_positive_frequence(vector<unsigned int> Pos_items, vector<unsigned int> Neg_items)
{
	unsigned int frequent = 0;
	for (unsigned int i = 0; i < this->_row; i++)
	{
		bool flag = true;
		for (unsigned int j = 0; j < Pos_items.size(); j++)
		{
			if (this->_matrix[Pos_items[j]][i] == 0)//GetSpecEle(Pos_items[j], i) == 0)
			{
				flag = false;
				break;
			}
		}
		for (unsigned int j = 0; j < Neg_items.size(); j++)
		{
			if (this->_matrix[Neg_items[j]][i] == 1) //GetSpecEle(Neg_items[j], i) == 1)
			{
				flag = false;
				break;
			}
		}
		if (flag) frequent++;
	}
	return frequent;
}

void DA2dims::get_support_confident_positive_rule(vector<unsigned int> frequence, unsigned int Y, int Y_value, unsigned int* Supp, unsigned int* Conf)
{
	unsigned int supp = 0;
	unsigned int conf = 0;
	for (unsigned int i = 0; i < this->_row; i++)
	{
		bool flag = true;
		for (unsigned int j = 0; j < frequence.size(); j++)
		{
			if (this->_matrix[frequence[j]][i]==0 ) //GetSpecEle(frequence[j], i) == 0)
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			conf++;
			if (this->_matrix[Y][i] ==Y_value)// GetSpecEle(Y, i) == Y_value)
				supp++;
		}
	}
	*Supp = supp;
	*Conf = conf;
}

void DA2dims::get_support_confident_negative_positive_rule(vector<unsigned int> Pos_items, vector<unsigned int> Neg_items, unsigned int Y, int Y_value, unsigned int* Supp, unsigned int* Conf)
{
	unsigned int supp = 0;
	unsigned int conf = 0;

	for (unsigned int i = 0; i < this->_row; i++)
	{
		bool flag = true;
		for (unsigned int j = 0; j < Pos_items.size(); j++)
		{
			if (this->_matrix[Pos_items[j]][i]==0) // GetSpecEle(Pos_items[j], i) == 0)
			{
				flag = false;
				break;
			}
		}
		for (unsigned int j = 0; j < Neg_items.size(); j++)
		{
			if (this->_matrix[Neg_items[j]][i] ==1)// GetSpecEle(Neg_items[j], i) == 1)
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			conf++;
			if (this->_matrix[Y][i] == Y_value)// GetSpecEle(Y, i) == Y_value)
				supp++;
		}
	}
	*Supp = supp;
	*Conf = conf;
}

vector<unsigned int> DA2dims::getrow(unsigned int Index)
{
	// mac dinh bo bient target o cuoi cung
	vector<unsigned int> result;
	for (unsigned int i = 0; i < this -> _col-1; i++)
	{
		result.push_back(this->_matrix[i][Index]); // GetSpecEle(i, Index));
	}
	return result;
}