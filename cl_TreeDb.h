#pragma once
#include "stdafx.h";
class cl_TreeDb
{
int Item_size;
int Tid_size;

db_pattern *head;

public:
	cl_TreeDb(void);
	cl_TreeDb(int it_size, int Ti_size);
public:
	~cl_TreeDb(void);
};
