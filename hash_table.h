#pragma once
#include "stdafx.h"
typedef struct ht_set // cấu trúc mảng băm
{
	//int it_set[Item_size];
	tr_charm *closed_member;
	ht_set* pointer_set;
};
const int M =4093;
class hash_table
{
private:
	ht_set closed_pattern[M];
public:
	hash_table(void);
public:
	~hash_table(void);
	void init();// khoi tao bang bam
	void check_subsumed(tr_charm *node);// dua mot node vao bang bam
	int exist(tr_charm node,int hash_num);// kiem tra xem mot node co phai da ton tai
	void insert(tr_charm * node,int hash_num);// them mot node vao bang bam
	void print(char filename[]);// in ket qua bang bam

};
