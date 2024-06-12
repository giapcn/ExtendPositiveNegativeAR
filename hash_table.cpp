#include "StdAfx.h"
#include "hash_table.h"

hash_table::hash_table(void)
{
}

hash_table::~hash_table(void)
{
}

void hash_table::init()// khoi tao bang bam
{
	for(int i=0; i<M; i++)
	{
		this->closed_pattern[i].closed_member=NULL;
		this->closed_pattern[i].pointer_set=NULL;
	}
}
void hash_table::check_subsumed(tr_charm *node)// dua mot node vao bang bam
{
	int hash_num=0;
		for( int i=0; i< node->real_tid; i++) hash_num= hash_num + node->id_set[i];
	hash_num= hash_num % M;
	if (exist(*node,hash_num)==1)
	{
		this->insert(node,hash_num);
	}
}
int hash_table::exist(tr_charm node, int hash_num)// kiem tra xem mot node co phai da ton tai
{
	ht_set * temp;
	temp = &this->closed_pattern[hash_num];
	while(temp->pointer_set!= NULL)
	{
		temp=temp->pointer_set;
		int st=0;
		for( int i=0; i<node.real_item; i++)
		{
			int comp=0;
			for (int j=0; j<temp->closed_member->real_item; j++)
			{
				if(node.it_set[i]==temp->closed_member->it_set[j] )
				{
					comp=1;// da ton tai
					break;
				}
			}
			if (comp==0) 
			{
				st=1;//khong giong
				break;
			}
		}
		if (st==0)// ddax tim thay
			return 0;
	}
	return 1;// khong co trong bang bam
}
void hash_table::insert(tr_charm * node, int hash_num)// them mot node vao bang bam
	{
		ht_set *temp;
		temp = &this->closed_pattern[hash_num];
		while (temp->pointer_set!=NULL) temp=temp->pointer_set;
		temp->pointer_set= new ht_set;
		temp->pointer_set->closed_member=node;
		temp->pointer_set->pointer_set=NULL;
		
	}
void hash_table::print(char filename[])// in ket qua bang bam
{
	FILE *fp;
	fp= fopen(filename,"a+t");
	for (int i=0; i<M; i++)
	{
		ht_set *temp;
		temp = &this->closed_pattern[i];
		while(temp->pointer_set!=NULL)
		{
			temp=temp->pointer_set;
			for(int j=0; j< temp->closed_member->real_item;j++) 
				fprintf(fp,"%d",temp->closed_member->it_set[j]);
			fprintf(fp,",");	
		}
	}
	fclose(fp);
}