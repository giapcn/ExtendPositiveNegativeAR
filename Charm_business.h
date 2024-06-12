#pragma once

#include "stdafx.h";
#include "string.h";
#include "hash_table.h";

class Charm_business
{

private: 
	tr_charm head;// luu giu cay ket qua
	double min_level;
	int total_Tid;
	hash_table C;
public:
	Charm_business(void);
public:
	~Charm_business(void);
private:
	int Item_compare(tr_charm *node1,tr_charm *node2);// so sanh luat 2 nut
	tr_charm blend(tr_charm node1, tr_charm node2); // tron gia tri itemset va Tidset cua 2 nhanh
	int Charm_extend( tr_charm * node); // xet thuat toan ket hop cac nhanh cung muc
	int Charm_property(tr_charm* node1,tr_charm*node2, tr_charm *New_node); // so sanh nut dang xet va itemset ket hop moi duoc tao thanh
	int printn(tr_charm node);
public:
	int init_not(db_pattern example_db, double min_sup);// tao ra tap du lieu don ban dau, tra ve 0 neu thanh cong, #0 neu co loi
	int Charm_main();// ham thu tuc chinh xu ly du lieu nhi phan va tra ve, tra ve 0 neu thanh cong, #0 neu co loi
	int print();
	void print(char filename[]);

};
