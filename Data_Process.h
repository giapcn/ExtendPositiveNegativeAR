#pragma once
#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
typedef struct attr_str
{
	std::string name;//[100];
	attr_str * next;
};
using namespace std;

class Data_Process
{
private:
	db_pattern bi_db;

public:
	Data_Process(void);
public:
	~Data_Process(void);
	db_pattern bi_blind(char filename[]);// binary data file
	db_pattern gl_blind(char filename[]);// general data file
	void print_db();	
};
