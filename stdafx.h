
#pragma once

#include "targetver.h"
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <vector>

using namespace std;
/* kich thuoc du lieu quyet dinh viec con loading toan bo du lieu vao mot cau truc nao do 
va xu ly trong RAM hay khong.*/

// struct for EPSAR

typedef struct epnAR
{
	bool precedent;
	struct epnAR *far, *mot; // rules that were used to constructed this rule, its are NULL for rule with length 1;
	struct epnAR *pre, *next; // rules of the same lenght;
	vector<unsigned int> pos; // positive items  
	vector<unsigned int> neg; // negative items
	unsigned int frequent; // = sup (pos U neg U precedent) ->> real supp = prequent/ trans number
	unsigned int confident;// = sup (pos U neg ) ->> real confident = frequent/confident
}epnAR; 

//struct for all rules

typedef struct ARlist
{
	int length;
	epnAR *head;
	struct ARlist *next;
}ARlist;